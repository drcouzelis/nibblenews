#ifndef NIBBLENEWS_H
#define NIBBLENEWS_H

#include <Application.h>
#include <MessageRunner.h>

//#include "FeedDialog.h"
#include "MainWindow.h"
#include "Settings.h"
#include "SettingsWindow.h"


class NibbleNews : public BApplication
{
public:
	NibbleNews(void);
	virtual void ReadyToRun();
	virtual bool QuitRequested();
	virtual void AboutRequested();
	virtual void MessageReceived(BMessage* msg);
	virtual void ArgvReceived(int32 argc, char** argv);
	
	void LoadSettings();
					// Load settings from the default config file
	void SaveSettings();
					// Save the application settings
					
	Settings* GetSettings(void);
					// The current application settings
					
	void LoadDummyFeeds();
					// TEMP

private:
	Settings fSettings;

	BMessageRunner* fUpdateRunner;
	
	MainWindow* fMainWindow;
	SettingsWindow* fSettingsWindow;
	//FeedDialog* fFeedDialog;
};


#endif
