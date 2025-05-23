// ParaTNC_config_vs2005.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "main.h"
#include "codeplug.h"
#include "commctrl.h"

#include <iostream>
#include <fstream>
#include <io.h>
#include <fcntl.h>
#include <map>
#include <stdlib.h>

#include "./gui/EditCodeplugDialog_Basic.h"
#include "./gui/ReadDidDialog.h"

#include "./files/DiagnosticDescription.h"

#define MAX_LOADSTRING 100

class outbuf : public std::streambuf {
public:
    outbuf() {
        setp(0, 0);
    }

    virtual int_type overflow(int_type c = traits_type::eof()) {
        return fputc(c, stdout) == EOF ? traits_type::eof() : c;
    }
};

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HWND hWnd;

// KISS protocol communication handler
LPPCBT lpsKissProtocolComm;

// 
TCHAR szEditVersionText[64];
LPCTSTR szText = L"1234test";

// locales to be used for function operating on strings and characters
_locale_t localeEnglish;
std::locale cLocaleEnglish( "English_US" );

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	MainDialogProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// create the console
    if (AllocConsole()) {
        FILE* pCout;
        freopen_s(&pCout, "CONOUT$", "w", stdout);
        SetConsoleTitle(L"Debug Console");
    }

	// set std::cout to use my custom streambuf
    outbuf ob;
    std::streambuf *sb = std::cout.rdbuf(&ob);

	printf("dupa\r\n");
	std::cout << "dupa druga" << std::endl;

	localeEnglish = _create_locale(LC_ALL, "English");

	DiagnosticDescription_Init();

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PARATNC_CONFIG_VS2005, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	InitCommonControls();

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PARATNC_CONFIG_VS2005));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		/*now check for default dlg keyboard keys eg TAB, ESC*/
		//if ((!IsWindow(msg.hwnd))||(!IsDialogMessage(msg.hwnd,&msg)))
		if (!IsDialogMessage(hWnd,&msg))
		{
			/*translate and dispatch other messages*/
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (!TranslateAccelerator(hWnd, hAccelTable, &msg))
		{

		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= MainDialogProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PARATNC_CONFIG_VS2005));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PARATNC_CONFIG_VS2005);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{


   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAIN), 0, (DLGPROC)MainDialogProc);

   HANDLE hWndEdit = GetDlgItem(hWnd, IDC_EDIT_VERSION);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK MainDialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR did[6] = {0u};

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDC_BUTTON_START_SERIAL:
			lpsKissProtocolComm = new PCBT();
			break;
		case IDC_BUTTON_GET_VERSION:
			lpsKissProtocolComm->commVersionAndUpdateGui(hWnd, NULL);
			break;
		case IDC_BUTTON_READ_DID:
			//GetDlgItemText(hWnd, IDC_DID_NUM, did, 5);
			//didNumber = _wcstoi64(did, NULL, 16);
			//if (didNumber > 0x0 && didNumber < 0xFFFF) 
			//{
			//	lpsKissProtocolComm->commReadDidAndUpdateGui(NULL, NULL, didNumber);
			//}
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIAG_READ_DID), hWnd, ReadDidDialog);
			break;
		case IDC_BUTTON_GET_RUNNING:
			lpsKissProtocolComm->commRunningConfigAndUpdateGui(&Codeplug_NewDataCallback, &vCodeplug_EditedConfig);
			break;
		case IDC_BUTTON_EDIT_CODEPLUG_DATA:
			if (Codeplug_CheckIsLoaded(hWnd) == TRUE) {
				DialogBox(hInst, MAKEINTRESOURCE(IDD_EDIT_CODEPLUG_BASIC), hWnd, EditCodeplugDialog_Basic);
			}
			break;
		case IDCANCEL:
			DestroyWindow(hWnd);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return NULL;
			//return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_INITDIALOG:
		break;
	default:
		return NULL;
		//return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	std::cout << "D = About, message: 0x" << std::hex << message << std::dec << std::endl;

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
