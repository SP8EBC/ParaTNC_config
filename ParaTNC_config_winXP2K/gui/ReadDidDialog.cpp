#include "stdafx.h"
#include "ReadDidDialog.h"
#include "Resource.h"
#include "../files/DiagnosticDescription.h"

WNDPROC		lpfnReadDidDialog_OldProc = NULL;	// original Window Proc function

HWND		hReadDidDialog = NULL;	// handle to this dialog window

HWND		hReadDidDialog_List					= NULL;
HWND		hReadDidDialog_EditDidName			= NULL;
HWND		hReadDidDialog_EditDidDescsription	= NULL;
HWND		hReadDidDialog_Edit1stName			= NULL;
HWND		hReadDidDialog_Edit2ndName			= NULL;
HWND		hReadDidDialog_Edit3rdName			= NULL;
HWND		hReadDidDialog_Edit1stValue			= NULL;
HWND		hReadDidDialog_Edit2ndValue			= NULL;
HWND		hReadDidDialog_Edit3rdValue			= NULL;

//
//  FUNCTION: ReadDidDialog_FillDialogWithDiagDescription()
//
//  PURPOSE: Fills IDC_RDID_LIST controls will a list of all DIDs defined 
//			 in diagnostics description file
//
//
static VOID	ReadDidDialog_FillDialogWithDiagDescription()
{
	if (hReadDidDialog_List != NULL && hReadDidDialog != NULL)
	{
		std::vector<DDD> & ddd = vDiagnosticDescription_DidDefs;
		const size_t siz = ddd.size();

		for (size_t i = 0; i < siz; i++)
		{
			SendMessage(hReadDidDialog_List, LB_ADDSTRING , 0, (LPARAM)ddd.at(i).didNameOnList);
		}
	}
}

//
//  FUNCTION: ReadDidDialog_Update(DidResponse *)
//
//  PURPOSE: Updates all controls in the dialog with information read from the controller
//
//  WM_INITDIALOG	- Initialized a dialog, set properties for all controls
//  WM_COMMAND		- Handles an event from clicking certain controls, like OK or Cancel button
//	WM_HSCROLL		- Handles an event from moving sliders
//  WM_DESTROY		- post a quit message and return
//
//
void	ReadDidDialog_Update(DidResponse * didResponse)
{

}

//
//  FUNCTION: ReadDidDialog(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: 
//
//  WM_INITDIALOG	- Initialized a dialog, set properties for all controls
//  WM_COMMAND		- Handles an event from clicking certain controls, like OK or Cancel button
//	WM_HSCROLL		- Handles an event from moving sliders
//  WM_DESTROY		- post a quit message and return
//
//
INT_PTR CALLBACK		ReadDidDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
			// store a handle to this dialog window
			hReadDidDialog = hDlg;

			// get handles to all important controls on this dialog
			hReadDidDialog_List					= GetDlgItem(hDlg, IDC_RDID_LIST);
			hReadDidDialog_EditDidName			= GetDlgItem(hDlg, IDC_RDID_EDIT_DIDNAME);
			hReadDidDialog_EditDidDescsription	= GetDlgItem(hDlg, IDC_RDID_EDIT_DIDDESCR);
			hReadDidDialog_Edit1stName			= GetDlgItem(hDlg, IDC_RDID_EDIT_1ST_NAME);
			hReadDidDialog_Edit2ndName			= GetDlgItem(hDlg, IDC_RDID_EDIT_2ND_NAME);
			hReadDidDialog_Edit3rdName			= GetDlgItem(hDlg, IDC_RDID_EDIT_3RD_NAME);
			hReadDidDialog_Edit1stValue			= GetDlgItem(hDlg, IDC_RDID_EDIT_1ST_VALUE);
			hReadDidDialog_Edit2ndValue			= GetDlgItem(hDlg, IDC_RDID_EDIT_2ND_VALUE);
			hReadDidDialog_Edit3rdValue			= GetDlgItem(hDlg, IDC_RDID_EDIT_3RD_VALUE);

			SetDlgItemText(hDlg, IDC_RDID_EDIT_DIDNAME, _T("Please use the list on the left to select a DID You want to read"));

			ReadDidDialog_FillDialogWithDiagDescription();

			return (INT_PTR)TRUE;
		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			if (LOWORD(wParam) == IDREADDID)
			{
				return (INT_PTR)TRUE;
			}
			break;
		case WM_DESTROY:
			//SetWindowLongPtr(hEditSsid, GWLP_WNDPROC, (LONG_PTR)lpfnEditCodeplugDialog_Basic_SsidOldProc);
			break;
	}

	return (INT_PTR)FALSE;
}
