void makeDynamicPath(char path[], const char fileName[]){
	strcpy(path, "data\\files\\");
	strcat(path, fileName);
}

FILE *openFile(const char fileName[], const char method[3]){
	char path [40];
	makeDynamicPath(path, fileName);
	return fopen(path, method);
}

FILE *openWriteFile(const char fileName[]){
	return openFile(fileName, "wb+");
}
FILE *openReadFile(const char fileName[]){
	return openFile(fileName, "rb+");
}
FILE *openAppendFile(const char fileName[]){
	return openFile(fileName, "ab+");
}

int checkFileOpened(FILE *file){
	return file != NULL;
}

int closeFile(FILE *file){
	if(checkFileOpened(file)){
		fclose(file);
		return 1;
	}
	return 0;
}

void createFileIfNotExists(const char fileName[]){
	FILE *file;
	file = openReadFile(fileName);
	if(!checkFileOpened(file))
		file = openWriteFile(fileName);
	
	closeFile(file);
}


void setFileCursor(FILE *file, int from, int address){
	fseek(file, address, from);
}

int getFileCursor(FILE *file, int registySize){
	int addr = ftell(file);
	return addr > 0 ? addr - registySize : 0;
}

int isEndFile(FILE *file){
	return feof(file);
}
