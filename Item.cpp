#include <iostream>

#include "Item.h"


Item::Item(BString title, BString link, BString date)
{
	fTitle.SetTo(title);
	fLink.SetTo(link);
	fDate.SetTo(date);
	fRead = false;
	fVisible = true;
}


void
Item::SetRead(bool read)
{
	fRead = read;
}


bool
Item::Equals(Item* other)
{
	if (fTitle.Compare(other->fTitle) != 0)
		return false;
	if (fLink.Compare(other->fLink) != 0)
		return false;
	if (fDate.Compare(other->fDate) != 0)
		return false;

	// All fields are equal
	return true;
}


void
Item::Print()
{
	std::cout << "Item: " << fTitle << std::endl;
	std::cout << "      " << fLink << std::endl;
	std::cout << "      " << fDate << std::endl;
}
