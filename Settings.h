#ifndef SETTINGS_H
#define SETTINGS_H

#include <Locker.h>
#include <ObjectList.h>
#include <Rect.h>
#include <SupportDefs.h>

#include "Feed.h"


class Settings
{
public:
	Settings();

	BObjectList<Feed> fFeeds;
		// The list of feeds that the user wants to read

	BRect fMainWindowFrame;
		// The position and size of the main window

	int32 fColumnWidthName;
	int32 fColumnWidthDate;
	int32 fColumnWidthFeed;
		// The widths of the different columns

	time_t fLastUpdate;
		// The last time this feed was updated
	
	bool Lock(void);
	void Unlock(void);
	
private:		
	BLocker fLock;
		// Prevent multiple threads from trying to change settings at once
};


#endif
