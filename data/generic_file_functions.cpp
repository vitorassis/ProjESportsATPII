FILE *openWriteFile(const char[]);
FILE *openAppendFile(const char[]);
FILE *openReadFile(const char[]);
int checkFileOpened(FILE *);
int closeFile(FILE *);
void createFileIfNotExists(const char[]);
int isEndFile(FILE *);
int getFileCursor(FILE *, int);
void setFileCursor(FILE *, int, int);
