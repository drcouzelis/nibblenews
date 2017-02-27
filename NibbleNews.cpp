#include "NibbleNews.h"

#include <Alert.h>
#include <Entry.h>
#include <File.h>
#include <FindDirectory.h>
#include <Path.h>
#include <iostream>
#include <mrss.h>
#include <stdlib.h>
#include <time.h>

#include "Constants.h"
#include "Feed.h"
#include "MainWindow.h"


#define SETTINGS_FILENAME "NibbleNews"

#define MICROSECONDS_PER_MINUTE (1000000L * 60L)
#define SLEEP_TIME (1L * MICROSECONDS_PER_MINUTE) // Minutes * MSecs Per Min


Item* find_item(Item* item, BObjectList<Item>* list) {
	
	for (int32 i = 0; i < list->CountItems(); i++) {
		
		Item* test = list->ItemAt(i);
		
		if (item->fTitle == test->fTitle &&	item->fLink == test->fLink &&
											item->fDate == test->fDate)
			return test;
	}
		
	return NULL;
}


bool continue_updating_feeds = true;


int32 update_feed(void* data)
{
	BString* location = (BString*) data;

	// Use the MRSS library to get a list of the new updates from the feed
	mrss_t* rss = NULL;
	int32 error = mrss_parse_url(const_cast<char*>(location->String()), &rss);
	
	Settings* settings = ((NibbleNews*)be_app)->GetSettings();
	
	// Lock the list of feeds before modifying them
	if (!settings->Lock()) {
		mrss_free(rss);
		return B_ERROR;
	}
	
	// Find the feed that matches this location
	Feed* feed = NULL;
	for (int32 i = 0; i < settings->fFeeds.CountItems(); i++) {
		Feed* feedToCheck = settings->fFeeds.ItemAt(i);
		if (feedToCheck->fLocation == *location)
			feed = feedToCheck;
	}
	
	// If the feed isn't in this list anymore then there's nothing to do
	if (!feed) {
		mrss_free(rss);
		settings->Unlock();
		return B_ERROR;
	}
	
	// If there was no response from the feed, mark it as such
	if (error) {
		feed->fFailed = true;
		mrss_free(rss);
		settings->Unlock();
		return B_ERROR;
	}
	
	// Update the data in the feed with what you just collected
	feed->fFailed = false;
	feed->fTitle = rss->title;
	feed->fLink = rss->link;

	// Create a list of items from the updates you just collected	
	BObjectList<Item> items;
	mrss_item_t* item = rss->item;
	while (item) {
		items.AddItem(new Item(item->title, item->link, item->pubDate));
		item = item->next;
	}
	
	// Mark the items in the new list as "read" from the old list
	for (int32 i = 0; i < items.CountItems(); i++) {
		
		Item* newItem = items.ItemAt(i);
		
		Item* oldItem = find_item(newItem, &(feed->fItems));
		
		if (oldItem) {
			newItem->fRead = oldItem->fRead;
			newItem->fVisible = oldItem->fVisible;
		}
	}
	
	// Clear the old list of items
	for (int32 i = 0; Item* anItem = feed->fItems.ItemAt(i); i++)
		delete anItem;
	feed->fItems.MakeEmpty();
	
	// Move items from the new list to the old list
	for (int32 i = 0; Item* anItem = items.ItemAt(i); i++)
		feed->fItems.AddItem(anItem);
	
	be_app->PostMessage(MSG_FEEDS_CHANGED);

	settings->Unlock();
	mrss_free(rss);
	
	return B_OK;
}


int32 update_all_feeds(void* data)
{
	Settings* settings = ((NibbleNews*)be_app)->GetSettings();
	
	std::cout << "UPDATE_FEEDS_THREAD: Updating feeds..." << std::endl;

	if (!settings->Lock())
		return B_ERROR;

	// Copy the links to all of the feeds into a new list.
	// This allows you to update the feeds without "locking"
	// them in this thread.
	BObjectList<BString> locations;
	for (int32 i = 0; i < settings->fFeeds.CountItems(); i++) {
		BString* location = new BString(settings->fFeeds.ItemAt(i)->fLocation);
		locations.AddItem(location);
	}

	settings->Unlock();

	// Update the individual feeds
	for (int32 i = 0; i < locations.CountItems(); i++) {
		
		update_feed(locations.ItemAt(i));
		
		if (!continue_updating_feeds)
			break;
	}

	// All done!
	
	// Mark the time of the last update
	if (settings->Lock()) {
		settings->fLastUpdate = time(NULL); // The current time
		settings->Unlock();
	}
	
	// Delete the temporary location strings
	for (int32 i = 0; BString* aString = locations.ItemAt(i); i++)
		delete aString;


	std::cout << "UPDATE_FEEDS_THREAD: Finished updating feeds." << std::endl;
	
	return B_OK;
}


NibbleNews::NibbleNews(void)
	:	BApplication("application/x-vnd.drc-NibbleNews")
{
	// Reload saved settings
	LoadSettings();

	fMainWindow = new MainWindow(fSettings.fMainWindowFrame);
	fSettingsWindow = NULL;

	LoadDummyFeeds(); // TEMP
	fUpdateRunner = new BMessageRunner(this, new BMessage(MSG_TIME_TO_UPDATE_FEEDS), SLEEP_TIME);
}


