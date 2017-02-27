#ifndef FEEDLISTVIEW_H
#define FEEDLISTVIEW_H


#include "Item.h"

// A copy of private Haiku headers
#include "ColumnListView.h"


class ItemRow : public BRow
{
public:
	ItemRow(Item* item) { fItem = item; }
	
//private: // TEMP
	Item* fItem;
};


class FeedListView : public BColumnListView
{
public:
	FeedListView(const char* name, uint32 flags);
	
};

#endif
