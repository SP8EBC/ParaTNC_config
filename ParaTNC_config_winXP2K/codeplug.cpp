#include "stdafx.h"
#include "codeplug.h"


// a pointer to the configuration which user edits. the content inside
// this vector is set (overwritten) each time a file is opened from disk
// or a configuration from connected controller is received via KISS protocol
std::vector<uint8_t> vCodeplug_EditedConfig;

IConfigDecode * lpcCodeplug_ConfigDecode;

