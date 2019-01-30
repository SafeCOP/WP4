/*
 * BuilderIOCreator.h
 *
 *  Created on: 30 ott 2017
 *      Author: user
 */

#ifndef BUILDERIOCREATOR_H_
#define BUILDERIOCREATOR_H_

#include <string>
#include <map>

/**
 * in questa classe vengono definiti tutti i dati generici dell'IO
 * */
class BuilderIOCreator {
public:
	BuilderIOCreator();
	virtual ~BuilderIOCreator();

	virtual void openChannel()=0;
	virtual void closeChannel()=0;
	virtual bool isOpen()=0;
	//bool insert()=0;

protected:
	std::map<int,std::string*> queuePriority;
private:
};



#endif /* BUILDERIOCREATOR_H_ */
