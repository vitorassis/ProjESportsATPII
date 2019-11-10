int persistNewGame(FILE *file, _game game){
	if(checkFileOpened(file)){
		fwrite(&game, sizeof(game), 1, file);
		return 1;
	}
	return 0;
}

_game getNextGame(FILE *file){
	_game buffer;
	buffer.code = 0;
	if(checkFileOpened(file)){
		fread(&buffer, sizeof(_game), 1, file);
	}
	return buffer;
}

_game getAddressedGame(FILE *file, int from, int address){
	_game buffer;
	buffer.code = 0;
	if(checkFileOpened(file)){
		setFileCursor(file, from, address);
		fread(&buffer, sizeof(_game), 1, file);
	}
	return buffer;
}

int updateGameRegistry(FILE *file, int address, _game game){
	if(checkFileOpened(file)){
		setFileCursor(file, 0, address);
		fwrite(&game, sizeof(_game), 1, file);
		return 1;
	}
	else
		return 0;
}
