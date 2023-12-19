#include "stdafx.h"
#include "EditCodeplugDialog.h"
#include <iostream>
#include <iomanip>

INT_PTR CALLBACK	EditCodeplugDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	std::cout << "D = EditCodeplugDialog, message: 0x" << std::hex << message << std::dec << std::endl;

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
