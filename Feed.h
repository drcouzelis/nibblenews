#ifndef FEED_H
#define FEED_H

#include <ObjectList.h>
#include <String.h>

#include "Item.h"


class Feed
{
public:

	class Listener
	{
	public:
		virtual void FeedUpdated(Feed* feed)=0;
		virtual void FeedHasNewItems(Feed* feed, int amount)=0;
	};
	
	Feed(BString location);
	~Feed();
	
	void SetUserTitle(BString title);
		// Overrides the feed title
	void UnsetUserTitle();
		// Use the default title for the feed
		
	void Print();
		// Print some debugging info
	
private:
	void _NotifyFeedUpdated();
	void _NotifyFeedHasNewItems(int amount);
	
public: // TEMP
	BString fLocation;
	BString fTitle;
	BString fLink;
	
	BString fUserTitle;
	bool fUseUserTitle;

	bool fFailed;
		// The feed failed to load
	
public:
	BObjectList<Item> fItems;
		// The "updates" for this feed, a list of "Item" objects
};


#endif
