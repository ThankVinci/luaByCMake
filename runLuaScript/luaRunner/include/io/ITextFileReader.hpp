#pragma once
#include <stdio.h>
#include <stdlib.h>

class ITextFileReader {
public:
	ITextFileReader(){
		readBuff = NULL;
	}
	virtual int readAll(const char* filePath) = 0;
	const char* getData() { return readBuff;  }
	virtual ~ITextFileReader() {
		if(readBuff != NULL) free(readBuff);
	}
protected:
	char* readBuff;
};