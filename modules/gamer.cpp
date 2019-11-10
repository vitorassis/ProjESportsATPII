#define GAMER_FILE "gamers.dat"

struct _gamer{
	int code;
	char name [50];
	
	int active;
};


int persistNewGamer(FILE*, _gamer);
_gamer getNextGamer(FILE *);
_gamer getAddressedGamer(FILE *, int, int);
int updateGamerRegistry(FILE *, int, _gamer);
int getGamersQuantity();



int isGamerRemoved(_gamer gamer){
	return !gamer.active;
}

int exaustiveSearchGamerByName(char name[]){
	FILE *file;
	
	createFileIfNotExists(GAMER_FILE);
	
	file = openReadFile(GAMER_FILE);
	_gamer gamer;
	int ret;
	
	while(!isEndFile(file) && (stricmp(gamer.name, name) != 0 || isGamerRemoved(gamer)))
		gamer = getNextGamer(file);
	
	if(isEndFile(file))
		ret= -1;
	else
		ret= getFileCursor(file, sizeof(_gamer));
	closeFile(file);
	
	return ret;
}

_gamer getGamer(int next = 1, int address = 0, int from = 0){
	FILE *file;
	file = openReadFile(GAMER_FILE);
	_gamer gotGamer;
	if(next)
		gotGamer = getNextGamer(file);
	else
		gotGamer = getAddressedGamer(file, from, address);
	
	closeFile(file);
	
	if(isGamerRemoved(gotGamer))
		gotGamer.code = 0;
	
	return gotGamer;
}

int insertGamer(_gamer new_gamer){
	new_gamer.active = 1;
	new_gamer.code = getGamersQuantity() + 1;
	createFileIfNotExists(GAMER_FILE);

	if(exaustiveSearchGamerByName(new_gamer.name) == -1)
	{
		FILE *file = openAppendFile(GAMER_FILE);
		persistNewGamer(file, new_gamer);
		closeFile(file);
		return 1;
	}
	else
		return 0;
}

int updateGamer(int address, _gamer gamer){
	FILE *file;
	createFileIfNotExists(GAMER_FILE);
	file = openReadFile(GAMER_FILE);
	int ret = updateGamerRegistry(file, address, gamer);
	closeFile(file);
	
	return ret;
}

int removeGamer(int address){
	FILE *file;
	file = openReadFile(GAMER_FILE);
	_gamer gamer = getAddressedGamer(file, 0, address);
	gamer.active = 0;
	int ret = updateGamer(address, gamer);
	closeFile(file);
	
	return ret;
}

int getGamersQuantity(){
	FILE *file;
	file = openAppendFile(GAMER_FILE);
	setFileCursor(file, SEEK_END, 0);
	int lastAddress = getFileCursor(file, 0);
	closeFile(file);
	
	return lastAddress / sizeof(_gamer);
}
