/*
 * OutputValues.h
 *
 *  Created on: 24.09.2017
 *      Author: mateusz
 */

#ifndef TYPES_OUTPUTVALUES_H_
#define TYPES_OUTPUTVALUES_H_

class OutputValues {

	unsigned short windDirection;
	float windSpeed, windGusts;
	float temperature;
	unsigned short qnh, qfe;
	unsigned char humidity;

public:
	OutputValues();
	virtual ~OutputValues();

	unsigned char getHumidity() const {
		return humidity;
	}

	void setHumidity(unsigned char humidity) {
		this->humidity = humidity;
	}

	unsigned short getQfe() const {
		return qfe;
	}

	void setQfe(unsigned short qfe) {
		this->qfe = qfe;
	}

	unsigned short getQnh() const {
		return qnh;
	}

	void setQnh(unsigned short qnh) {
		this->qnh = qnh;
	}

	float getTemperature() const {
		return temperature;
	}

	void setTemperature(float temperature) {
		this->temperature = temperature;
	}

	unsigned short getWindDirection() const {
		return windDirection;
	}

	void setWindDirection(unsigned short windDirection) {
		this->windDirection = windDirection;
	}

	float getWindGusts() const {
		return windGusts;
	}

	void setWindGusts(float windGusts) {
		this->windGusts = windGusts;
	}

	float getWindSpeed() const {
		return windSpeed;
	}

	void setWindSpeed(float windSpeed) {
		this->windSpeed = windSpeed;
	}
};

#endif /* TYPES_OUTPUTVALUES_H_ */
