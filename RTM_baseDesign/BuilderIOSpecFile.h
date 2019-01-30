/*
 * BuilderIOSpecFile.h
 *
 *  Created on: 30 ott 2017
 *      Author: user
 */

#ifndef BUILDERIOSPECFILE_H_
#define BUILDERIOSPECFILE_H_
#include <fstream>
#include "BuilderIOCreator.h"

static std::string fileOp = "C:\\Users\\user\\eclipse-workspace\\RunTimeManager\\prova.txt"; //the name file open

class BuilderIOSpecFile : public BuilderIOCreator {

public:
	BuilderIOSpecFile();

	BuilderIOSpecFile(const std::ifstream &file);

	virtual ~BuilderIOSpecFile();

	void openChannel() {file.open(fileOp, std::ios::in | std::ios::binary | std::ios::ate);}
	bool isOpen() { return file.is_open(); }
	void closeChannel() { file.close(); }
	//inserimento di un elemento nella coda. la classificazione vediamo se metterla direttamete qui.
	//bool insert() {
		//int prio = getPriority();
		//queuePriority.insert(&memFile);//type message quantity
		//return 0;
	//}

	void manageFile();

protected:
private:
	//static std::string fileOp = "%USERPROFILE%\eclipse-workspace\RunTimeManager\prova.txt"; //the name file open
	std::ifstream file;

};

#endif /* BUILDERIOSPECFILE_H_ */
