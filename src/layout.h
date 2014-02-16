#ifndef QJOYPAD_LAYOUT_H
#define QJOYPAD_LAYOUT_H


//to allow for interprocess communications (ie, signaling a running instance of
//qjoypad by running "qjoypad layout-name", etc.) QJoyPad uses signals to
//triger certain events. This is for signaling the main program to update
//the joystick device list.
#include <signal.h>

#include <QAction>
#include <QDir>
#include <QMenu>
#include <QApplication>
#include <QDialog>
#include <QPointer>
#include <QInputDialog>
#include <QSystemTrayIcon>

//a layout handles several joypads
#include "joypad.h"
//for errors
#include "error.h"
//For displaying a floating icon instead of a tray icon
#include "icon.h"
//So we can know if there is a graphical version of the Layout Manager displayed
#include "layout_edit.h"

//for recognizing when the special empty layout is in use
#define NL "[NO LAYOUT]"

//handles loading, saving, and changing of layouts
class LayoutManager : public QObject {
	friend class LayoutEdit;
	Q_OBJECT
	public:
        LayoutManager(bool useTrayIcon, const QString &devdir, const QString &settingsDir);
        ~LayoutManager();

		//produces a list of the names of all the available layout.
        QStringList getLayoutNames() const;
	public slots:
		//load a layout with a given name
		bool load(const QString& name);
		//look for the last loaded layout and try to load that.
		bool load();
		//load the current layout, overwriting any changes made to it.
		bool reload();
		//reset to a blank, empty layout
		void clear();
		
		//save the current layout with its current name
		void save();
		//save the current layout with a new name
		void saveAs();
		//save the currently loaded layout so it can be recalled later
		void saveDefault();

		//get rid of a layout
		void remove();
		
		//when the tray icon is clicked
		void iconClick();
        void trayClick(QSystemTrayIcon::ActivationReason reason);
		//rebuild the popup menu with the current information
		void fillPopup();
		//update the list of available joystick devices
		void updateJoyDevs();
    private slots:
        //when the user selects an item on the tray's popup menu
        void layoutTriggered();
	private:
		//change to the given layout name and make all the necesary adjustments
		void setLayoutName(QString name);
		//get the file name for a layout name
		QString getFileName( QString layoutname );
        //the directory in wich the joystick devices are (e.g. "/dev/input")
        QString devdir;
        QString settingsDir;
		//the layout that is currently in use
        QString currentLayout;
		//the popup menu from the tray/floating icon
        QMenu trayMenu;
        //known actions for the popup menu
        QActionGroup *layoutGroup;
        QAction *titleAction;
        QAction *updateDevicesAction;
        QAction *updateLayoutsAction;
        QAction *quitAction;

		//if there is a LayoutEdit open, this points to it. Otherwise, NULL.	
        QPointer<LayoutEdit> le;

        QHash<int, JoyPad*> available;
        QHash<int, JoyPad*> joypads;
};

#endif
