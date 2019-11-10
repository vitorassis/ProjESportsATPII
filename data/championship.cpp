int persistNewChampionship(FILE *file, _championship championship){
	if(checkFileOpened(file)){
		fwrite(&championship, sizeof(championship), 1, file);
		return 1;
	}
	return 0;
}

_championship getNextChampionship(FILE *file){
	_championship buffer;
	buffer.code = 0;
	if(checkFileOpened(file)){
		fread(&buffer, sizeof(_championship), 1, file);
	}
	return buffer;
}

_championship getAddressedChampionship(FILE *file, int from, int address){
	_championship buffer;
	buffer.code = 0;
	if(checkFileOpened(file)){
		setFileCursor(file, from, address);
		fread(&buffer, sizeof(_championship), 1, file);
	}
	return buffer;
}

int updateChampionshipRegistry(FILE *file, int address, _championship championship){
	if(checkFileOpened(file)){
		setFileCursor(file, 0, address);
		fwrite(&championship, sizeof(_championship), 1, file);
		return 1;
	}
	else
		return 0;
}
