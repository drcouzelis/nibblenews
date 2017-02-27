#ifndef CONSTANTS_H
#define CONSTANTS_H


#include <SupportDefs.h>


// BMessages
enum
{
	MSG_SETTINGS = 1000,
	MSG_UPDATE,
	MSG_MARK_READ,
	MSG_VIEW_UNREAD_ONLY,
	MSG_VIEW_NEW,
	MSG_VIEW_ICON,
	MSG_VIEW_DATE,
	MSG_VIEW_FEED,
	MSG_FEEDS_CHANGED,
	MSG_TIME_TO_UPDATE_FEEDS,
	MSG_OPEN_ITEM_LINK
};

// Strings
extern const char* STR_FEEDS;
extern const char* STR_EDIT;
extern const char* STR_UPDATE_NOW;
extern const char* STR_MARK_ALL_READ;
extern const char* STR_ABOUT;
extern const char* STR_QUIT;

#endif

