#include "stdafx.h"
#include "EditCodeplugDialog_Basic.h"
#include <iostream>
#include <iomanip>

#include "commctrl.h"

#include "Resource.h"
#include "main.h"

WNDPROC		lpfnEditCodeplugDialog_Basic_SsidOldProc = NULL;	// original Window Proc function for SSID edit
WNDPROC		lpfnEditCodeplugDialog_Basic_CallsignOldProc = NULL;
WNDPROC		lpfnEditCodeplugDialog_Basic_LatitudeOldProc = NULL;
WNDPROC		lpfnEditCodeplugDialog_Basic_LongitudeOldProc = NULL;

HWND		hEditCodeplugDialog_Basic = NULL;	// handle to this dialog window

HWND		hEditCodeplugDialog_Basic_ComboNS		= NULL;
HWND		hEditCodeplugDialog_Basic_ComboWE		= NULL;
HWND		hEditCodeplugDialog_Basic_ComboSymbol	= NULL;
HWND		hEditCodeplugDialog_Basic_ComboDigipath	= NULL;

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

//
//  FUNCTION: EditCodeplugDialog_Basic_SetSliderLabel(int, LRESULT)
//
//	PARAMETERS:
//		
//			int	label		//!< ID of label to update taken from Resource.h
//			LRESULT	value	//!< integer value (amount of minutes) to display on label
//
//  PURPOSE:	Custom message message processing function for SSID edit control. Checks if user input
//				is valid SSID
//
//  WM_CHAR	- Message of typing a character by a user
//
//
static void EditCodeplugDialog_Basic_SetSliderLabel(int label, LRESULT value)
{
	TCHAR buffer[9];
	memset(buffer, 0x00, 0x9 * sizeof(TCHAR));

	_snwprintf(buffer, 0x9, L"%d min", value);

	SetDlgItemText(hEditCodeplugDialog_Basic, label, buffer);
}

//
//  FUNCTION: EditCodeplugDialog_Basic_CallsignProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:	Custom message message processing function for Callsign edit control. Checks if user input
//				is valid
//
//  WM_CHAR	- Message of typing a character by a user
//
//
static LRESULT CALLBACK	EditCodeplugDialog_Basic_CallsignProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = -1;

	// if character is OK and a message shall be passed to 'CallWindowProc'
	BOOL passMessage = false;
	
