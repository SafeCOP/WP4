/*
 * BuilderIOSpecFile.cpp
 *
 *  Created on: 06 nov 2017
 *      Author: user
 */

#include "BuilderIOSpecFile.h"


void BuilderIOSpecFile::manageFile() {

		std::string content;
		char *ptrMemblock=NULL;
		std::string memFile;
		std::streampos size;

		if(isOpen()) {
			size = file.tellg();
			ptrMemblock = new char[size];
			file.seekg(0, std::ios::beg);
			file.read(ptrMemblock, size);
			file.close();
		}

		memFile = ptrMemblock;
		if(ptrMemblock != NULL) delete[] ptrMemblock;


	}
//TODO
	//inserire la priorit� del messaggio
	//inserire il messaggio in coda

