/*
 * Menu.cpp
 *
 *  Created on: Oct 11, 2026
 *      Author: mateusz
 */

#include "assert.h"
#include <gui_ncurses/ncmp/Menu.h>

#include <utility>

// https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/menus.html

namespace ncurses {

MenuItem::MenuItem (std::string name, std::string descr)
	: m_name (name), m_description (descr), m_item (NULL)
{
	m_item = new_item (m_name.c_str (), m_description.c_str ());
}

MenuItem::~MenuItem ()
{
	free_item (m_item);
}

Menu::Menu (std::vector<std::unique_ptr<MenuItem>> &menuItems, const std::string &title,
			const int color, const int starty, const int startx, const int height, const int width)
	: m_menu (NULL), m_subWindow (NULL), m_window (title, false, color, starty, startx, height, width),
	  m_menuItems (menuItems)
{
	for (std::unique_ptr<MenuItem> &item : m_menuItems) {
		// fill an array (continuous storage) of pointers to elements
		m_itemsPointer.push_back (item->getItemPointer ());
	}

	assert (m_itemsPointer.size () > 0);

	// function `new_menu` reads array until NULL terminator is found
	m_itemsPointer.push_back (NULL);

	m_menu = new_menu (m_itemsPointer.data ());
	assert (m_menu != NULL);
	// m_window = newwin (height, width, starty, startx);
	keypad (*m_window, TRUE);

	/* Set main window and sub window */

	// the subwindow is placed below the title (row 1) and the border, so it must leave
	// 3 rows at the top, 1 row at the bottom and 1 column on each side. derwin returns
	// NULL if the subwindow doesn't fit inside the parent window
	int win_height, win_width;
	getmaxyx (*m_window, win_height, win_width);

	const int derived_begin_y = 3;
	const int derived_begin_x = 1;
	const int derived_nlines = win_height - derived_begin_y - 1;
	const int derived_ncols = win_width - derived_begin_x - 1;

	constexpr int menu_format_rows = 5;
	constexpr int menu_format_cols = 1;

	/*
	 * Every menu has an associated pair of curses windows. The menu window displays any title and
	 * border associated with the window; the menu subwindow displays the items of the menu that are
	 * currently available for selection.
	 *
	 * Calling subwin creates and returns a pointer to a new window with the given number of lines,
	 * nlines, and columns, ncols. The window is at position (begin_y, begin_x) on the screen. The
	 * subwindow shares memory with the window orig, its ancestor, so that changes made to one
	 * window will affect both windows. When using this routine, it is necessary to call touchwin or
	 * touchline on orig before calling wrefresh on the subwindow.
	 *
	 * Calling derwin is the same as calling subwin, except that begin_y and begin_x are relative to
	 * the origin of the window orig rather than the screen. There is no difference between the
	 * subwindows and the derived windows.
	 */
	set_menu_win (m_menu, *m_window);
	m_subWindow = derwin (*m_window, derived_nlines, derived_ncols, derived_begin_y, derived_begin_x);
	assert (m_subWindow != NULL);

	set_menu_sub (m_menu, m_subWindow);

	/*
	 * The function set_menu_format sets the maximum display size of the given menu. If this size is
	 * too small to display all menu items, the menu will be made scrollable. If this size is larger
	 * than the menus subwindow and the subwindow is too small to display all menu items, post_menu
	 * will fail.
	 */
	set_menu_format (m_menu, menu_format_rows, menu_format_cols);
	set_menu_mark (m_menu, " * ");
}

Menu::~Menu ()
{
	unpost_menu (m_menu);
	free_menu (m_menu);
	delwin (m_subWindow);
}

void Menu::showMenu ()
{
	/* Post the menu */
	const int result = post_menu (m_menu);
	assert (result == E_OK);
	(void)result;
	wrefresh (*m_window);

	int keypress = 0;
	while (keypress != KEY_F (4)) {
		keypress = wgetch (*m_window);

		switch (keypress) {
		case KEY_DOWN: menu_driver (m_menu, REQ_DOWN_ITEM); break;
		case KEY_UP: menu_driver (m_menu, REQ_UP_ITEM); break;
		case KEY_NPAGE: menu_driver (m_menu, REQ_SCR_DPAGE); break;
		case KEY_PPAGE: menu_driver (m_menu, REQ_SCR_UPAGE); break;
		case 10: /* Enter */
			// The clrtoeol and wclrtoeol routines erase the current line to the right of the
			// cursor, inclusive, to the end of the current line.
			clrtoeol ();
			pos_menu_cursor (m_menu);
			break;
		}
		wrefresh (*m_window);
	}
}

} /* namespace ncurses */