#define CALLSIGN_BUFFER_LN	9
	TCHAR text[CALLSIGN_BUFFER_LN];

	memset (text, 0, CALLSIGN_BUFFER_LN * sizeof(TCHAR));

	// process only keystrokes with characters
    if(uMsg == WM_CHAR)
    {
		// get current edit content, before typed character is appended into it
		const UINT textLenght = GetDlgItemText(hEditCodeplugDialog_Basic, IDC_EC_EDIT_CALLSIGN, text, CALLSIGN_BUFFER_LN);

		std::cout << "D = EditCodeplugDialog_Basic_CallsignProc, wParam: " << (char)wParam << 
											", textLenght: " << textLenght << std::endl;

		// verify if a keypress is alphanumerical
		if (isalnum((TCHAR)wParam))
		{
			// check callsign current lenghth, before typed character is concenated
			if (textLenght > 5)
			{
				;// do not allow more than 6 characters
			}
			else
			{
				// convert character to upper case
				wParam =  _toupper_l((TCHAR)wParam, localeEnglish);

				passMessage = true;

			}
		}
		else if (wParam == VK_BACK)
		{
			passMessage = true;
		}
		else
		{
			;// if not ignore it
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

//
//  FUNCTION: EditCodeplugDialog_Basic_SsidProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:	Custom message message processing function for SSID edit control. Checks if user input
//				is valid SSID
//
//  WM_CHAR	- Message of typing a character by a user
//
//
static LRESULT CALLBACK	EditCodeplugDialog_Basic_SsidProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = -1;
	int number = -1;

	// if character is OK and a message shall be passed to 'CallWindowProc'
	BOOL passMessage = false;

	// buffer to hold new keypress as an array od characters
	TCHAR typed[2] = {(TCHAR) wParam, 0x00};

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
			if (isdigit((TCHAR)wParam))
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
						int _new = _tstoi(typed);

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

//
//  FUNCTION: EditCodeplugDialog_Basic_LatitudeProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:	Custom message message processing function for Latitude edit control.
//
//  WM_CHAR	- Message of typing a character by a user
//
//
static LRESULT CALLBACK	EditCodeplugDialog_Basic_LatitudeProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = -1;

	// if character is OK and a message shall be passed to 'CallWindowProc'
	BOOL passMessage = false;

#define LATITUDE_TEXT_LN	16
	TCHAR text[LATITUDE_TEXT_LN];

	memset (text, 0, LATITUDE_TEXT_LN * sizeof(TCHAR));

	// process only keystrokes with characters
    if(uMsg == WM_CHAR)
    {
		// special case for backspace
		if ((TCHAR)wParam == VK_BACK)
		{
			// allow backspace to go through
			passMessage = true;
		}
		else
		{
			// process and allow only digits and decimal point to be typed by the user
			if (isdigit((TCHAR)wParam) || (TCHAR)wParam == '.')
			{
				// get current text
				const UINT uLenght = GetDlgItemText(hEditCodeplugDialog_Basic, IDC_EC_EDIT_LATITUDE, text, LATITUDE_TEXT_LN);

				// limit how many characters cha be typed into text edit control
				if (uLenght > LATITUDE_TEXT_LN / 2)
				{
					;
				}
				else
				{
					// convert string to float
					float number = (float)_tstof_l(text, localeEnglish);

					std::cout << "D = EditCodeplugDialog_Basic_LatitudeProc, wParam: " << (char)wParam <<
					", number: " << std::setprecision(7) << number << std::endl;

					passMessage = true;
				}

			}
		}
	}
	else
	{
		passMessage = true;
	}

	if (passMessage)
	{
		result = CallWindowProc(lpfnEditCodeplugDialog_Basic_LatitudeOldProc, hWnd, uMsg, wParam, lParam); 
	}


	return result;
}

//
//  FUNCTION: EditCodeplugDialog_Basic_LongitudeProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:	Custom message message processing function for Longitude edit control.
//
//  WM_CHAR	- Message of typing a character by a user
//
//
static LRESULT CALLBACK	EditCodeplugDialog_Basic_LongitudeProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = -1;

	// if character is OK and a message shall be passed to 'CallWindowProc'
	BOOL passMessage = false;

#define LONGITUDE_TEXT_LN	16
	TCHAR text[LONGITUDE_TEXT_LN];

	memset (text, 0, LONGITUDE_TEXT_LN * sizeof(TCHAR));

	// process only keystrokes with characters
    if(uMsg == WM_CHAR)
    {
		// special case for backspace
		if ((TCHAR)wParam == VK_BACK)
		{
			// allow backspace to go through
			passMessage = true;
		}
		else
		{
			// process and allow only digits and decimal point to be typed by the user
			if (isdigit((TCHAR)wParam) || (TCHAR)wParam == '.')
			{
				// get current text
				const UINT uLenght = GetDlgItemText(hEditCodeplugDialog_Basic, IDC_EC_EDIT_LONGITUDE, text, LONGITUDE_TEXT_LN);

				// limit how many characters cha be typed into text edit control
				if (uLenght > LONGITUDE_TEXT_LN / 2)
				{
					;
				}
				else
				{
					// convert string to float
					float number = (float)_tstof_l(text, localeEnglish);

					std::cout << "D = EditCodeplugDialog_Basic_LongitudeProc, wParam: " << (char)wParam <<
					", number: " << std::setprecision(7) << number << std::endl;

					passMessage = true;
				}

			}
		}
	}
	else
	{
		passMessage = true;
	}

	if (passMessage)
	{
		result = CallWindowProc(lpfnEditCodeplugDialog_Basic_LongitudeOldProc, hWnd, uMsg, wParam, lParam); 
	}


	return result;
}


