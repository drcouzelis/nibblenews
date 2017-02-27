#include "MainWindow.h"

#include <Application.h>
#include <LayoutBuilder.h>
#include <Menu.h>
#include <MenuItem.h>
#include <Roster.h>
#include <View.h>
#include <iostream>

#include <ListView.h>

#include "Constants.h"
#include "Feed.h"
#include "FeedListView.h"
#include "Item.h"
#include "NibbleNews.h"


enum
{
	COLUMN_INDEX_NEW = 0,
	//COLUMN_INDEX_ICON, // TEMP
	COLUMN_INDEX_NAME,
	COLUMN_INDEX_DATE,
	COLUMN_INDEX_FEED
};


MainWindow::MainWindow(BRect frame)
	:	BWindow(frame, "NibbleNews", B_TITLED_WINDOW,
			B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS)
{
	Settings* settings = ((NibbleNews*)be_app)->GetSettings();
	
	// Feeds List
	fFeedListView = new BColumnListView("List", 0);
	fFeedListView->SetInvocationMessage(new BMessage(MSG_OPEN_ITEM_LINK));
	
	fNewColumn = new BStringColumn("*", 16, 16, 16, B_TRUNCATE_END);
	fFeedListView->AddColumn(fNewColumn, COLUMN_INDEX_NEW);
	
	// TODO
	// Favicons are 16x16 pixels in size
	//fIconColumn = new BStringColumn("", 16, 16, 16, B_TRUNCATE_END);
	//fFeedListView->AddColumn(fIconColumn, COLUMN_INDEX_ICON);
	
	float min = 80;
	float max = 600;
	
	fNameColumn = new BStringColumn("News", settings->fColumnWidthName, min, max, B_TRUNCATE_END);
	fFeedListView->AddColumn(fNameColumn, COLUMN_INDEX_NAME);
	
	fDateColumn = new BDateColumn("Date", settings->fColumnWidthDate, min, max);
	fFeedListView->AddColumn(fDateColumn, COLUMN_INDEX_DATE);
	
	fFeedColumn = new BStringColumn("Feed", settings->fColumnWidthFeed, min, max, B_TRUNCATE_END);
	fFeedListView->AddColumn(fFeedColumn, COLUMN_INDEX_FEED);

	// Sort	
	fFeedListView->SetSortingEnabled(true);
	fFeedListView->SetSortColumn(fDateColumn, true, false);
		// Sort by date column, compound sort, descending
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(_CreateMenuBar())
		.Add(fFeedListView);
}


BMenuBar*
MainWindow::_CreateMenuBar(void)
{
	BMenuItem* item;

	// Feeds Menu
	BMenu* feedsMenu = new BMenu("Feeds");

	item = new BMenuItem("Edit" B_UTF8_ELLIPSIS, new BMessage(MSG_SETTINGS));
	item->SetTarget(be_app);
	feedsMenu->AddItem(item);
	feedsMenu->AddItem(new BMenuItem("Update Now", new BMessage(MSG_UPDATE)));
	feedsMenu->AddItem(new BMenuItem("Mark All As Read", new BMessage(MSG_MARK_READ)));
	feedsMenu->AddSeparatorItem();
	item = new BMenuItem("About" B_UTF8_ELLIPSIS, new BMessage(B_ABOUT_REQUESTED));
	item->SetTarget(be_app);
	feedsMenu->AddItem(item);

	BMenuBar* menuBar = new BMenuBar("MenuBar");
	menuBar->AddItem(feedsMenu);

	return menuBar;
}


void
MainWindow::_UpdateFeedList()
{
	// Clear the old entries
	fFeedListView->Clear();
	//fFeedListView->SetFont(be_bold_font);
	
	Settings* settings = ((NibbleNews*)be_app)->GetSettings();
	
	// For each feed...
	for (int32 i = 0; i < settings->fFeeds.CountItems(); i++) {
		
		Feed* aFeed = settings->fFeeds.ItemAt(i);
		
		// For each item...
		for (int32 j = 0; j < aFeed->fItems.CountItems(); j++) {
			
			Item* anItem = aFeed->fItems.ItemAt(j);
			
			ItemRow* row = new ItemRow(anItem);
			
			if (!anItem->fRead)
				row->SetField(new BStringField("*"), COLUMN_INDEX_NEW);
			else
				row->SetField(new BStringField(" "), COLUMN_INDEX_NEW);
				
			//row->SetField(new BStringField(""), COLUMN_INDEX_ICON);
			row->SetField(new BStringField(anItem->fTitle), COLUMN_INDEX_NAME);
			
			// Convert the date string to type time_t
			struct tm datetime;
			strptime(anItem->fDate, "%a, %d %b %Y %H:%M:%S %z", &datetime);
			time_t t = mktime(&datetime);
			row->SetField(new BDateField(&t), COLUMN_INDEX_DATE);
			
			row->SetField(new BStringField(aFeed->fTitle), COLUMN_INDEX_FEED);
			
			// Add the row
			fFeedListView->AddRow(row);
		}
	}
}


#define BROWSER_MIME_WEBPOSITIVE "application/x-vnd.Haiku-WebPositive"


void open_link(BString link)
{
	char* argv[1];
	argv[0] = (char*)link.String();
	
	status_t status = be_roster->Launch(BROWSER_MIME_WEBPOSITIVE, 1, argv);
	if (status == B_OK || status == B_ALREADY_RUNNING) {
		// Mark as read!
	} else {
		std::cout << "Failed to load WebPositive." << std::endl;
	}
}

	
void
MainWindow::MessageReceived(BMessage* msg)
{
	BRow* selection = 0;
	
	switch (msg->what)
	{
		case MSG_FEEDS_CHANGED:
			// The list of feeds and news has changed.
			// Update the list.
			_UpdateFeedList();
			break;
		case MSG_OPEN_ITEM_LINK:
			while ((selection = fFeedListView->CurrentSelection(selection))) {
				Item* anItem = ((ItemRow*)selection)->fItem;
				open_link(anItem->fLink);
				anItem->fRead = true;
				this->PostMessage(MSG_FEEDS_CHANGED);
			}
			break;
		default:
			BWindow::MessageReceived(msg);
			break;
	}
}


bool
MainWindow::QuitRequested(void)
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
