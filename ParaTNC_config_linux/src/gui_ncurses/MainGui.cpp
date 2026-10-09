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

MainGui::~MainGui ()
{
	endwin ();
}

void MainGui::printCentered (WINDOW *win, int y, const char *s)
{
	int w = getmaxx (win);
	int x = (w - (int)strlen (s)) / 2;
	if (x < 0)
		x = 0; /* string wider than window */
	mvwprintw (win, y, x, "%s", s);
	wrefresh (win); /* Show that box                */
	refresh ();
}

WINDOW *MainGui::createWindow (const char *title, bool titleSeparator, const int color,
							   const int starty, const int startx, const int height,
							   const int width)
{
	int _height, _width;
	WINDOW *win = newwin (height, width, starty, startx);

	assert (win != NULL);

	// set a background for the whole window.
	// title bar and the rest will have the same color
	wbkgd (win, color);

	getmaxyx (win, _height, _width);
	(void)_height;

	box (win, 0, 0);

	if (titleSeparator) {
		// these three lines of code prints horizontal line
		// separating a window title from the window content
		mvwaddch (win, 2, 0, ACS_LTEE);				// puts '├─' at the left side
		mvwhline (win, 2, 1, ACS_HLINE, width - 2); // draws the line itself
		mvwaddch (win, 2, width - 1, ACS_RTEE);		// puts '─┤' at the right side
	}

	const size_t length = (int)strlen (title);
	const float temp = (_width - length) / 2;
	const int _x = (int)temp;

	// prints the window title, starting from the first line in the window (zero is the border).
	// the text 'label' will be centered, by using value '_x' calculated from text lenght
	mvwprintw (win, 1, _x, "%s", title);

	wrefresh (win);
	refresh ();

	return win;
}

void MainGui::startGui ()
{
	init_pair (1, COLOR_RED, COLOR_GREEN);
	init_pair (2, COLOR_WHITE, COLOR_BLUE);

	move (10, 10);

	int startx, starty, width, height;

	cbreak ();						  /* disable line buffering and
									   * editing control characters   */
	wbkgd (m_window, COLOR_PAIR (2)); // global background color
	refresh ();

	height = 10;
	width = 50;
	starty = (LINES - height) / 2; /* Calculating for a center placement */
	startx = (COLS - width) / 2;   /* of the window                */

	WINDOW *my_win =
		MainGui::createWindow (" ", false, COLOR_PAIR (1), starty, startx, height, width);

	MainGui::printCentered (my_win, 3, "ParaMETEO configuration tool.");

	getch ();

	WINDOW *second_win = MainGui::createWindow ("etykieta",
												true,
												COLOR_PAIR (1),
												starty - 20,
												startx + 20,
												height,
												width);

	werase (my_win);
	wrefresh (my_win);
	delwin (my_win);
	refresh ();
	getch ();
	werase (second_win);
	wrefresh (second_win);
	refresh ();
	getch ();
}
