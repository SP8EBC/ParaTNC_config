#pragma once 

#include <windows.h>
/*
 * Used to distinguish between ParaTNC and 
 * ParaMETEO.
 */
typedef enum ControlerSoftwareType {
	CONTROLLER_SOFTWARE_PARATNC,
	CONTROLLER_SOFTWARE_PARAMETEO
}ControlerSoftwareType;


/*
 *
 *
 */
typedef struct ControllerSoftwareVersion {

	// controller type
	ControlerSoftwareType type;

	// software version
	CHAR softwareVersion[5];

	// software version as wide string
	WCHAR softwareVersionW[5];

	// KISS protocol version
	CHAR kissVersion;
}ControllerSoftwareVersion;

typedef ControllerSoftwareVersion CSV;
typedef ControllerSoftwareVersion * LPCSV;