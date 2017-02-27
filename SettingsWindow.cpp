#include "SettingsWindow.h"

#include <Button.h>
#include <GroupLayout.h>
#include <LayoutBuilder.h>


enum
{
	M_FEED_ADD = 'fead',
	M_FEED_DELETE = 'fede',
	M_FEED_EDIT = 'feed'
};


SettingsWindow::SettingsWindow(void)
	:	BWindow(BRect(510, 100, 510 + 500, 400), "Settings", B_TITLED_WINDOW,
			B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS)
{
	// Settings List
	fFeedListView = new BListView("List");

	// Control Buttons
	BButton* addButton = new BButton("AddButton", "Add...", new BMessage(M_FEED_ADD));
	BButton* deleteButton = new BButton("DeleteButton", "Remove", new BMessage(M_FEED_DELETE));
	BButton* editButton = new BButton("EditButton", "Edit...", new BMessage(M_FEED_EDIT));

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.AddGroup(B_HORIZONTAL, 0)
			.Add(fFeedListView)
			.AddGroup(B_VERTICAL, 10)
				.SetInsets(10, 10, 10, 10)
				.Add(addButton)
				.Add(editButton)
				.Add(deleteButton)
				.AddGlue();
}


void
SettingsWindow::MessageReceived(BMessage *msg)
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
SettingsWindow::QuitRequested()
{
	Hide();
	return false;
}


void
SettingsWindow::OnAddFeed()
{
}


void
SettingsWindow::OnDeleteFeed()
{
}


void
SettingsWindow::OnEditFeed()
{
}



