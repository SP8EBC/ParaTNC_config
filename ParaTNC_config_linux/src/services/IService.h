/*
 * IService.h
 *
 *  Created on: Aug 22, 2022
 *      Author: mateusz
 */

#ifndef SRC_SERVICES_ISERVICE_H_
#define SRC_SERVICES_ISERVICE_H_

#include <vector>
#include <cstdint>

class IService {

public:

	virtual ~IService();

	virtual void callback(const std::vector<uint8_t> & frame) = 0;

};

#endif /* SRC_SERVICES_ISERVICE_H_ */
