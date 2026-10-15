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

#include <string>
#include <vector>

namespace ncurses {

class MenuItem {
	std::string m_Name;

  public:
	virtual void onSelect () = 0;

	virtual ~MenuItem (){};
};

class Menu {

	std::vector<MenuItem> m_menuItems;

	MENU *m_menu;

	/**
	 * Each pointer to ITEM in this vector will be initialized by a call to "new_item".
	 * The size of this vector is the same as @link{m_menuItems}
	 */
	std::vector<ITEM *> m_rawMenuItems;

  public:
	Menu (std::vector<MenuItem> &menuItems);
	virtual ~Menu ();
};

} /* namespace ncurses */

#endif /* SRC_GUI_NCURSES_NCMP_MENU_H_ */
