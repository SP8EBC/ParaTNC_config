/*
 * Panel.cpp
 *
 *  Created on: Oct 11, 2026
 *      Author: mateusz
 */

#include <gui_ncurses/ncmp/Panel.h>

namespace ncurses {
Panel *Panel::m_Top;

Panel *Panel::m_Bottom;

Panel::Panel (Window &window, Panel *below)
	: m_window (window), m_panelBelow (below), m_panelAbove (NULL)
{
	m_panel = new_panel (*window);

	// if there is panel below
	if (m_panelBelow) {
		set_panel_userptr (m_panel, (void *)m_panelBelow);
	}
	else {
		set_panel_userptr (m_panel, NULL);
	}

	update_panels ();
	doupdate ();
}

Panel::~Panel ()
{
	del_panel (m_panel);
	update_panels ();
	doupdate ();
}

Panel *Panel::showBottom ()
{
	if (m_panelBelow) {
		top_panel (m_panelBelow->m_panel);
		update_panels ();
		doupdate ();

		return m_panelBelow;
	}
	else if (Panel::m_Top) {
		// if this is the most bottom panel, and there is something set
		// as the top panel -> move to the top panel
		top_panel (Panel::m_Top->m_panel);
		update_panels ();
		doupdate ();

		return Panel::m_Top;
	}
	else {
		return NULL;
	}
}

Panel *Panel::showTop ()
{
	if (m_panelAbove) {
		top_panel (m_panelAbove->m_panel);
		update_panels ();
		doupdate ();

		return m_panelAbove;
	}
	else if (Panel::m_Bottom) {
		top_panel (Panel::m_Bottom->m_panel);
		update_panels ();
		doupdate ();

		return Panel::m_Bottom;
	}
	else {
		return NULL;
	}
}

} /* namespace ncurses */
