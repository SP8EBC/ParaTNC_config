#include "stdafx.h"
#include "EditCodeplugDialog_Basic.h"
#include <iostream>
#include <iomanip>

#include "commctrl.h"
#include "Resource.h"

WNDPROC		lpfnEditCodeplugDialog_Basic_SsidOldProc = NULL;	// original Window Proc function for SSID edit

HWND		hEditCodeplugDialog_Basic = NULL;	// handle to this dialog window

HWND		hEditCodeplugDialog_Basic_ComboNS		= NULL;
HWND		hEditCodeplugDialog_Basic_ComboWE		= NULL;
HWND		hEditCodeplugDialog_Basic_ComboSymbol	= NULL;

HWND		hEditCodeplugDialog_Basic_SliderWxInterval		= NULL;
HWND		hEditCodeplugDialog_Basic_LabelWxInterval		= NULL;
HWND		hEditCodeplugDialog_Basic_SliderBeaconInterval	= NULL;

#define SSID_MIN		0
#define SSID_MAX		15

#define WX_INTERVAL_MIN		4
#define WX_INTERVAL_MAX		10
#define WX_INTERVAL_DEF		5

#define BEACON_INTERVAL_MIN	2	// 10 minutes
#define BEACON_INTERVAL_MAX	10	// 50 minutes
#define BEACON_INTERVAL_DEF	5	// 25 minutes

const static TCHAR szN[2] = {L'N', 0};
const static TCHAR szS[2] = {L'S', 0};

const static TCHAR szE[2] = {L'E', 0};
const static TCHAR szW[2] = {L'W', 0};

static LPCWSTR szDIGI			= L"DIGI\0";
static LPCWSTR szWIDE1DIGI		= L"DIGI Wide 1-1\0";
static LPCWSTR szHOUSE			= L"House\0";
static LPCWSTR szYACHT			= L"Yacht\0";
static LPCWSTR szIGATE			= L"Igate\0";
static LPCWSTR szRXIGATE		= L"RX-Igate\0";

static LPCWSTR szPATHNONE		= L"None\0";
static LPCWSTR szPATHWIDE1		= L"WIDE1-1\0";
static LPCWSTR szPATHWIDE21		= L"WIDE2-1\0";
static LPCWSTR szPATHWIDE22		= L"WIDE2-2\0";

static void EditCodeplugDialog_Basic_SetSliderLabel(int label, LRESULT value)
{
	TCHAR buffer[9];
	memset(buffer, 0x00, 0x9 * sizeof(TCHAR));

	_snwprintf(buffer, 0x9, L"%d min", value);

	SetDlgItemText(hEditCodeplugDialog_Basic, label, buffer);
}

LRESULT CALLBACK	EditCodeplugDialog_Basic_SsidProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = -1;
	int number = -1;

	// if character is OK and a message shall be passed to 'CallWindowProc'
	BOOL passMessage = false;

	// buffer to hold new keypress as an array od characters
	CHAR typed[2] = {(char) wParam, 0x00};

	// process only keystrokes with characters
    if(uMsg == WM_CHAR)
    {
		// special case for backspace
		if (wParam == VK_BACK)
		{
			// allow backspace to go through
			passMessage = true;
		}
		else 
		{
			// process and allow only digits to be input
			if (isdigit(wParam))
			{
				// get an integer from the edit content BEFORE this edit
				number = GetDlgItemInt(hEditCodeplugDialog_Basic, IDC_EC_EDIT_SSID, NULL, FALSE);
				
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
							passMessage = true;
						}
						else 
						{
							;	// if not ignore it
						}
					}
					else if (number == 0)
					{
						passMessage = true;
					}
					else
					{

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
		passMessage = true;
	}

	if (passMessage)
	{
		result = CallWindowProc(lpfnEditCodeplugDialog_Basic_SsidOldProc, hWnd, uMsg, wParam, lParam); 
	}

	return result;
}

