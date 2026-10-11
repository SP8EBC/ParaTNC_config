/*
 * MainGui.h
 *
 *  Created on: Oct 8, 2026
 *      Author: mateusz
 */

#ifndef SRC_GUI_NCURSES_MAINGUI_H_
#define SRC_GUI_NCURSES_MAINGUI_H_

#include <curses.h>
#include <menu.h>
#include <panel.h>

class MainGui {

  private:
	/**
	 * Pointer to the screen. The main object NCurses draws onto
	 */
	WINDOW *m_window;

  public:
	/**
	 * Entry point to the GUI.
	 */
	void startGui ();

	MainGui ();
	virtual ~MainGui ();
};

#endif /* SRC_GUI_NCURSES_MAINGUI_H_ */
