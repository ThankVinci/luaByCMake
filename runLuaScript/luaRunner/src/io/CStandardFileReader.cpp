#include "io/CStandardFileReader.h"

CStandardFileReader::CStandardFileReader() {
	pf = NULL;
}

int CStandardFileReader::readAll(const char* filePath) {
	pf = fopen(filePath, "r");
	if (pf == NULL) return EXIT_FAILURE;
	fseek(pf, 0, SEEK_END);
	readBufferSize = ftell(pf);
	fseek(pf, 0, SEEK_SET);
	readBuff = (char*)malloc(sizeof(char) * (readBufferSize + 1));
	if (readBuff == NULL) return EXIT_FAILURE;
	readBufferSize = fread(readBuff, 1, readBufferSize, pf);
	readBuff[readBufferSize] = '\0';
	return EXIT_SUCCESS;
}