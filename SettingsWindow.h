#ifndef FEEDSWINDOW_H
#define FEEDSWINDOW_H

#include <ListView.h>
#include <Window.h>


class SettingsWindow : public BWindow
{
public:
						SettingsWindow(void);
			void		MessageReceived(BMessage* msg);
			bool		QuitRequested();

			// Message Handlers
			void		OnAddFeed();
			void		OnDeleteFeed();
			void		OnEditFeed();

private:
			BListView* 	fFeedListView;
};


#endif
