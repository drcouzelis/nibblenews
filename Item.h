#ifndef ITEM_H
#define ITEM_H

#include <String.h>


class Item
{
public:
	Item(BString title, BString link, BString Date);

	void SetRead(bool read);
		// Mark the item as read or unread

	bool Equals(Item* other);
		// Compares two items, returns true if they are equivalent
	
	void Print();
		// Print some debugging info
		
public: // TEMP
	BString fTitle;
	BString fLink;
	BString fDate;

	bool fRead;
	bool fVisible;
};


#endif
