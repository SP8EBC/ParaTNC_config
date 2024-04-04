#pragma once

#include "resource.h"
#include "locale.h"
#include <locale>

#include "./protocol/ProtocolCommBackgroundThread.h"

extern _locale_t localeEnglish;
extern std::locale cLocaleEnglish;

// KISS protocol communication handler
extern LPPCBT lpsKissProtocolComm;