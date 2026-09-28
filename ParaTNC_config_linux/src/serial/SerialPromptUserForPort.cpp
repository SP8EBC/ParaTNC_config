#include "SerialPromptUserForPort.h"

#include <iostream>
#include <vector>

#include <serial/serial.h>

bool SerialPromptUserForPort::isLegacyPortPresent (const std::string &port)
{
	// Kernel creates /dev/ttySx nodes for all possible legacy UARTs, but
	// configuring a port without real hardware behind fails in tcgetattr.
	// Serial object opens the port in constructor and closes it in destructor
	try {
		serial::Serial probe (port);
		return probe.isOpen ();
	}
	catch (const serial::IOException &) {
		return false;
	}
	catch (const serial::SerialException &) {
		return false;
	}
}

std::string SerialPromptUserForPort::promptForSerial ()
{
	std::vector<serial::PortInfo> ports;

	for (const serial::PortInfo &info : serial::list_ports ()) {
		if (info.port.rfind ("/dev/ttyS", 0) == 0 && !isLegacyPortPresent (info.port)) {
			continue;
		}
		ports.push_back (info);
	}

	if (ports.empty ()) {
		std::cout << "E = SerialPromptUserForPort, no serial ports found in this system!"
				  << std::endl;
		return "";
	}

	std::cout << "I = SerialPromptUserForPort, select which RS232 serial port You want to use:"
			  << std::endl;
	for (size_t i = 0; i < ports.size (); i++) {
		std::cout << "  [" << (i + 1) << "] " << ports[i].port;

		// for ports without USB info, description is only a copy of device name
		if (ports[i].port.rfind ("/" + ports[i].description) == std::string::npos) {
			std::cout << " - " << ports[i].description;
		}
		if (ports[i].hardware_id != "n/a") {
			std::cout << " (" << ports[i].hardware_id << ")";
		}
		std::cout << std::endl;
	}

	while (true) {
		std::cout << "Select port [1-" << ports.size () << "]: " << std::flush;

		std::string line;
		if (!std::getline (std::cin, line)) {
			// stdin closed, nothing more can be read
			std::cout << std::endl;
			return "";
		}

		try {
			size_t parsedLen = 0;
			const int selection = std::stoi (line, &parsedLen);
			if (parsedLen == line.length () && selection >= 1 && selection <= (int)ports.size ()) {
				return ports[selection - 1].port;
			}
		}
		catch (const std::exception &) {
			// not a number, fall through and ask again
		}

		std::cout << "Invalid selection, try again." << std::endl;
	}
}
