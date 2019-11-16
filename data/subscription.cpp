int persistNewSubscription(FILE *file, _subscription subscription){
	if(checkFileOpened(file)){
		fwrite(&subscription, sizeof(subscription), 1, file);
		return 1;
	}
	return 0;
}

_subscription getNextSubscription(FILE *file){
	_subscription buffer;
	
	if(checkFileOpened(file)){
		fread(&buffer, sizeof(_subscription), 1, file);
	}
	return buffer;
}

_subscription getNextSubscriptionByChampionship(FILE *file, int championshipCode){
	_subscription buffer;
	buffer.championship = 0;
	buffer.gamer = 0;
	if(checkFileOpened(file)){
		
		do{
			buffer = getNextSubscription(file);
		}while(!isEndFile(file) && buffer.championship != championshipCode);
		if(buffer.championship != championshipCode){
			buffer.championship = 0;
			buffer.gamer = 0;
		}
		
	}
	return buffer;
}

_subscription getAddressedSubscription(FILE *file, int from, int address){
	_subscription buffer;
	buffer.championship = 0;
	if(checkFileOpened(file)){
		setFileCursor(file, from, address);
		fread(&buffer, sizeof(_subscription), 1, file);
	}
	return buffer;
}

int updateSubscriptionRegistry(FILE *file, int address, _subscription sub){
	if(checkFileOpened(file)){
		setFileCursor(file, 0, address);
		fwrite(&sub, sizeof(sub), 1, file);
		return 1;
	}
	else
		return 0;
}
