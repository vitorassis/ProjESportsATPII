#define CHAMPIONSHIP_FILE "championships.dat"

struct _championship{
	int code;
	char name [30];
	char organizer [30];
	int year;
	int game; // GAME CODE
	char description [100];
	
	int active;
};


int persistNewChampionship(FILE*, _championship);
_championship getNextChampionship(FILE *);
_championship getAddressedChampionship(FILE *, int, int);
int updateChampionshipRegistry(FILE *, int, _championship);
int getChampionshipsQuantity();



int isChampionshipRemoved(_championship championship){
	return !championship.active;
}

int exaustiveSearchChampionshipByName(char name[]){
	FILE *file;
	
	createFileIfNotExists(CHAMPIONSHIP_FILE);
	
	file = openReadFile(CHAMPIONSHIP_FILE);
	_championship championship;
	int ret;
	
	while(!isEndFile(file) && (stricmp(championship.name, name) != 0 || isChampionshipRemoved(championship)))
		championship = getNextChampionship(file);
	
	if(isEndFile(file))
		ret= -1;
	else
		ret= getFileCursor(file, sizeof(_championship));
	closeFile(file);
	
	return ret;
}

_championship getChampionship(int next = 1, int address = 0, int from = 0){
	FILE *file;
	file = openReadFile(CHAMPIONSHIP_FILE);
	_championship gotChampionship;
	if(next)
		gotChampionship = getNextChampionship(file);
	else
		gotChampionship = getAddressedChampionship(file, from, address);
	
	closeFile(file);
	
	if(isChampionshipRemoved(gotChampionship))
		gotChampionship.code = 0;
	
	return gotChampionship;
}

int insertChampionship(_championship new_championship){
	new_championship.active = 1;
	new_championship.code = getChampionshipsQuantity() + 1;
	createFileIfNotExists(CHAMPIONSHIP_FILE);

	if(exaustiveSearchChampionshipByName(new_championship.name) == -1)
	{
		FILE *file = openAppendFile(CHAMPIONSHIP_FILE);
		persistNewChampionship(file, new_championship);
		closeFile(file);
		return 1;
	}
	else
		return 0;
}

int updateChampionship(int address, _championship championship){
	FILE *file;
	createFileIfNotExists(CHAMPIONSHIP_FILE);
	file = openReadFile(CHAMPIONSHIP_FILE);
	int ret = updateChampionshipRegistry(file, address, championship);
	closeFile(file);
	
	return ret;
}

int removeChampionship(int address){
	FILE *file;
	file = openReadFile(CHAMPIONSHIP_FILE);
	_championship championship = getAddressedChampionship(file, 0, address);
	championship.active = 0;
	int ret = updateChampionship(address, championship);
	closeFile(file);
	
	return ret;
}

int getChampionshipsQuantity(){
	FILE *file;
	file = openAppendFile(CHAMPIONSHIP_FILE);
	setFileCursor(file, SEEK_END, 0);
	int lastAddress = getFileCursor(file, 0);
	closeFile(file);
	
	return lastAddress / sizeof(_championship);
}
