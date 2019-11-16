#define SUBSCRIPTION_FILE "subscriptions.dat"

struct _subscription{
	int championship; //CHAMPIONSHIP CODE
	int gamer; //GAMER CODE
	char nickname [20];
};

_subscription getNextSubscriptionByChampionship(FILE *, int);
_subscription getNextSubscription(FILE *);
int updateSubscriptionRegistry(FILE *, int, _subscription);
_subscription getAddressedSubscription(FILE *, int, int);
int persistNewSubscription(FILE *, _subscription);

int insertionSortGetBestAddressSubscription(int championshipCode, int gamerCode){
	_subscription sub;
	createFileIfNotExists(SUBSCRIPTION_FILE);
	int ret;
	FILE *file = openReadFile(SUBSCRIPTION_FILE);
	
	sub = getNextSubscription(file);
	while(!isEndFile(file) && sub.championship < championshipCode){
		if(sub.championship < championshipCode)
			sub = getNextSubscription(file);
	}
	if(sub.championship == championshipCode){
		if(sub.gamer < gamerCode){
			sub = getNextSubscription(file);
			while(!isEndFile(file) && sub.gamer < gamerCode && (sub.championship == championshipCode)){
				sub = getNextSubscription(file);
			}
		}
		
	}
	ret = getFileCursor(file, sizeof(_subscription));
	
	int addr = ret;
	setFileCursor(file, SEEK_END, 0);
	int end = getFileCursor(file, sizeof(sub));
	getch();
	while(addr <= end){
		sub = getAddressedSubscription(file, 0, end);
		updateSubscriptionRegistry(file, end+sizeof(sub), sub);
		end-= sizeof(sub);
	}
	closeFile(file);
	return ret;
}


int exaustiveSearchChampionshipSubscriptionByGamerCode(int championshipCode, int gamerCode){
	_subscription sub;
	createFileIfNotExists(SUBSCRIPTION_FILE);
	int ret;
	FILE *file = openReadFile(SUBSCRIPTION_FILE);
	
	sub = getNextSubscriptionByChampionship(file, championshipCode );
	while(!isEndFile(file) && sub.gamer != gamerCode)
	{
		sub = getNextSubscriptionByChampionship(file, championshipCode);
    }
	if(sub.gamer == gamerCode)
		ret = getFileCursor(file, sizeof(_subscription));
	else
		ret = -1;
	closeFile(file);
	return ret;
}

int exaustiveSearchChampionshipSubscriptionByGamerName(int championshipCode, char gamerName[]){
	int gamerCode = getGamer(0, exaustiveSearchGamerByName(gamerName)).code;
	return exaustiveSearchChampionshipSubscriptionByGamerCode(championshipCode, gamerCode);
}

int exaustiveSearchChampionshipSubscriptionByNickname(int championshipCode, char nickname[]){
	_subscription sub;
	int ret=-1;
	FILE *file = openReadFile(SUBSCRIPTION_FILE);
	
	sub = getNextSubscriptionByChampionship(file, championshipCode );
	while(!isEndFile(file) && (stricmp(nickname, sub.nickname)!=0))
		sub = getNextSubscriptionByChampionship(file, championshipCode );
	if(stricmp(nickname, sub.nickname)==0)
		ret = getFileCursor(file, sizeof(_subscription));
		
	closeFile(file);
	return ret;
}

int insertSubscription(_subscription new_subscription){
	createFileIfNotExists(SUBSCRIPTION_FILE);

	FILE *file = openReadFile(SUBSCRIPTION_FILE);
	int position = insertionSortGetBestAddressSubscription(new_subscription.championship, new_subscription.gamer);
	updateSubscriptionRegistry(file, position, new_subscription);
	closeFile(file);
	return 1;
}

int getSubscriptionsQuantity(int championshipCode){
	FILE *file;
	createFileIfNotExists(SUBSCRIPTION_FILE);
	file = openReadFile(SUBSCRIPTION_FILE);
	int quantity = 0;
	_subscription sub;
	sub.championship = -1;
	
	do{
		sub = getNextSubscriptionByChampionship(file, championshipCode );
		if(sub.championship == championshipCode)
			quantity++;
	}while(!isEndFile(file) && sub.championship != championshipCode);
	
	
	closeFile(file);
	
	return quantity;
}
