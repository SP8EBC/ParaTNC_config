/*
 * MainGui.cpp
 *
 *  Created on: Oct 8, 2026
 *      Author: mateusz
 */

#include "assert.h"
#include <gui_ncurses/MainGui.h>
#include <string.h>


MainGui::MainGui () : m_window (NULL)
{
	m_window = initscr ();
	assert (m_window != NULL);
	keypad (m_window, TRUE);
	start_color ();
}

void MainGui::printCentered (WINDOW *win, int y, const char *s)
{
    int w = getmaxx(win);
    int x = (w - (int)strlen(s)) / 2;
    if (x < 0) x = 0;               /* string wider than window */
    mvwprintw(win, y, x, "%s", s);
}

MainGui::~MainGui ()
{
	endwin ();
}

void MainGui::startGui ()
{
	init_pair (1, COLOR_RED, COLOR_GREEN);
	init_pair (2, COLOR_WHITE, COLOR_BLUE);

	move (10, 10);

	WINDOW *my_win;
	int startx, starty, width, height;

	cbreak (); /* disable line buffering and
				* editing control characters   */
	wbkgd (m_window, COLOR_PAIR (2));

	height = 10;
	width = 50;
	starty = (LINES - height) / 2; /* Calculating for a center placement */
	startx = (COLS - width) / 2;   /* of the window                */
	refresh ();
	my_win = newwin (height, width, starty, startx);

	// let's define first color pair as such:
	//	init_pair(1, COLOR_RED, COLOR_GREEN);
	// this will make a text and border RED, background for border and text green
	// but the rest will remain black, or in another way the box has a transparent background
	// wbkgdset (my_win, COLOR_PAIR (1));

	/*
	 * wbkgdset() only sets the background property for characters written later. The window's
existing cells (all blanks after newwin) keep no color, so only what you draw (border, text) gets
the color pair. wbkgd() sets the background property and applies it to every cell already in the
window, so the whole box fills with green.
	 */
	wbkgd (my_win, COLOR_PAIR (1));

	box (my_win, 0, 0); /* 0, 0 gives default characters
						 * for the vertical and horizontal
						 * lines                        */

	PANEL* my_panel = new_panel(my_win);

	update_panels();
	doupdate();

	wrefresh (my_win);	/* Show that box                */
	refresh ();

	MainGui::printCentered (my_win, 2, "ParaMETEO configuration tool.");
	wrefresh (my_win); /* Show that box                */
	refresh ();
	getch ();

	werase (my_win);
	wrefresh (my_win);
	del_panel(my_panel);
	delwin (my_win);
	refresh ();
	getch ();
}
