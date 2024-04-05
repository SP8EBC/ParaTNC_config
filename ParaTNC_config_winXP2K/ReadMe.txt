========================================================================
    WIN32 APPLICATION : ParaTNC_config_vs2005 Project Overview
========================================================================

AppWizard has created this ParaTNC_config_vs2005 application for you.  

This file contains a summary of what you will find in each of the files that
make up your ParaTNC_config_vs2005 application.


ParaTNC_config_vs2005.vcproj
    This is the main project file for VC++ projects generated using an Application Wizard. 
    It contains information about the version of Visual C++ that generated the file, and 
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

ParaTNC_config_vs2005.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
AppWizard has created the following resources:

ParaTNC_config_vs2005.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
    Visual C++.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

ParaTNC_config_vs2005.ico
    This is an icon file, which is used as the application's icon (32x32).
    This icon is included by the main resource file ParaTNC_config_vs2005.rc.

small.ico
    This is an icon file, which contains a smaller version (16x16)
    of the application's icon. This icon is included by the main resource
    file ParaTNC_config_vs2005.rc.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named ParaTNC_config_vs2005.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
Serial and KISS Protocol Handling code:

serial/Serial.cpp
	Contains class Serial, which handles low level serial I/O using WinAPI.
	It also handles HDLC protocol to an extent it is used for KISS protocol,
	so it adds FEND at the begining of a transmission. It also detects FEND 
	during receiving to switch rx state machine
	
serial/SerialRxBackgroundThread.cpp
	Contains class SerialRxBackgroundThread, which uses Serial class and
	receives data asynchronously in the background. It uses internal 
	callback map, to invoke handler specific for certain KISS diagnostics
	serive. It is not used in case of this application, due to the fact
	that Windows is not able to transmit and receive data via the same serial
	port in the same time. 
	
protocol/ProtocolCommBackgroundThread.cpp
	Cotains class ProtocolCommBackgroundThread, which handless KISS diagnostics
	transction-wise. It is a middleware between Serial class (responsible for
	low level and WinAPI specific I/O) and shared library code, which decodes
	and encodes KISS diagnostics protocol data. Internally it uses a mutex to
	synchronize possible concurent access to serial port. It contains methods
	which creates background thread where KISS diagnostics comm transactions
	are handled. Callback are used to signalize main UI thread that diagnostic
	communication is done and a data is available.