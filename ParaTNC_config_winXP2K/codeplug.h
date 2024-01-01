#pragma once
#include <vector>
#include "stdint.h"

#include "../shared/config/decode/DecodeVer0.h"
#include "../shared/config/encode/EncodeVer0.h"

extern std::vector<uint8_t> vCodeplug_EditedConfig;

extern IConfigDecode * lpcCodeplug_ConfigDecode;
extern IConfigEcode * lpcCodeplug_ConfigEncode;

void Codeplug_NewDataCallback();
bool Codeplug_CheckIsLoaded(HWND hParentWnd);

