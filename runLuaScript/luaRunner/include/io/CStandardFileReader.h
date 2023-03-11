#pragma once
#include "ITextFileReader.hpp"

class CStandardFileReader :public ITextFileReader {
public:
	CStandardFileReader();
	virtual int readAll(const char* filePath);
	~CStandardFileReader() {
		if(pf != NULL) fclose(pf);
	}
protected:
	FILE* pf;
};