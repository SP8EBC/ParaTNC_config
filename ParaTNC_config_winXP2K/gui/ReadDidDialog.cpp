#include "stdafx.h"
#include "main.h"
#include "ReadDidDialog.h"
#include "Resource.h"
#include "../files/DiagnosticDescription.h"
#include "../misc/AuxStuff.h"

#include <iostream>
#include "assert.h"

WNDPROC		lpfnReadDidDialog_DidListOldProc = NULL; // original Window Proc function for DID list

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
HWND		hReadDidDialog_StaticId				= NULL;	// Static text to display selected data ID
HWND		hReadDidDialog_StaticRawHexResponse	= NULL;	// Raw response received from RS232
HWND		hReadDidDialog_CheckboxUnscaled		= NULL;

UINT		ReadDidDialog_StateCheckboxUnscaled = 0;
UINT		ReadDidDialog_SelectedListIndex		= 0;		
UINT		ReadDidDialog_SelectedDid			= 0;	// DID selected on the list
UINT		ReadDidDialog_ListIdxOfQueriedDid	= 0;
UINT		ReadDidDialog_QueriedDid			= 0;

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
		const std::vector<DDD> & ddd = vDiagnosticDescription_DidDefs;
		const size_t siz = ddd.size();

		for (size_t i = 0; i < siz; i++)
		{
			SendMessage(hReadDidDialog_List, LB_ADDSTRING , 0, (LPARAM)ddd.at(i).didNameOnList);
		}
	}
}

//
//  FUNCTION: ReadDidDialog_NrcCallback(uint16_t nrc)
//
//	PARAMETERS:
//				uint16_t nrc
// 
//  PURPOSE: Display error message box with an info, that a NRC is received
//
//
//
static VOID ReadDidDialog_NrcCallback(uint16_t nrc)
{
	assert(hReadDidDialog != NULL);

	WORD LangID = MAKELANGID(LANG_ENGLISH,SUBLANG_DEFAULT);
	
	TCHAR szTitle[64];
	LoadString(NULL, IDS_NRC_RECEIVED_TITLE, szTitle, 64);
	
	TCHAR szMessage[64];
	LoadString(NULL, AuxStuff::getResourceIdForNrc(nrc), szMessage, 64);

	MessageBox(hReadDidDialog, szMessage, szTitle, MB_OK | MB_ICONERROR);
}

//
//  FUNCTION: ReadDidDialog_SetDialogControlsToDidInformation(int didIndex)
//
//	PARAMETERS:
//				int didIndex
//
//  PURPOSE: Updates EDITTEXT controls with information about selected did
//
//
//
static VOID ReadDidDialog_SetDialogControlsWithDidInformation(int didIndex)
{
	const std::vector<DDD> & ddd = vDiagnosticDescription_DidDefs;
	const size_t siz = ddd.size();

	std::cout << "D = ReadDidDialog_SetDialogControlsWith..., " << std::hex <<
			"didIndex: 0x" << didIndex << ", " << 
			"siz: 0x" << siz << 
			std::dec << std::endl;

	if (didIndex < (int)siz)
	{
		// get definition information for a did stored in a certain index. 
		// an assumption is that a vector of diagnostic description information
		// stores elements in the same order they are displayed on a list
		const DDD & didDefinition = ddd.at(didIndex);

		// set text edit with DID name
		SetDlgItemText(hReadDidDialog, IDC_RDID_EDIT_DIDNAME, (LPCWSTR)didDefinition.didName);

		// set text edit with DID description
		SetDlgItemText(hReadDidDialog, IDC_RDID_EDIT_DIDDESCR, (LPCWSTR)didDefinition.didDescription);

		//SendMessage(hReadDidDialog_EditDidDescsription, EM_SETMARGINS, (WPARAM)EC_RIGHTMARGIN, MAKELPARAM(0, 50));

		// set static text with DID 
		TCHAR buffer[5];
		_sntprintf_s(buffer, 5, 5, _T("%X"), didDefinition.id);
		SetDlgItemText(hReadDidDialog, IDC_RDID_STATIC_ID, (LPCWSTR)buffer);

		// store DID which is selected
		ReadDidDialog_SelectedDid = didDefinition.id;

		if (didDefinition.first.isUsed())
		{
			SetDlgItemText(hReadDidDialog, IDC_RDID_EDIT_1ST_NAME, (LPCWSTR)didDefinition.first.name);
		}

		if (didDefinition.second.isUsed())
		{
			SetDlgItemText(hReadDidDialog, IDC_RDID_EDIT_2ND_NAME, (LPCWSTR)didDefinition.second.name);
		}

		if (didDefinition.third.isUsed())
		{
			SetDlgItemText(hReadDidDialog, IDC_RDID_EDIT_3RD_NAME, (LPCWSTR)didDefinition.third.name);
		}

	}
}

