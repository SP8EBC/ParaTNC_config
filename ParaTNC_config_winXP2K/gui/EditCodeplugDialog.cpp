#include "stdafx.h"
#include "EditCodeplugDialog.h"
#include <iostream>
#include <iomanip>

#include "commctrl.h"
#include "Resource.h"

WNDPROC		lpfnEditCodeplugSsidOldProc = NULL;	// original Window Proc function for SSID edit

HWND		hEditCodeplugDlg = NULL;	// handle to this dialog window

#define SSID_MIN		0
#define SSID_MAX		15

LRESULT CALLBACK	EditCodeplugDialogSsidProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam /*, 
											   UINT_PTR uIdSubclass, DWORD_PTR dwRefData*/)
{
	LRESULT result = -1;
	int number = -1;

	// buffer to hold new keypress as an array od characters
	CHAR typed[2] = {(char) wParam, 0x00};

	// process only keystrokes with characters
    if(uMsg == WM_CHAR)
    {
		// special case for backspace
		if (wParam == VK_BACK)
		{
			// allow backspace to go through
			result = CallWindowProc(lpfnEditCodeplugSsidOldProc, hWnd, uMsg, wParam, lParam); 
		}
		else 
		{
			// process and allow only digits to be input
			if (isdigit(wParam))
			{
				// get an integer from the edit content BEFORE this edit
				number = GetDlgItemInt(hEditCodeplugDlg, IDC_EC_EDIT2, NULL, FALSE);
				
				// do not allow three and more digits numbers
				if (number >= 10)
				{
					;
				}
				else
				{
					// if there is only one digit in the edit control, check if this one and 
					// the new one do not extend over limit
					if ( number == 1)
					{
						// convert new character into digit
						int _new = atoi(typed);

						// check if new SSID will fit within limit
						if (10 + _new <= SSID_MAX)
						{
							// if it is less or equal upper limit process message like normal
							result = CallWindowProc(lpfnEditCodeplugSsidOldProc, hWnd, uMsg, wParam, lParam); 
						}
						else 
						{
							;	// if not ignore it
						}
					}
					else
					{
						result = CallWindowProc(lpfnEditCodeplugSsidOldProc, hWnd, uMsg, wParam, lParam); 
					}
					
				}

				std::cout << "D = EditCodeplugDialogSsidProc, wParam: " << number << ", GetLastError: " << GetLastError() << std::endl;
			}
			else
			{
				;
			}
		}
    }
	else
	{
		result = CallWindowProc(lpfnEditCodeplugSsidOldProc, hWnd, uMsg, wParam, lParam); 
	}

	return result;
}

INT_PTR CALLBACK	EditCodeplugDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	std::cout << "D = EditCodeplugDialog, message: 0x" << std::hex << message << std::dec << std::endl;

	HWND		hEditSsid	= NULL;


	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		hEditCodeplugDlg = hDlg;
		// get a handle to edit with SSID
		hEditSsid = GetDlgItem(hDlg, IDC_EC_EDIT2);

		lpfnEditCodeplugSsidOldProc = (WNDPROC)SetWindowLongPtr(hEditSsid, GWLP_WNDPROC, (LONG_PTR)EditCodeplugDialogSsidProc);
		//SetWindowSubclass(hEditSsid, EditCodeplugDialogSsidProc, 0, 0);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	case WM_DESTROY:
		SetWindowLongPtr(hEditSsid, GWLP_WNDPROC, (LONG_PTR)lpfnEditCodeplugSsidOldProc);
		break;
	}

	return (INT_PTR)FALSE;
}