INT_PTR CALLBACK	EditCodeplugDialog_Basic(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND		hEditSsid	= NULL;
	LRESULT wxIntervalPosition = -1;
	LRESULT beaconIntervalPosition = -1;

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		// store a handle to this dialog window
		hEditCodeplugDialog_Basic = hDlg;
		// get a handle to dialog controls
		hEditSsid = GetDlgItem(hDlg, IDC_EC_EDIT_SSID);
 		hEditCodeplugDialog_Basic_ComboNS = GetDlgItem(hDlg, IDC_EC_COMBO_NS);
		hEditCodeplugDialog_Basic_ComboWE = GetDlgItem(hDlg, IDC_EC_COMBO_WE);
		hEditCodeplugDialog_Basic_ComboSymbol = GetDlgItem(hDlg, IDC_EC_COMBO_SYMBOL);
		hEditCodeplugDialog_Basic_SliderWxInterval = GetDlgItem(hDlg, IDC_EC_SLIDER_INTERVAL_WX);
		hEditCodeplugDialog_Basic_SliderBeaconInterval = GetDlgItem(hDlg, IDC_EC_SLIDER_INTERVAL_BEACON);

		// add entries to North/South combo box
		SendMessage(hEditCodeplugDialog_Basic_ComboNS, CB_ADDSTRING, 0, (LPARAM)szN);
		SendMessage(hEditCodeplugDialog_Basic_ComboNS, CB_ADDSTRING, 0, (LPARAM)szS);

		// add entries to West/East combo box
		SendMessage(hEditCodeplugDialog_Basic_ComboWE, CB_ADDSTRING, 0, (LPARAM)szE);
		SendMessage(hEditCodeplugDialog_Basic_ComboWE, CB_ADDSTRING, 0, (LPARAM)szW);

		// add entries to symbol combo box
		SendMessage(hEditCodeplugDialog_Basic_ComboSymbol, CB_ADDSTRING, 0, (LPARAM)szDIGI);
		SendMessage(hEditCodeplugDialog_Basic_ComboSymbol, CB_ADDSTRING, 0, (LPARAM)szWIDE1DIGI);
		SendMessage(hEditCodeplugDialog_Basic_ComboSymbol, CB_ADDSTRING, 0, (LPARAM)szHOUSE);
		SendMessage(hEditCodeplugDialog_Basic_ComboSymbol, CB_ADDSTRING, 0, (LPARAM)szYACHT);
		SendMessage(hEditCodeplugDialog_Basic_ComboSymbol, CB_ADDSTRING, 0, (LPARAM)szIGATE);
		SendMessage(hEditCodeplugDialog_Basic_ComboSymbol, CB_ADDSTRING, 0, (LPARAM)szRXIGATE);

		// set range for weather interval slider
		SendMessage(hEditCodeplugDialog_Basic_SliderWxInterval, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(WX_INTERVAL_MIN, WX_INTERVAL_MAX));
        SendMessage(hEditCodeplugDialog_Basic_SliderWxInterval, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)WX_INTERVAL_DEF);
		EditCodeplugDialog_Basic_SetSliderLabel(IDC_EC_T_VAL_PERIOD_WX, WX_INTERVAL_DEF);

		// set range for beacon interval
		SendMessage(hEditCodeplugDialog_Basic_SliderBeaconInterval, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(BEACON_INTERVAL_MIN, BEACON_INTERVAL_MAX));
        SendMessage(hEditCodeplugDialog_Basic_SliderBeaconInterval, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)BEACON_INTERVAL_DEF);
		EditCodeplugDialog_Basic_SetSliderLabel(IDC_EC_T_VAL_PERIOD_BEACON, BEACON_INTERVAL_DEF);


		lpfnEditCodeplugDialog_Basic_SsidOldProc = (WNDPROC)SetWindowLongPtr(hEditSsid, GWLP_WNDPROC, (LONG_PTR)EditCodeplugDialog_Basic_SsidProc);
		
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	case WM_HSCROLL:
		wxIntervalPosition = SendMessage(hEditCodeplugDialog_Basic_SliderWxInterval, TBM_GETPOS, 0, 0);
		beaconIntervalPosition = SendMessage(hEditCodeplugDialog_Basic_SliderBeaconInterval, TBM_GETPOS, 0, 0);

		beaconIntervalPosition *= (LRESULT)5;

		EditCodeplugDialog_Basic_SetSliderLabel(IDC_EC_T_VAL_PERIOD_WX, wxIntervalPosition);
		EditCodeplugDialog_Basic_SetSliderLabel(IDC_EC_T_VAL_PERIOD_BEACON, beaconIntervalPosition);
		std::cout << "D = EditCodeplugDialog_Basic, WM_HSCROLL, slider wx position: " << wxIntervalPosition << std::endl;
		break;
	case WM_DESTROY:
		SetWindowLongPtr(hEditSsid, GWLP_WNDPROC, (LONG_PTR)lpfnEditCodeplugDialog_Basic_SsidOldProc);
		break;
	}

	return (INT_PTR)FALSE;
}
