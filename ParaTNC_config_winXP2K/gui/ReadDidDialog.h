#pragma once
#include "stdafx.h"
#include "../shared/types/DidResponse.h"

VOID					ReadDidDialog_Update(DidResponse * didResponse);
INT_PTR CALLBACK		ReadDidDialog(HWND, UINT, WPARAM, LPARAM);
