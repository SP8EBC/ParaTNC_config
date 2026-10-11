/*
 * MainGui.cpp
 *
 *  Created on: Oct 8, 2026
 *      Author: mateusz
 */

#include "assert.h"
#include <gui_ncurses/MainGui.h>
#include <gui_ncurses/ncmp/Panel.h>
#include <gui_ncurses/ncmp/Window.h>
#include <memory>
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

	ncurses::Window my_win (" ", false, COLOR_PAIR (1), starty, startx, height, width);
	my_win.printCentered (std::string ("ParaMETEO configuration tool"), 3);
	ncurses::Panel my_panel (my_win, NULL);

	getch ();

	// my_win.move (12, 12);

	ncurses::Window
		second_win ("etykieta", true, COLOR_PAIR (1), starty - 3, startx + 3, height, width);
	ncurses::Panel second_panel (second_win, &my_panel);

	// now all panels are created, so it is possible to set a pointer to "next-towards-the-top"
	// panel on the bottom one
	my_panel.setPanelAbove (&second_panel);

	my_panel.setMTop(&second_panel);
	my_panel.setMBottom(&my_panel);

	ncurses::Panel * current = &second_panel;

	// disable an echo not to see 'z' and 'x' on the screen while navigating

	noecho();

	int keypress = 0;
	while (keypress != KEY_F(12))
	{
		keypress = getch();

		if (keypress == 'z')
		{
			current = current->showBottom();
		}
		else if (keypress == 'x')
		{
			current = current->showTop();
		}
		else
		{
			;	// do nothing
		}

	}

	getch();
}
