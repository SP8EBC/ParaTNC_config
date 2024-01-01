#include "stdafx.h"
#include "resource.h"
#include "codeplug.h"

#include <iostream>

#include "../shared/config/decode/ValidateVer0.h"

// a pointer to the configuration which user edits. the content inside
// this vector is set (overwritten) each time a file is opened from disk
// or a configuration from connected controller is received via KISS protocol
std::vector<uint8_t> vCodeplug_EditedConfig;

IConfigDecode * lpcCodeplug_ConfigDecode = NULL;
IConfigEcode * lpcCodeplug_ConfigEncode = NULL;

static ValidateVer0 Codeplug_Validate;

//
//  FUNCTION: Codeplug_NewDataCallback()
//
//  PURPOSE:	Called by KISS communciation classes / functions after new codeplug data
//				had been uploaded by a TNC and this codepug has been validated OK
//
//
void Codeplug_NewDataCallback() {
	std::cout << "I = Codeplug_NewDataCallback" << std::endl;

	if (lpcCodeplug_ConfigDecode != NULL) {
		delete lpcCodeplug_ConfigDecode;
	}
	lpcCodeplug_ConfigDecode = new DecodeVer0(vCodeplug_EditedConfig);

	if (lpcCodeplug_ConfigEncode != NULL) {
		delete lpcCodeplug_ConfigEncode;
	}
	lpcCodeplug_ConfigEncode = new EncodeVer0(vCodeplug_EditedConfig);

	std::string beaconDescription;
	lpcCodeplug_ConfigDecode->getDescritpion(beaconDescription);

	const float latitude = lpcCodeplug_ConfigDecode->getLatitude();
	const float longitude = lpcCodeplug_ConfigDecode->getLongitude();

	std::cout << "I = beaconDescription: " << beaconDescription << std::endl;
	std::cout << "I = latitude: " << latitude << std::endl;
	std::cout << "I = longitude: " << longitude << std::endl;
}

bool Codeplug_CheckIsLoaded(HWND hParentWnd) {

	if (lpcCodeplug_ConfigEncode == NULL || vCodeplug_EditedConfig.size() == 0 ||
		Codeplug_Validate.checkValidate(vCodeplug_EditedConfig) == FALSE) 
	{
		BOOL intres = IS_INTRESOURCE(IDS_COMM_TIMEOUT_TITLE);
		WORD LangID = MAKELANGID(LANG_ENGLISH,SUBLANG_DEFAULT);
		
		TCHAR szTitle[64];
		LoadString(NULL, IDS_EDIT_CODEPLUG_DATA, szTitle, 64);
		
		TCHAR szMessage[64];
		LoadString(NULL, IDS_NO_CODEPLUG, szMessage, 64);

		MessageBox(hParentWnd, szMessage, szTitle, MB_OK | MB_ICONERROR);

		return false;
	}

	return true;

}