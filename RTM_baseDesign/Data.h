/*
 * Data.h
 *
 *  Created on: 30 ott 2017
 *      Author: user
 */

#ifndef DATA_H_
#define DATA_H_

#include <string>

class Data {
public:
	Data() {}
	virtual ~Data() {}
protected:
private:
	/*
	 * type=Quantity(Q), Command(CMD), Response(Res),Acknowledge(ACK)
	 * Q = {Speed, Temperature, Status,…}?? per me è un int.
	 * CMD = {Set Speed, Get Speed, Set Temperature, Get Temperature, Set Status, Get Status,…}
	 * */
	std::string type;//sarà una classe
};
