int persistNewGamer(FILE *file, _gamer gamer){
	if(checkFileOpened(file)){
		fwrite(&gamer, sizeof(gamer), 1, file);
		return 1;
	}
	return 0;
}

_gamer getNextGamer(FILE *file){
	_gamer buffer;
	buffer.code = 0;
	if(checkFileOpened(file)){
		fread(&buffer, sizeof(_gamer), 1, file);
	}
	return buffer;
}

_gamer getAddressedGamer(FILE *file, int from, int address){
	_gamer buffer;
	buffer.code = 0;
	if(checkFileOpened(file)){
		setFileCursor(file, from, address);
		fread(&buffer, sizeof(_gamer), 1, file);
	}
	return buffer;
}

int updateGamerRegistry(FILE *file, int address, _gamer gamer){
	if(checkFileOpened(file)){
		setFileCursor(file, 0, address);
		fwrite(&gamer, sizeof(_gamer), 1, file);
		return 1;
	}
	else
		return 0;
}
