/*
 * NcursesWindow.cpp
 *
 *  Created on: Oct 11, 2026
 *      Author: mateusz
 */

#include "Window.h"
#include <assert.h>

namespace ncurses {

Window::Window (const std::string &title, const bool titleSeparator, const int color,
				const int starty, const int startx, const int height, const int width)
	: m_windowTitle (title), m_hasTitleSeparator (titleSeparator), m_width (width),
	  m_height (height)

{
	m_startX = startx;
	m_startY = starty;

	m_window = newwin (m_height, m_width, m_startY, m_startX);
	assert (m_window != NULL);

	// set a background for the whole window.
	// title bar and the rest will have the same color
	wbkgd (m_window, color);

	int _height, _width;

	getmaxyx (m_window, _height, _width);
	(void)_height;

	box (m_window, 0, 0);

	if (titleSeparator) {
		// these three lines of code prints horizontal line
		// separating a window title from the window content
		mvwaddch (m_window, 2, 0, ACS_LTEE);			 // puts '├─' at the left side
		mvwhline (m_window, 2, 1, ACS_HLINE, width - 2); // draws the line itself
		mvwaddch (m_window, 2, m_width - 1, ACS_RTEE);	 // puts '─┤' at the right side
	}

	const size_t length = m_windowTitle.length ();
	const float temp = (_width - length) / 2;
	const int _x = (int)temp;

	// prints the window title, starting from the first line in the window (zero is the border).
	// the text 'label' will be centered, by using value '_x' calculated from text lenght
	mvwprintw (m_window, 1, _x, "%s", m_windowTitle.c_str ());

	wrefresh (m_window);
	refresh ();
}

Window::~Window ()
{
	werase (m_window);
	wrefresh (m_window);
	delwin (m_window);
	refresh ();
}

WINDOW *Window::operator() ()
{
	wrefresh (m_window);
	refresh ();

	return m_window;
}

WINDOW *Window::operator* ()
{
	return m_window;
}

void Window::printCentered (const std::string &text, const int y)
{
	int w = getmaxx (m_window);
	int x = (w - text.size ()) / 2;
	if (x < 0)
		x = 0; /* string wider than window */
	mvwprintw (m_window, y, x, "%s", text.c_str ());
	wrefresh (m_window); /* Show that box                */
	refresh ();
}

void Window::move (int newY, int newX)
{
	werase (m_window);
	wrefresh (m_window);
	mvwin (m_window, newY, newX);
	box (m_window, 0, 0);
	wrefresh(m_window);
	refresh ();
}


} /* namespace ncurses */
