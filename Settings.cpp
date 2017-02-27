#include <iostream>

#include "Settings.h"


Settings::Settings()
	:	fMainWindowFrame(),
		fColumnWidthName(300),
		fColumnWidthDate(150),
		fColumnWidthFeed(150)
{
	// Empty
}


bool
Settings::Lock(void)
{
	return fLock.Lock();
}


void
Settings::Unlock(void)
{
	fLock.Unlock();
}

