/*
 * Panel.h
 *
 *  Created on: Oct 11, 2026
 *      Author: mateusz
 */

#ifndef SRC_GUI_NCURSES_NCMP_PANEL_H_
#define SRC_GUI_NCURSES_NCMP_PANEL_H_

#include "Window.h"

#include <panel.h>

namespace ncurses {

class Panel {

	/**
	 * Instance of a window wrapped by this panel
	 */
	Window &m_window;

	/**
	 * Pointer to a panel which is located just below of this panel on the stack
	 */
	Panel *m_panelBelow;

	/**
	 * Pointer to a panel which is located above, it is set by a getter, after the panel
	 * in question is created (by convene they're created from bottom to top - in the similar
	 * way in which a pieces of paper are laid on the table)
	 */
	Panel *m_panelAbove;

	/**
	 * Ncurses Panel library object
	 */
	PANEL *m_panel;

	/**
	 * This pointer is used only if panel sequencing should form a ring buffer,
	 * i.e. if after the most bottom panel is reached, this one should be print to the top
	 */
	static Panel *m_Top;

	static Panel *m_Bottom;
  public:
	/**
	 * Constructor which will create a panel over a provided instance of a Window.
	 * The idea is that a sequence of windows / panels is created from the bottom one,
	 * to the top one, what seems to be a natural order
	 * @param window
	 * @param below a panel which was created previously and which is located below this one, or
	 * NULL if this is first panel
	 */
	Panel (Window &window, Panel *below);
	virtual ~Panel ();

	/**
	 * Shows a panel just below this one
	 */
	Panel * showBottom();

	/**
	 * Shows a panel just above this one
	 */
	Panel * showTop();

	void setPanelAbove (Panel *nPanelAbove) { m_panelAbove = nPanelAbove; }

	void setMBottom ( Panel *mBottom) { Panel::m_Bottom = mBottom; }

	void setMTop ( Panel *mTop) { Panel::m_Top = mTop; }
};

} /* namespace ncurses */

#endif /* SRC_GUI_NCURSES_NCMP_PANEL_H_ */
