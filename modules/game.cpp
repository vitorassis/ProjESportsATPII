#define GAME_FILE "games.dat"

struct _game{
	int code;
	char name [30];
	
	int active;
};

int persistNewGame(FILE*, _game);
_game getNextGame(FILE *);
_game getAddressedGame(FILE *, int, int);
int updateGameRegistry(FILE *, int, _game);
FILE *openWriteFile(const char[]);
FILE *openAppendFile(const char[]);
FILE *openReadFile(const char[]);
int checkFileOpened(FILE *);
int closeFile(FILE *);
void createFileIfNotExists(const char[]);
int isEndFile(FILE *);
int getFileCursor(FILE *, int);


int exaustiveSearchGameByName(char name[]){
	FILE *file;
	
	file = openReadFile(GAME_FILE);
	_game game;
	int ret;
	
	while(!isEndFile(file) && stricmp(game.name, name) != 0)
		game = getNextGame(file);
	
	if(isEndFile(file))
		ret= -1;
	else
		ret= getFileCursor(file, sizeof(_game));
	closeFile(file);
	
	return ret;
}

int isRemoved(_game game){
	return !game.active;
}

_game getGame(int next = 1, int address = 0, int from = 0){
	FILE *file;
	file = openReadFile(GAME_FILE);
	_game gotGame;
	if(next)
		gotGame = getNextGame(file);
	else
		gotGame = getAddressedGame(file, from, address);
	
	closeFile(file);
	
	if(isRemoved(gotGame))
		gotGame.code = 0;
	
	return gotGame;
}

int insertGame(_game new_game){
	new_game.active = 1;
	createFileIfNotExists(GAME_FILE);
	if(exaustiveSearchGameByName(new_game.name) == -1)
	{
		FILE *file = openAppendFile(GAME_FILE);
		persistNewGame(file, new_game);
		closeFile(file);
		return 1;
	}
	else
		return 0;
}

int updateGame(int address, _game game){
	FILE *file;
	createFileIfNotExists(GAME_FILE);
	file = openReadFile(GAME_FILE);
	closeFile(file);
	return updateGameRegistry(file, address, game);
}

int removeGame(int address){
	FILE *file;
	file = openReadFile(GAME_FILE);
	_game game = getAddressedGame(file, 0, address);
	game.active = 0;
	int ret = updateGame(address, game);
	closeFile(file);
	
	return ret;
}
