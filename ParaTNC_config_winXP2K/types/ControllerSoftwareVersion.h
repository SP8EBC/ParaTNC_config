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
	CHAR softwareVersion[4];

	// KISS protocol version
	CHAR kissVersion;
}ControllerSoftwareVersion;

typedef ControllerSoftwareVersion CSV;
typedef ControllerSoftwareVersion * LPCSV;