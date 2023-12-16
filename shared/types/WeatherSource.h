/*
 * WeatherSource.h
 *
 *  Created on: Aug 28, 2022
 *      Author: mateusz
 */

#ifndef SRC_TYPES_WEATHERSOURCE_H_
#define SRC_TYPES_WEATHERSOURCE_H_


typedef enum WeatherSource {
	/**
		 * Internal sensors are:
		 * 	- Dallas DS18B20 for temperature
		 * 	- MS5611 or BME280 for pressure/humidity
		 * 	- analog/mechanical anemometer for wind
		 */
		WX_SOURCE_INTERNAL = 1,
		WX_SOURCE_INTERNAL_PT100 = 6,

		/**
		 * Lufft UMB devices
		 */
		WX_SOURCE_UMB = 2,

		/**
		 * RTU can be used for any measuremd parameter, but by default
		 * it is used for wind in the same way as the mechanic anemometer is.
		 * The controller asks for the windspeed and treat it as momentary value,
		 * which then is put into the circular buffer to calculate average and
		 * max from.
		 */
		WX_SOURCE_RTU = 3,

		/**
		 * This option makes a difference only for wind measurements. With all
		 * other parameters it works exactly the same as WX_SOURCE_RTU. In this mode
		 * the controller queries for average and maximum wind speed.
		 */
		WX_SOURCE_FULL_RTU = 4,
		WX_SOURCE_DAVIS_SERIAL = 5
} WeatherSource;


#endif /* SRC_TYPES_WEATHERSOURCE_H_ */