void
NibbleNews::LoadDummyFeeds()
{
	Feed* feed;
	
	feed = new Feed(BString("http://haikuware.com/remository/rss-feed?days=30&max=50&rtype=newest"));
	//feed->Update();
	fSettings.fFeeds.AddItem(feed);
	
	feed = new Feed(BString("http://archlinux.me/drcouzelis/feed/"));
	//feed->Update();
	fSettings.fFeeds.AddItem(feed);
	
	//fMainWindow->PostMessage(MSG_FEEDS_CHANGED);
}


void
NibbleNews::LoadSettings()
{
	BMessage msg;
	
	BPath settingsPath;
	if (find_directory(B_USER_SETTINGS_DIRECTORY, &settingsPath) != B_OK)
		return;

	settingsPath.Append(SETTINGS_FILENAME);

	BFile file(settingsPath.Path(), B_READ_ONLY);
	if (file.InitCheck() == B_OK)
		msg.Unflatten(&file);

	if (msg.FindRect("MainWindowFrame", &(fSettings.fMainWindowFrame)) != B_OK)
		fSettings.fMainWindowFrame.Set(100, 100, 600, 500);
}


void
NibbleNews::SaveSettings()
{
	BMessage msg;
	
	msg.AddRect("MainWindowFrame", fMainWindow->Frame());
	
	BPath settingsPath;
	if (find_directory(B_USER_SETTINGS_DIRECTORY, &settingsPath) != B_OK)
		return;
	settingsPath.Append(SETTINGS_FILENAME);

	BFile file(settingsPath.Path(), B_CREATE_FILE | B_ERASE_FILE | B_WRITE_ONLY);
	if (file.InitCheck() == B_OK)
		msg.Flatten(&file);
}


Settings*
NibbleNews::GetSettings(void)
{
	return &fSettings;
}


void
NibbleNews::ReadyToRun()
{
	fMainWindow->Show();
	
	this->PostMessage(MSG_TIME_TO_UPDATE_FEEDS);
}


bool
NibbleNews::QuitRequested()
{
	// A global variable to help stop the feed update thread
	continue_updating_feeds = false;
	
	delete fUpdateRunner;

	if (fMainWindow)
		fMainWindow->Hide();

	if (fSettingsWindow)
		fSettingsWindow->Hide();
	
	SaveSettings();

	return true;
}


void
NibbleNews::MessageReceived(BMessage* msg)
{
	thread_id aThread;
	
	switch (msg->what)
	{
		case MSG_SETTINGS:
		
			if (fSettingsWindow == NULL) {
				
			    fSettingsWindow = new SettingsWindow();
			    fSettingsWindow->Show();
			    
			} else {
				
			    if (fSettingsWindow->Lock()) {
			        if (fSettingsWindow->IsHidden()) {
			            // The user closed the window
			            fSettingsWindow->Show();
			        } else {
			            // The window is just out of sight somewhere
			            fSettingsWindow->Activate();
			        }
			        fSettingsWindow->Unlock();
			    }
			    
			}
			break;
			
		case MSG_TIME_TO_UPDATE_FEEDS:
			aThread = spawn_thread(update_all_feeds, "UpdateFeeds", B_NORMAL_PRIORITY, NULL);
			resume_thread(aThread);
			break;
			
		case MSG_FEEDS_CHANGED:
			fMainWindow->PostMessage(msg);
			break;
			
		default:
			BApplication::MessageReceived(msg);
			break;
	}
}


#define APP_NAME "NibbleNews Feed Aggregator 0.1\n\n"
#define APP_AUTHOR "David Couzelis 2012"


void
NibbleNews::AboutRequested()
{

	char* about;

	// Show a random haiku poem on the "About" page
	static bool initRandom = false;
	
	if (!initRandom) {
		srand(time(NULL));
		initRandom = true;
	}
	
	int32 low = 0;
	int32 high = 4;
	int32 randomNum = (rand() % (high - low + 1)) + low;

	switch (randomNum) {
		case 0:
			about =
				APP_NAME
				"Do you like the news?\n"
				"Are you lazy and hate work?\n"
				"I will push you news!\n\n"
				APP_AUTHOR;
			break;
		case 1:
			about =
				APP_NAME
				"Behold! A reader\n"
				"For RSS on Haiku.\n"
				"Updates, sans effort.\n\n"
				APP_AUTHOR;
			break;
		case 2:
			about =
				APP_NAME
				"Oh hey, you guys, look\n"
				"At this thing that I have made.\n"
				"RSS reader!\n\n"
				APP_AUTHOR;
			break;
		case 3:
			about =
				APP_NAME
				"Swim in the vast Web.\n"
				"Command effortless effort.\n"
				"Update stress leaves you.\n\n"
				APP_AUTHOR;
			break;
		default:
			about =
				APP_NAME
				"Get news from the Net\n"
				"Using free open standards\n"
				"With this feed reader.\n\n"
				APP_AUTHOR;
			break;
	}

	(new BAlert("About NibbleNews", about, "Okay"))->Go();
}


void
NibbleNews::ArgvReceived(int32 argc, char** argv)
{
	// If there was a command line argument, load it as a new feed
	
	if (argc > 1) {
		std::cout << "Pretending to load arg..." << std::endl;
		
		// Load arguments into the "Add Feed" window.
		// The first argument is the feed link.
		// The second argument is the optional feed name.
	}
}


int
main(void)
{
	NibbleNews *app = new NibbleNews();
	app->Run();
	delete app;
	return 0;
}
