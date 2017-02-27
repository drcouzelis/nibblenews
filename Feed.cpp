#include <iostream>

#include "Feed.h"
#include "Item.h"


Feed::Feed(BString location)
{
	fLocation.SetTo(location);
	fUseUserTitle = false;
	fFailed = false;
}


Feed::~Feed()
{
	for (int32 i = 0; Item* anItem = fItems.ItemAt(i); i++)
		delete anItem;
}


void
Feed::SetUserTitle(BString title)
{
	fUserTitle.SetTo(title);
	fUseUserTitle = true;
}


void
Feed::UnsetUserTitle()
{
	fUseUserTitle = false;
}


void
Feed::Print()
{
	std::cout << "Feed: ";
	
	if (fUseUserTitle)
		std::cout << fUserTitle << std::endl;
	else
		std::cout << fTitle << std::endl;
		
	std::cout << "      " << fLink << std::endl;
	
	// Print the items
	for (int32 i = 0; Item* anItem = fItems.ItemAt(i); i++ )
		anItem->Print();
}
