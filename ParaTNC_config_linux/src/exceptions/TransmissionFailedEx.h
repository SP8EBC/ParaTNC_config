/*
 * TransmissionFailedEx.h
 *
 *  Created on: Aug 21, 2022
 *      Author: mateusz
 */

#ifndef SRC_EXCEPTIONS_TRANSMISSIONFAILEDEX_H_
#define SRC_EXCEPTIONS_TRANSMISSIONFAILEDEX_H_

class TransmissionFailedEx {
public:
	TransmissionFailedEx();
	virtual ~TransmissionFailedEx();
	TransmissionFailedEx(const TransmissionFailedEx &other);
	TransmissionFailedEx(TransmissionFailedEx &&other);
	TransmissionFailedEx& operator=(const TransmissionFailedEx &other);
	TransmissionFailedEx& operator=(TransmissionFailedEx &&other);
};

#endif /* SRC_EXCEPTIONS_TRANSMISSIONFAILEDEX_H_ */
