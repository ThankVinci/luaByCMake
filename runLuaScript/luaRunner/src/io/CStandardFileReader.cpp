#include "CStandardFileReader.h"

CStandardFileReader::CStandardFileReader() {
	pf = NULL;
}

int CStandardFileReader::readAll(const char* filePath) {
	pf = fopen(filePath, "r");
	if (pf == NULL) return EXIT_FAILURE;
	fseek(pf, 0, SEEK_END);
	size_t fileSize = ftell(pf);
	fseek(pf, 0, SEEK_SET);
	readBuff = (char*)malloc(sizeof(char) * (fileSize + 1));
	if (readBuff == NULL) return NULL;
	fileSize = fread(readBuff, 1, fileSize, pf);
	readBuff[fileSize] = '\0';
	return EXIT_SUCCESS;
}