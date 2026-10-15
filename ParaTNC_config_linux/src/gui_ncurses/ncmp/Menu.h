/*
 * Menu.h
 *
 *  Created on: Oct 11, 2026
 *      Author: mateusz
 */

#ifndef SRC_GUI_NCURSES_NCMP_MENU_H_
#define SRC_GUI_NCURSES_NCMP_MENU_H_

#include <curses.h>
#include <menu.h>

#include <memory>
#include <string>
#include <vector>

#include "Window.h"

namespace ncurses {

/**
 * Base class, holding a context of single menu entry. This is only an interface and must
 * be implemented by each entry
 */
class MenuItem {
	/**
	 * Displayed name
	 */
	const std::string m_name;

	/**
	 * Displayed description
	 */
	const std::string m_description;

	/**
	 * This pointer will be initialized by a call to "new_item".
	 * It is Ncurses specific type
	 */
	ITEM *m_item;

  public:
	MenuItem (std::string name, std::string descr);

	/**
	 * @note MenuItem can't be copied. This would free the same item twice. It owns a raw ITEM* and
	 * frees it in its destructor. Copying one leaves two objects that both call free_item on the
	 * same pointer. Deleting the copy operations makes that a compile error instead:
	 */
	MenuItem (const MenuItem &) = delete;

	/**
	 * @note MenuItem can't be copied. This would free the same item twice. It owns a raw ITEM* and
	 * frees it in its destructor.  Copying one leaves two objects that both call free_item on the
	 * same pointer. Deleting the copy operations makes that a compile error instead:
	 */
	MenuItem &operator= (const MenuItem &) = delete;

	/**
	 * Simply a destructor
	 */
	virtual ~MenuItem ();

	/**
	 * Callback, which must be implemented by child class, will be used when
	 * a user press enter on a selected menu entry. Basically onSelect callback
	 */
	virtual void onSelect () = 0;

	ITEM *getItemPointer () const { return m_item; }
};

class Menu {

	/**
	 * Ncurses specific menu instance
	 */
	MENU *m_menu;

	/**
	 * Subwindow derived from `m_window`, where menu items are displayed
	 */
	WINDOW *m_subWindow;

	/**
	 * Ncurses window to display this menu
	 */
	Window m_window;

	/**
	 * Menu items owned by this menu, each one providing its own onSelect callback
	 */
	std::vector<std::unique_ptr<MenuItem>> &m_menuItems;

	/**
	 * Vector of pointers to menu items. required to be passed to `new_menu`
	 */
	std::vector<ITEM *> m_itemsPointer;

  public:
	Menu (std::vector<std::unique_ptr<MenuItem>> &menuItems, const std::string &title,
		  const int color, const int starty, const int startx, const int height, const int width);
	virtual ~Menu ();

	/**
	 * Shows menu, waits for a user to make a selection and then calls a callback
	 */
	void showMenu ();
};

} /* namespace ncurses */

#endif /* SRC_GUI_NCURSES_NCMP_MENU_H_ */
