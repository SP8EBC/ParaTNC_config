/*
 * MainGui.h
 *
 *  Created on: Oct 8, 2026
 *      Author: mateusz
 */

#ifndef SRC_GUI_NCURSES_MAINGUI_H_
#define SRC_GUI_NCURSES_MAINGUI_H_

#include <curses.h>
#include <panel.h>
#include <menu.h>

class MainGui {

private:
	/**
	 * Pointer to the screen. The main object NCurses draws onto
	 */
	WINDOW *m_window;

	/**
	 *
	 * @param win
	 * @param y
	 * @param s
	 */
	static void printCentered(WINDOW *win, int y, const char *s);

  public:
	/**
	 * Entry point to the GUI.
	 */
	void startGui ();

	MainGui ();
	virtual ~MainGui ();
};

#endif /* SRC_GUI_NCURSES_MAINGUI_H_ */
