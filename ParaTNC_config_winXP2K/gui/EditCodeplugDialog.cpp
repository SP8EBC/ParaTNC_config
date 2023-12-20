#include "stdafx.h"
#include "EditCodeplugDialog.h"
#include <iostream>
#include <iomanip>

#include "commctrl.h"
#include "Resource.h"

WNDPROC		lpfnEditCodeplugSsidOldProc = NULL;

LRESULT CALLBACK	EditCodeplugDialogSsidProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam /*, 
											   UINT_PTR uIdSubclass, DWORD_PTR dwRefData*/)
{
    if(uMsg == WM_CHAR)
    {
        if(0x8000 & GetKeyState(VK_CONTROL))
        {
            switch(wParam)
            {
                case 0x16: // ctrl-V
                {
                    //HandlePaste(hWnd);
                    return 0;
                }
                break;

                case 0x03: // ctrl-C
                case 0x18: // ctrl-X
                {
                    // allow through
                }
                break;

                default:
                {
                    return 0;
                }
            }
        }
        else
        {
            // Only check non-digits
            if(!isdigit(wParam))
            {
                switch(wParam)
                {
                    case _T('-'):
                    {
                        DWORD from = 0;
                        DWORD to   = 0;
                        SendMessage(hWnd, EM_GETSEL, (WPARAM)&from, (WPARAM)&to);
                        if(0 == from)
                        {
                            // if to is greater than zero, we're replacing the 0th
                            // char so we don't need to worry. If we're inserting
                            // have to check the first char isn't already a -
                            if(0 == to)
                            {
                                TCHAR buffer[2] = _T(""); // big enough for one char plus term null
                                SendMessage(hWnd, WM_GETTEXT, (WPARAM)2, (WPARAM)buffer);
                                if(buffer[0] == _T('-'))
                                    return 0;
                            }
                            // allow through
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    break;

                    case VK_BACK:
                    {
                        // allow backspace through
                    }
                    break;

                    default:
                    {
                        // block everything else
                        return 0;
                    }
                }
            }
        }
    }

	return CallWindowProc(lpfnEditCodeplugSsidOldProc, hWnd, uMsg, wParam, lParam);
	//return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

INT_PTR CALLBACK	EditCodeplugDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	std::cout << "D = EditCodeplugDialog, message: 0x" << std::hex << message << std::dec << std::endl;

	HWND		hEditSsid	= NULL;


	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
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
