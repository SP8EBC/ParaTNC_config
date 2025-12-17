/*
 * Ver0Map.h
 *
 * 	Configuration data map for EA16 to EA20, although some parts are available
 * 	since EA05 like APRS-IS configuration, or since EA04 which introduced
 * 	API configuration
 *
 *  Created on: Apr 24, 2023
 *      Author: mateusz
 */

#ifndef SRC_CONFIG_VER0MAP_H_
#define SRC_CONFIG_VER0MAP_H_


#define CONFIG_MODE_PGM_CNTR	0x0
#define CONFIG_MODE_OFSET		0x20			//	Current size: 0x10, free: 0x10
#define CONFIG_BASIC_OFFSET		0x40			//	Current size: 0x9C, free: 0x44
#define CONFIG_SOURCES_OFFSET	0x120			//	Current size: 0x4,  free: 0x1C
#define CONFIG_UMB_OFFSET		0x140			//	Current size: 0x10, free: 0x10
#define CONFIG_RTU_OFFSET		0x160			//	Current size: 0x54, free: 0x4C
#define CONFIG_GSM_OFFSET		0x200			//	Current size: 0xF8,
#define CONFIG__END__OFFSET		0x300
#define CONFIG_BLOCK_SIZE		0x800

#define CRC_OFFSET				0x7F8
#define CRC_16B_WORD_OFFSET		CRC_OFFSET / 2
#define CRC_32B_WORD_OFFSET		CRC_OFFSET / 4

#define BASIC_CALLSIGN_OFFSET			0x0			// string - max ln 7
#define BASIC_SSID_OFFSET				0x7			// uint8_t
#define BASIC_LATITUDE_OFFSET			0x8			// float32_t
#define BASIC_NS_OFFSET					0xC			// uint8_t
#define BASIC_LONGITUDE_OFFS			0x10		// float32_t
#define BASIC_WE_OFFSET					0x14		// uint8_t
#define BASIC_COMMENT_OFFSET			0x15		// string - max ln 128
	#define BASIC_COMMENT_LENGHT		128
#define BASIC_SYMBOL_OFFSET				0x95		// uint8_t
#define BASIC_PATHTYPE_OFFSET			0x96		// uint8_t
#define BASIC_BEACON_BOOTUP_OFFSET		0x97		// uint8_t
#define BASIC_WX_TRANSMIT_PERIOD		0x98		// uint8_t
#define BASIC_BEACON_TRANSMIT_PERIOD	0x99		// uint8_t
#define BASIC_WX_DOUBLE_TRANSMIT		0x9A		// uint8_t

#define MODE_DIGI_OFFSET				0x0			// uint8_t
#define MODE_WX_OFFSET					0x1			// uint8_t
#define MODE_WX_UMB_OFFSET				0x2			// uint8_t
#define MODE_WX_MODBUS_OFFSET			0x3			// uint8_t
#define MODE_WX_DAVIS_OFFSET			0x4			// uint8_t
#define MODE_WX_MS5611_OR_BME_OFFSET	0x5			// uint8_t
#define MODE_WX_ANEMOMETER_CONST_OFFSET	0x6			// uint8_t
#define MODE_WX_DUST_SENSOR_OFFSET		0x7			// uint8_t
#define MODE_WX_PT_SENSOR_OFFSET		0x8			// uint8_t
#define MODE_VICTRON_OFFSET				0x9			// uint8_t
#define MODE_DIGI_VISCOUS_OFFSET		0xA			// uint8_t
#define MODE_DIGI_ONLY_SSID_OFFSET		0xB			// uint8_t
#define MODE_DIGI_VISCOUS_DELAY_OFFSET	0xC			// uint8_t
#define MODE_DIGI_DELAY_100MSEC_OFFSET	0xD			// uint8_t
#define MODE_POWERSAVE_OFFSET			0xE			// uint8_t
#define MODE_POWERSAVE_KEEP_GSM_OFFSET	0xF			// uint8_t
#define MODE_GSM_OFFSET					0x10		// uint8_t

