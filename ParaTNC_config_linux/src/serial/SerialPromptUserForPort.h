#ifndef EBAC8668_E424_49A1_9632_7D1D38028D3D
#define EBAC8668_E424_49A1_9632_7D1D38028D3D

#include <string>

class SerialPromptUserForPort
{
    private:
        /**
         * Checks if /dev/ttySx port is backed by a real UART. Linux kernel
         * always creates a bunch of these device nodes regardless of hardware
         */
        static bool isLegacyPortPresent(const std::string & port);

    public:
        /**
         * Lists all serial ports present in the system and asks the user to
         * choose one of them. Returns a path to selected port or an empty
         * string if no port is available or user aborted the selection
         */
        static std::string promptForSerial();
};

#endif /* EBAC8668_E424_49A1_9632_7D1D38028D3D */
