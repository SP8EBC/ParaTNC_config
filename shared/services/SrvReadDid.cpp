/*
 * SrvReadDid.cpp
 *
 *  Created on: Jul 6, 2023
 *      Author: mateusz
 */
#include "stdafx.h"

#include "SrvReadDid.h"
#include "../shared/kiss_communication_service_ids.h"

#include <iostream>

SrvReadDid::SrvReadDid() {
#if defined (_MSC_VER) && (_MSC_VER <= 1400)
	syncEvent = OpenEvent(EVENT_ALL_ACCESS, false, L"ServiceSyncEv");
#else
	conditionVariable = 0;
#endif
}

SrvReadDid::~SrvReadDid() {
	// TODO Auto-generated destructor stub
}

void SrvReadDid::sendRequest() {
	if (s != 0 && this->requestData.size() > 0) {
		s->transmitKissFrame(this->requestData);
	}
}

void SrvReadDid::receiveSynchronously(IService_NegativeResponseCodeCbk cbk) {
	if (s) {
		std::vector<uint8_t> response;

		// receive a response
		s->receiveKissFrame(response);

		if (response.size() > 1) {
			// get frame type, which was received
			uint8_t frameType = response[1];

			if (frameType == KISS_NEGATIVE_RESPONSE_SERVICE) 
			{
				std::cout << "E = SrvReadDid::receiveSynchronously, NRC received: 0x" << 
					std::hex << response[2] << std::dec << std::endl;

				if (cbk != NULL)
				{
					cbk(response[2]);
				}
			}
			else if (frameType == KISS_READ_DID_RESP)
			{
				// use callback manualy
				this->callback(&response);
			}
			else
			{
				std::cout << "E = SrvReadDid::receiveSynchronously, response to unknown service!" << std::endl;
			}


		}
	}
}

void SrvReadDid::sendRequestForDid(uint16_t did) {

	// check if all variables are set correctly
	if (this->s != NULL) {
		// wipe content of request buffer
		requestData.clear();

		// service identifier
		requestData.push_back(KISS_READ_DID);

		// data identifier - LSB
		requestData.push_back(did & 0xFFu);

		// data identifier - MSB
		requestData.push_back((did & 0xFF00u) >> 8);

		sendRequest();
	}

}

