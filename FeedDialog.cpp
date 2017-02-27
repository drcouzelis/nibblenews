#include "FeedDialog.h"

#include <Button.h>
#include <GroupLayout.h>


//enum
//{
//};


/*
FeedDialog::FeedDialog(void)
	:	BWindow(BRect(510, 100, 510 + 500, 400), "Feeds",
		B_MODAL_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS)
{
	BButton* button;

	BGroupLayout* layout = new BGroupLayout(B_VERTICAL);
	SetLayout(layout);
	
	// Feeds List
	_feedListView = new BListView(BRect(), "List");
	layout->AddView(_feedListView);

	// Control Buttons
	BGroupLayout* buttonLayout = new BGroupLayout(B_HORIZONTAL);
	button = new BButton(BRect(), "AddButton", "Add...", new BMessage(M_FEED_ADD));
	buttonLayout->AddView(button);
	button = new BButton(BRect(), "DeleteButton", "Delete", new BMessage(M_FEED_DELETE));
	buttonLayout->AddView(button);
	button = new BButton(BRect(), "EditButton", "Edit...", new BMessage(M_FEED_EDIT));
	buttonLayout->AddView(button);
	layout->AddItem(buttonLayout);

	// Close Button
	button = new BButton(BRect(), "CloseButton", "Close", new BMessage(B_QUIT_REQUESTED));
	layout->AddView(button);
}


void
FeedDialog::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case M_FEED_ADD:
			OnAddFeed();
			break;
		case M_FEED_DELETE:
			OnDeleteFeed();
			break;
		case M_FEED_EDIT:
			OnEditFeed();
			break;
		default:
			BWindow::MessageReceived(msg);
			break;
	}
}


bool
FeedDialog::QuitRequested()
{
	Hide();
	return false;
}


void
FeedDialog::OnAddFeed()
{
	//FeedEntryWindow window;
}


void
FeedDialog::OnDeleteFeed()
{
}


void
FeedDialog::OnEditFeed()
{
}
*/
