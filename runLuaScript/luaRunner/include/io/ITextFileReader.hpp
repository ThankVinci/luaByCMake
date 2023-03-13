#pragma once
#include <stdio.h>
#include <stdlib.h>

class ITextFileReader {
public:
	ITextFileReader(){
		readBuff = NULL;
		readBufferSize = 0;
	}
	virtual int readAll(const char* filePath) = 0;
	const char* getData() { return readBuff;  }
	size_t getDataSize() { return readBufferSize; }
	virtual ~ITextFileReader() {
		if(readBuff != NULL) free(readBuff);
	}
protected:
	char* readBuff;
	size_t readBufferSize;
};