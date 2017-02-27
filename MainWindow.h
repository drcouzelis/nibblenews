#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Window.h>
#include <MenuBar.h>
#include <MenuField.h>
#include <MenuItem.h>

// A copy of private Haiku headers
#include "ColumnListView.h"
#include "ColumnTypes.h"

#include "Constants.h"


class MainWindow : public BWindow
{
public:
						MainWindow(BRect frame);
			void		MessageReceived(BMessage *msg);
			bool		QuitRequested(void);

private:
			BMenuBar*	_CreateMenuBar(void);
			void		_UpdateFeedList(void);
			
private:
			BColumnListView* fFeedListView;
			BStringColumn* fNewColumn;
			BStringColumn* fIconColumn;
			BStringColumn* fNameColumn;
			BDateColumn* fDateColumn;
			BStringColumn* fFeedColumn;
};

#endif