void SrvReadDid::callback(
		const std::vector<unsigned char, std::allocator<unsigned char> > *frame) {

	std::vector<uint8_t>::const_iterator it = frame->begin();

	std::cout << "I = SrvReadDid::callback" << std::endl;

	int8_t generic_8;
	int16_t generic_16;
	int32_t generic_32;
	float generic_float = 0.0f;

	for (size_t i = 0; i < frame->size(); i++) {
		std::cout << ", 0x" << std::hex << (int)frame->at(i);
	}

	std::cout << std::dec << std::endl;

	if (it != frame->end()) {
		// rewind first two bytes which contains frame size and service ID
		it += 2;

		const uint8_t did_lsb = *it;
		it++;

		const uint8_t did_msb = *it;
		it++;

		this->didResponse.did = did_lsb | (did_msb << 8);

		const uint8_t size_byte = *it;

		const bool is_string = ((size_byte & 0x80) == 0) ? true : false;

		const bool is_float = ((size_byte & 0xC0) == 0xC0) ? true : false;

		if (is_string) {
			std::string content;

			this->didResponse.firstSize = DIDRESPONSE_DATASIZE_STRING;
			this->didResponse.secondSize = DIDRESPONSE_DATASIZE_EMPTY;
			this->didResponse.thirdSize = DIDRESPONSE_DATASIZE_EMPTY;

			do {
				content.push_back(*it);

			}while (++it != frame->end());

			memset(this->didResponse.first.str, 0x00, DID_RESPONSE_MAX_STRING);
			strncpy(this->didResponse.first.str, content.c_str(), content.size());

		}
		else if (is_float) {

			it++;

			const uint8_t amount_of_data = (size_byte & 0x3);
			uint32_t * float_ptr = (uint32_t*)&generic_float;

			if (amount_of_data >= 1) {

				*float_ptr = *it++;
				*float_ptr |= ((*it++) << 8);
				*float_ptr |= ((*it++) << 16);
				*float_ptr |= ((*it++) << 24);

				this->didResponse.firstSize = DIDRESPONSE_DATASIZE_FLOAT;
				this->didResponse.first.f = generic_float;
			}

			generic_float = 0.0f;

			if (amount_of_data >= 2) {
				*float_ptr = *it++;
				*float_ptr |= ((*it++) << 8);
				*float_ptr |= ((*it++) << 16);
				*float_ptr |= ((*it++) << 24);

				this->didResponse.secondSize = DIDRESPONSE_DATASIZE_FLOAT;
				this->didResponse.second.f = generic_float;
			}
			else {
				this->didResponse.secondSize = DIDRESPONSE_DATASIZE_EMPTY;
			}

			generic_float = 0.0f;

			if (amount_of_data >= 3) {
				*float_ptr = *it++;
				*float_ptr |= ((*it++) << 8);
				*float_ptr |= ((*it++) << 16);
				*float_ptr |= ((*it++) << 24);

				this->didResponse.thirdSize = DIDRESPONSE_DATASIZE_FLOAT;
				this->didResponse.third.f = generic_float;
			}
			else {
				this->didResponse.thirdSize = DIDRESPONSE_DATASIZE_EMPTY;
			}

		}
		else {
			it++;

			const uint8_t first_size = size_byte & 0x03;
			const uint8_t second_size = (size_byte & 0x0C) >> 2;
			const uint8_t third_size = (size_byte & 0x30) >> 4;

			switch (first_size) {
			case 1:	// int8_t
				generic_8 = *it++;

				this->didResponse.firstSize = DIDRESPONSE_DATASIZE_INT8;
				this->didResponse.first.i8 = generic_8;

				break;
			case 2:	// int16_t
				generic_16 = *it++;
				generic_16 |= ((*it++) << 8);

				this->didResponse.firstSize = DIDRESPONSE_DATASIZE_INT16;
				this->didResponse.first.i16 = generic_16;

				break;
			case 3:	// int32_t
				generic_32 = *it++;
				generic_32 |= ((*it++) << 8);
				generic_32 |= ((*it++) << 16);
				generic_32 |= ((*it++) << 24);

				this->didResponse.firstSize = DIDRESPONSE_DATASIZE_INT32;
				this->didResponse.first.i32 = generic_32;

				break;
			}

			switch (second_size) {
			case 1:	// int8_t
				generic_8 = *it++;

				this->didResponse.secondSize = DIDRESPONSE_DATASIZE_INT8;
				this->didResponse.second.i8 = generic_8;

				break;
			case 2:	// int16_t
				generic_16 = *it++;
				generic_16 |= ((*it++) << 8);

				this->didResponse.secondSize = DIDRESPONSE_DATASIZE_INT16;
				this->didResponse.second.i16 = generic_16;

				break;
			case 3:	// int32_t
				generic_32 = *it++;
				generic_32 |= ((*it++) << 8);
				generic_32 |= ((*it++) << 16);
				generic_32 |= ((*it++) << 24);

				this->didResponse.secondSize = DIDRESPONSE_DATASIZE_INT32;
				this->didResponse.second.i32 = generic_32;

				break;
			default:
				this->didResponse.secondSize = DIDRESPONSE_DATASIZE_EMPTY;
			}

			switch (third_size) {
			case 1:	// int8_t
				generic_8 = *it++;

				this->didResponse.thirdSize = DIDRESPONSE_DATASIZE_INT8;
				this->didResponse.third.i8 = generic_8;

				break;
			case 2:	// int16_t
				generic_16 = *it++;
				generic_16 |= ((*it++) << 8);

				this->didResponse.thirdSize = DIDRESPONSE_DATASIZE_INT16;
				this->didResponse.third.i16 = generic_16;

				break;
			case 3:	// int32_t
				generic_32 = *it++;
				generic_32 |= ((*it++) << 8);
				generic_32 |= ((*it++) << 16);
				generic_32 |= ((*it++) << 24);

				this->didResponse.thirdSize = DIDRESPONSE_DATASIZE_INT32;
				this->didResponse.third.i32 = generic_32;

				break;
			default:
				this->didResponse.thirdSize = DIDRESPONSE_DATASIZE_EMPTY;
			}
		}

		// print response
		if (this->didResponse.firstSize != DIDRESPONSE_DATASIZE_EMPTY) {
			std::cout 	<< "I = SrvReadDid::callback, firstSize: "
						<< DidResponse_DataSize_toString(this->didResponse.firstSize)
						<< ", data: ";

			switch (this->didResponse.firstSize) {
				case DIDRESPONSE_DATASIZE_INT8:		std::cout << (int)this->didResponse.first.i8 << std::endl; break;
				case DIDRESPONSE_DATASIZE_INT16:	std::cout << (int)this->didResponse.first.i16 << std::endl; break;
				case DIDRESPONSE_DATASIZE_INT32:	std::cout << (int)this->didResponse.first.i32 << std::endl; break;
				case DIDRESPONSE_DATASIZE_FLOAT:	std::cout << this->didResponse.first.f << std::endl; break;
				case DIDRESPONSE_DATASIZE_STRING:	std::cout << this->didResponse.first.str << std::endl; break;
				default: break;
			}
		}

		if (this->didResponse.secondSize != DIDRESPONSE_DATASIZE_EMPTY) {
			std::cout 	<< "I = SrvReadDid::callback, secondSize: "
						<< DidResponse_DataSize_toString(this->didResponse.secondSize)
						<< ", data: ";

			switch (this->didResponse.secondSize) {
				case DIDRESPONSE_DATASIZE_INT8:		std::cout << (int)this->didResponse.second.i8 << std::endl; break;
				case DIDRESPONSE_DATASIZE_INT16:	std::cout << (int)this->didResponse.second.i16 << std::endl; break;
				case DIDRESPONSE_DATASIZE_INT32:	std::cout << (int)this->didResponse.second.i32 << std::endl; break;
				case DIDRESPONSE_DATASIZE_FLOAT:	std::cout << this->didResponse.second.f << std::endl; break;
				case DIDRESPONSE_DATASIZE_STRING:	std::cout << this->didResponse.second.str << std::endl; break;
				default: break;
			}
		}

		if (this->didResponse.thirdSize != DIDRESPONSE_DATASIZE_EMPTY) {
			std::cout 	<< "I = SrvReadDid::callback, thirdSize: "
						<< DidResponse_DataSize_toString(this->didResponse.thirdSize)
						<< ", data: ";

			switch (this->didResponse.thirdSize) {
				case DIDRESPONSE_DATASIZE_INT8:		std::cout << (int)this->didResponse.third.i8 << std::endl; break;
				case DIDRESPONSE_DATASIZE_INT16:	std::cout << (int)this->didResponse.third.i16 << std::endl; break;
				case DIDRESPONSE_DATASIZE_INT32:	std::cout << (int)this->didResponse.third.i32 << std::endl; break;
				case DIDRESPONSE_DATASIZE_FLOAT:	std::cout << this->didResponse.third.f << std::endl; break;
				case DIDRESPONSE_DATASIZE_STRING:	std::cout << this->didResponse.third.str << std::endl; break;
				default: break;
			}
		}
	}

#if defined (_MSC_VER) && (_MSC_VER <= 1400)
	SetEvent(syncEvent);
#else
	if (conditionVariable != 0x00) {

		pthread_cond_signal(conditionVariable);
	}
#endif
}