//
//  FUNCTION: EditCodeplugDialog_Basic(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the Edit Codeplug -> Radio Network Basic settings dialog.
//
//  WM_INITDIALOG	- Initialized a dialog, set properties for all controls
//  WM_COMMAND		- Handles an event from clicking certain controls, like OK or Cancel button
//	WM_HSCROLL		- Handles an event from moving sliders for weather pkts and beacons interval
//  WM_DESTROY		- post a quit message and return
//
//
INT_PTR CALLBACK	EditCodeplugDialog_Basic(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND		hEditSsid		= NULL;
	HWND		hEditCall		= NULL;
	HWND		hEditLatutude	= NULL;
	HWND		hEditLongitude	= NULL;

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
		hEditCall = GetDlgItem(hDlg, IDC_EC_EDIT_CALLSIGN);
		hEditLatutude = GetDlgItem(hDlg, IDC_EC_EDIT_LATITUDE);
		hEditLongitude = GetDlgItem(hDlg, IDC_EC_EDIT_LONGITUDE);

 		hEditCodeplugDialog_Basic_ComboNS = GetDlgItem(hDlg, IDC_EC_COMBO_NS);
		hEditCodeplugDialog_Basic_ComboWE = GetDlgItem(hDlg, IDC_EC_COMBO_WE);
		hEditCodeplugDialog_Basic_ComboSymbol = GetDlgItem(hDlg, IDC_EC_COMBO_SYMBOL);
		hEditCodeplugDialog_Basic_ComboDigipath = GetDlgItem(hDlg, IDC_EC_COMBO_PATH);
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

		// add entries to digipeater path combo box
		SendMessage(hEditCodeplugDialog_Basic_ComboDigipath, CB_ADDSTRING, 0, (LPARAM)szPATHNONE);
		SendMessage(hEditCodeplugDialog_Basic_ComboDigipath, CB_ADDSTRING, 0, (LPARAM)szPATHWIDE1);
		SendMessage(hEditCodeplugDialog_Basic_ComboDigipath, CB_ADDSTRING, 0, (LPARAM)szPATHWIDE21);
		SendMessage(hEditCodeplugDialog_Basic_ComboDigipath, CB_ADDSTRING, 0, (LPARAM)szPATHWIDE22);

		// set range for weather interval slider
		SendMessage(hEditCodeplugDialog_Basic_SliderWxInterval, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(WX_INTERVAL_MIN, WX_INTERVAL_MAX));
        SendMessage(hEditCodeplugDialog_Basic_SliderWxInterval, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)WX_INTERVAL_DEF);
		EditCodeplugDialog_Basic_SetSliderLabel(IDC_EC_T_VAL_PERIOD_WX, WX_INTERVAL_DEF);

		// set range for beacon interval
		SendMessage(hEditCodeplugDialog_Basic_SliderBeaconInterval, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(BEACON_INTERVAL_MIN, BEACON_INTERVAL_MAX));
        SendMessage(hEditCodeplugDialog_Basic_SliderBeaconInterval, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)BEACON_INTERVAL_DEF);
		EditCodeplugDialog_Basic_SetSliderLabel(IDC_EC_T_VAL_PERIOD_BEACON, BEACON_INTERVAL_DEF);


		lpfnEditCodeplugDialog_Basic_SsidOldProc = (WNDPROC)SetWindowLongPtr(hEditSsid, GWLP_WNDPROC, (LONG_PTR)EditCodeplugDialog_Basic_SsidProc);
		lpfnEditCodeplugDialog_Basic_CallsignOldProc = (WNDPROC)SetWindowLongPtr(hEditCall, GWLP_WNDPROC, (LONG_PTR)EditCodeplugDialog_Basic_CallsignProc);
		lpfnEditCodeplugDialog_Basic_LatitudeOldProc = (WNDPROC)SetWindowLongPtr(hEditLatutude, GWLP_WNDPROC, (LONG_PTR)EditCodeplugDialog_Basic_LatitudeProc);
		lpfnEditCodeplugDialog_Basic_LongitudeOldProc = (WNDPROC)SetWindowLongPtr(hEditLongitude, GWLP_WNDPROC, (LONG_PTR)EditCodeplugDialog_Basic_LongitudeProc);

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
		//std::cout << "D = EditCodeplugDialog_Basic, WM_HSCROLL, slider wx position: " << wxIntervalPosition << std::endl;
		break;
	case WM_DESTROY:
		SetWindowLongPtr(hEditSsid, GWLP_WNDPROC, (LONG_PTR)lpfnEditCodeplugDialog_Basic_SsidOldProc);
		break;
	}

	return (INT_PTR)FALSE;
}