static VOID ReadDidDialog_Update_RenderValue(int dialogItem, int32_t value, const DDDD& definition, const bool displayUnscaled)
{
	TCHAR buffer[64];
	memset(buffer, 0x00, sizeof(TCHAR) * 64);

	assert(dialogItem != 0);

	BYTE precision = definition.precisionAfterDecimal;

	if (definition.isScaled() && !displayUnscaled)
	{
		float scaledValue = definition.scale(value);

		if (precision == 1)
		{
			_sntprintf_s(buffer, 64, 64, _T("%.1f"), scaledValue);
		}
		else if (precision == 2)
		{
			_sntprintf_s(buffer, 64, 64, _T("%.2f"), scaledValue);
		}
		else
		{
			_sntprintf_s(buffer, 64, 64, _T("%d"), (int32_t)scaledValue);
		}
	}
	else
	{
		_sntprintf_s(buffer, 64, 64, _T("%d"), value);
	}

	SetDlgItemText(hReadDidDialog, dialogItem, (LPCWSTR)buffer);
}

static VOID ReadDidDialog_Update_RenderValue(int dialogItem, float value, const DDDD& definition, const bool displayUnscaled)
{
	TCHAR buffer[64];
	memset(buffer, 0x00, sizeof(TCHAR) * 64);

	assert(dialogItem != 0);

	BYTE precision = definition.precisionAfterDecimal;
	float scaledValue = 0.0f;

	if (definition.isScaled() && !displayUnscaled)
	{
		float scaledValue = definition.scale(value);
	}
	else
	{
		float scaledValue = value;
	}

	if (precision == 1)
	{
		_sntprintf_s(buffer, 64, 64, _T("%.1f"), scaledValue);
	}
	else if (precision == 2)
	{
		_sntprintf_s(buffer, 64, 64, _T("%.2f"), scaledValue);
	}
	else
	{
		_sntprintf_s(buffer, 64, 64, _T("%f"), scaledValue);
	}

	SetDlgItemText(hReadDidDialog, dialogItem, (LPCWSTR)buffer);
}

