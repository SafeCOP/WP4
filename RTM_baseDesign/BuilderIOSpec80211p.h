/*
 * IOSpec80211p.h
 *
 *  Created on: 27 ott 2017
 *      Author: user
 */

#ifndef BUILDERIOSPEC80211P_H_
#define BUILDERIOSPEC80211P_H_

#include <string>
#include "BuilderIOCreator.h"

//using namespace spec {
//	class BuilderIOSpec80211p;
//}

class BuilderIOSpec80211p : public BuilderIOCreator {
public:
	BuilderIOSpec80211p();
	virtual ~BuilderIOSpec80211p();

	std::string* getData() { return data; }
	std::string* getMetadata() { return metadata; }

protected:
private:
	std::string data[3];
	std::string metadata[4];
};



#endif /* BUILDERIOSPEC80211P_H_ */