#define SOURCE_TEMPERATURE_OFFSET		0x0			// uint8_t
#define SOURCE_PRESSURE_OFFSET			0x1			// uint8_t
#define SOURCE_HUMIDITY_OFFSET			0x2			// uint8_t
#define SOURCE_WIND_OFFSET				0x3			// uint8_t

#define UMB_SLAVE_CLASS_OFFSET			0x0			// uint16_t
#define UMB_SLAVE_ID_OFFSET				0x2			// uint16_t
#define UMB_CHANNEL_WINDSPEED			0x4			// uint16_t
#define UMB_CHANNEL_WINDGUST			0x6			// uint16_t
#define UMB_CHANNEL_WINDDIRECTION		0x8			// uint16_t
#define UMB_CHANNEL_TEMPERATURE			0xA			// uint16_t
#define UMB_CHANNEL_QNH					0xC			// uint16_t

#define RTU_SLAVE_SPEED_OFFSET			0x0			// uint16_t
#define RTU_SLAVE_PARITY_SPEED			0x2			// uint8_t
#define RTU_SLAVE_STOP_BITS_OFFSET		0x3			// uint8_t
#define RTU_USE_FULL_WIND_DATA_OFFSET	0x4			// uint8_t
#define RTU_TEMPERATURE_SRC_OFFSET		0x5			// uint8_t
#define RTU_HUMIDITY_SRC_OFFSET			0x6			// uint8_t
#define RTU_PRESSURE_SRC_OFFSET			0x7			// uint8_t
#define RTU_WIND_DIR_OFFSET				0x8			// uint8_t
#define RTU_WIND_SPEED_OFFSET			0x9			// uint8_t
#define RTU_WIND_GUSTS_OFFSET			0xA			// uint8_t

#define RTU_SLAVE_CONFIG_BLOCK_OFFSET	0xB
#define RTU_SLAVE_CONFIG_BLOCK_SIZE		0xC
#define RTU_SLAVE_HOW_MANY				0x6

#define RTU_X_BUS_ADDRESS			0x0
#define RTU_X_FUNCTION				0x1
#define RTU_X_REGUSTER_ADDR			0x3
#define RTU_X_LENGHT				0x5
#define RTU_X_SCALLING_A			0x7
#define RTU_X_SCALLING_B			0x8
#define RTU_X_SCALLING_C			0x9
#define RTU_X_SCALLING_D			0xA
#define RTU_X_UNSIGNED_SIGNED		0xB

#define GSM_PIN_OFFSET				0x0		// string - max ln 5
		#define GSM_PIN_LENGHT		5
#define GSM_APN_OFFSET				0x5		// string - max ln 24
		#define GSM_APN_LENGHT		24
#define GSM_USERNAME_OFFSET			0x1D	// string - max ln 24
		#define GSM_USERNAME_LEN	24
#define GSM_PASSWORD_OFFSET			0x35	// string - max ln 24
		#define GSM_PASSWORD_LEN	24
#define GSM_API_ENABLE_OFFSET		0x4D	// uint8_t
#define GSM_API_BASE_URL_OFFSET		0x4E	// string - max ln 64
	#define GSM_API_BASE_URL_LEN	64
#define GSM_API_STATION_NAME_OFFSET	0x8E	// string - max ln 32
	#define GSM_API_STATION_NAME_LN	32
#define GSM_APRSIS_ENABLE			0xAE	// uint8_t
#define GSM_APRSIS_SERVER_OFFSET	0xAF	// string - max ln 64
	#define GSM_APRSIS_SERVER_LN	64
#define GSM_APRSIS_PORT_OFFSET		0xF0	// uint16_t
	#define GSM_APRSIS_PORT_LN		2
#define GSM_APRSIS_PASSCODE_OFFSET	0xF4	// string - max ln 8


#endif /* SRC_CONFIG_VER0MAP_H_ */