//
//  FUNCTION: ReadDidDialog_Update(DidResponse)
//
//	PARAMETERS:
//				DidResponse didResponse
//
//
//  PURPOSE: Updates all controls in the dialog with information read from the controller
//
//  WM_INITDIALOG	- Initialized a dialog, set properties for all controls
//  WM_COMMAND		- Handles an event from clicking certain controls, like OK or Cancel button
//	WM_HSCROLL		- Handles an event from moving sliders
//  WM_DESTROY		- post a quit message and return
//
//
static VOID	ReadDidDialog_Update(DidResponse didResponse, const std::vector<uint8_t> & rawResponse)
{
	const int bufferSize = 100;
	TCHAR buffer[bufferSize];
	memset(buffer, 0x00, sizeof(TCHAR) * bufferSize);
	const int idFromResponse = didResponse.did;

	std::cout << "I = ReadDidDialog_Update, idFromResponse: 0x" << std::hex << idFromResponse << std::dec << std::endl;

	const DDD & didDefinition = vDiagnosticDescription_DidDefs.at(ReadDidDialog_ListIdxOfQueriedDid);

	bool notScale = false;
	if (ReadDidDialog_StateCheckboxUnscaled == BST_CHECKED)
	{
		notScale = true;
	}

	if (idFromResponse == ReadDidDialog_QueriedDid)
	{
		if (didDefinition.first.isUsed())
		{
			switch(didResponse.firstSize) 
			{
			case DIDRESPONSE_DATASIZE_EMPTY:
			case DIDRESPONSE_DATASIZE_INT8:
				ReadDidDialog_Update_RenderValue(IDC_RDID_EDIT_1ST_VALUE, didResponse.first.i8, didDefinition.first, notScale);

				break;
			case DIDRESPONSE_DATASIZE_INT16:
			ReadDidDialog_Update_RenderValue(IDC_RDID_EDIT_1ST_VALUE, didResponse.first.i16, didDefinition.first, notScale);

				break;
			case DIDRESPONSE_DATASIZE_INT32:
				ReadDidDialog_Update_RenderValue(IDC_RDID_EDIT_1ST_VALUE, didResponse.first.i32, didDefinition.first, notScale);

				break;
			case DIDRESPONSE_DATASIZE_FLOAT:
				ReadDidDialog_Update_RenderValue(IDC_RDID_EDIT_1ST_VALUE, didResponse.first.f, didDefinition.first, notScale);
				break;
			case DIDRESPONSE_DATASIZE_STRING:
				_mbstowcs_l(buffer, didResponse.first.str, bufferSize, localeEnglish);
				SetDlgItemText(hReadDidDialog, IDC_RDID_EDIT_1ST_VALUE, (LPCWSTR)buffer);
				break;
			} // switch(didResponse.firstSize) 
		} // if (didDefinition.first.isUsed())

		if (didDefinition.second.isUsed())
		{
			// can't send more than one string in single DID
			assert(didResponse.secondSize != DIDRESPONSE_DATASIZE_STRING);

			switch(didResponse.secondSize) 
			{
			case DIDRESPONSE_DATASIZE_EMPTY:
			case DIDRESPONSE_DATASIZE_INT8:
				ReadDidDialog_Update_RenderValue(IDC_RDID_EDIT_2ND_VALUE, didResponse.second.i8, didDefinition.second, notScale);

				break;
			case DIDRESPONSE_DATASIZE_INT16:
				ReadDidDialog_Update_RenderValue(IDC_RDID_EDIT_2ND_VALUE, didResponse.second.i16, didDefinition.second, notScale);

				break;
			case DIDRESPONSE_DATASIZE_INT32:
				ReadDidDialog_Update_RenderValue(IDC_RDID_EDIT_2ND_VALUE, didResponse.second.i32, didDefinition.second, notScale);

				break;
			case DIDRESPONSE_DATASIZE_FLOAT:
				ReadDidDialog_Update_RenderValue(IDC_RDID_EDIT_2ND_VALUE, didResponse.second.f, didDefinition.second, notScale);
				break;
			} // switch(didResponse.secondSize) 
		} // if (didDefinition.second.isUsed())

		if (didDefinition.third.isUsed())
		{
			// can't send more than one string in single DID
			assert(didResponse.thirdSize != DIDRESPONSE_DATASIZE_STRING);

			switch(didResponse.thirdSize) 
			{
			case DIDRESPONSE_DATASIZE_EMPTY:
			case DIDRESPONSE_DATASIZE_INT8:
				ReadDidDialog_Update_RenderValue(IDC_RDID_EDIT_3RD_VALUE, didResponse.third.i8, didDefinition.third, notScale);

				break;
			case DIDRESPONSE_DATASIZE_INT16:
				ReadDidDialog_Update_RenderValue(IDC_RDID_EDIT_3RD_VALUE, didResponse.third.i16, didDefinition.third, notScale);

				break;
			case DIDRESPONSE_DATASIZE_INT32:
				ReadDidDialog_Update_RenderValue(IDC_RDID_EDIT_3RD_VALUE, didResponse.third.i32, didDefinition.third, notScale);

				break;
			case DIDRESPONSE_DATASIZE_FLOAT:
				ReadDidDialog_Update_RenderValue(IDC_RDID_EDIT_3RD_VALUE, didResponse.second.f, didDefinition.second, notScale);
				break;
			} // switch(didResponse.thirdSize) 
		} // if (didDefinition.third.isUsed())
	} // if (idFromResponse == ReadDidDialog_QueriedDid)

	memset(buffer, 0x00, sizeof(TCHAR) * bufferSize);
	const int chrsPerB = 5;

	if ((int)rawResponse.size() < (bufferSize / chrsPerB))
	{
		for (int i = 0; i < (int)rawResponse.size(); i++)
		{
			_sntprintf_s((buffer + (i * chrsPerB)), bufferSize - (i * chrsPerB), chrsPerB, _T("0x%02X "), rawResponse[i]);
		}
	}
	else
	{
		_sntprintf_s(buffer, bufferSize, bufferSize, _T("Response to long to display here"));
	}

	SetDlgItemText(hReadDidDialog, IDC_RDID_STATIC_RAW, (LPCWSTR)buffer);
}

