/*
 * NcursesWindow.h
 *
 *  Created on: Oct 11, 2026
 *      Author: mateusz
 */

#ifndef SRC_GUI_NCURSES_WINDOW_H_
#define SRC_GUI_NCURSES_WINDOW_H_

#include <curses.h>
#include <string>

namespace ncurses {

class Window {
  private:
	/**
	 * Pointer to the screen. The main object NCurses draws onto
	 */
	WINDOW *m_window;

	std::string m_windowTitle;

	bool m_hasTitleSeparator;

	const int m_width;

	const int m_height;

	int m_startY;

	int m_startX;

  public:
	Window (const std::string &title, const bool titleSeparator, const int color, const int starty,
			const int startx, const int height, const int width);
	virtual ~Window ();

	WINDOW *operator() ();

	WINDOW *operator* ();

	void printCentered (const std::string &text, const int y);

	/**
	 * Moves a window from one place to another, although any context
	 * inside this window (like a result of 'printCentered' is gone at the moment
	 * @param newY
	 * @param newX
	 */
	void move (int newY, int newX);
};

} /* namespace ncurses */

#endif /* SRC_GUI_NCURSES_WINDOW_H_ */
