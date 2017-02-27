#ifndef FEEDDIALOG_H
#define FEEDDIALOG_H

#include <Window.h>


class FeedDialog : public BWindow
{
public:
						FeedDialog(void);
			void		MessageReceived(BMessage* msg);
			bool		QuitRequested();

private:
			//BListView* 	_feedListView;
};


#endif