//
//  FUNCTION: ReadDidDialog_ListProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handles events generated by clicking on entries on DID list or pressing up-down cursor key
//
//  WM_INITDIALOG	- Initialized a dialog, set properties for all controls
//  WM_COMMAND		- Handles an event from clicking certain controls, like OK or Cancel button
//	WM_HSCROLL		- Handles an event from moving sliders
//  WM_DESTROY		- post a quit message and return
//
//
static LRESULT CALLBACK	ReadDidDialog_ListProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = -1;
	int number = -1;

	// if character is OK and a message shall be passed to 'CallWindowProc'
	BOOL passMessage = true;

	int wmId    = LOWORD(wParam);
	int wmEvent = HIWORD(wParam);

	// KEYDOWN is not an event from down arrow cursor key. It is the event
	// generated when a key is pressed down and periodically if it is held pressed
	if (uMsg == WM_CAPTURECHANGED || uMsg == WM_KEYDOWN)
	{
		// get an index of currently selected index 
		LRESULT selectedItem = SendMessage(hReadDidDialog_List,(UINT)LB_GETCURSEL, 0, 0);  

		ReadDidDialog_SelectedListIndex = (UINT)selectedItem;

		ReadDidDialog_SetDialogControlsWithDidInformation(selectedItem);
	}


	if (passMessage)
	{
		result = CallWindowProc(lpfnReadDidDialog_DidListOldProc, hWnd, uMsg, wParam, lParam); 
	}

	return result;
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

			hReadDidDialog_StaticId				= GetDlgItem(hDlg, IDC_RDID_STATIC_ID);
			hReadDidDialog_StaticRawHexResponse	= GetDlgItem(hDlg, IDC_RDID_STATIC_RAW);
			hReadDidDialog_CheckboxUnscaled		= GetDlgItem(hDlg, IDC_RDID_CHECK_UNSCALED);

			// set a tip text
			SetDlgItemText(hDlg, IDC_RDID_EDIT_DIDNAME, _T("Please use the list on the left to select a DID You want to read"));

			ReadDidDialog_FillDialogWithDiagDescription();
			
			// set window proc function for DID list
			lpfnReadDidDialog_DidListOldProc = (WNDPROC)SetWindowLongPtr(hReadDidDialog_List, GWLP_WNDPROC, (LONG_PTR)ReadDidDialog_ListProc);

			return (INT_PTR)TRUE;
		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			if (LOWORD(wParam) == IDREADDID)
			{
				std::cout << "D = ReadDidDialog_ListProc, IDREADDID, ReadDidDialog_SelectedDid: 0x" << std::hex << ReadDidDialog_SelectedDid << std::dec << std::endl;
				ReadDidDialog_ListIdxOfQueriedDid = ReadDidDialog_SelectedListIndex;
				ReadDidDialog_QueriedDid = ReadDidDialog_SelectedDid;
				ReadDidDialog_StateCheckboxUnscaled = IsDlgButtonChecked(hReadDidDialog, IDC_RDID_CHECK_UNSCALED);
				lpsKissProtocolComm->commReadDidAndUpdateGui(ReadDidDialog_Update, ReadDidDialog_NrcCallback, ReadDidDialog_QueriedDid);
				return (INT_PTR)TRUE;
			}
			break;
		case WM_DESTROY:
			//SetWindowLongPtr(hEditSsid, GWLP_WNDPROC, (LONG_PTR)lpfnEditCodeplugDialog_Basic_SsidOldProc);
			break;
	}

	return (INT_PTR)FALSE;
}
