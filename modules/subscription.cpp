#define SUBSCRIPTION_FILE "subscriptions.dat"
#define SUBS_TEMP_FILE "subscriptions_temp.dat"
#include <conio.h>

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
	while(addr <= end){
		sub = getAddressedSubscription(file, 0, end);
		updateSubscriptionRegistry(file, end+sizeof(sub), sub);
		end-= sizeof(sub);
	}
	closeFile(file);
	return ret;
}


int getTotalSubscriptionsQuantity(){
	FILE *file = openReadFile(SUBSCRIPTION_FILE);
	setFileCursor(file, 2, 0);
	int last_addr = getFileCursor(file, sizeof(_subscription));
	closeFile(file);
	return last_addr/sizeof(_subscription);
}


int updateSubscription(_subscription sub, int addr){
	createFileIfNotExists(SUBSCRIPTION_FILE);

	FILE *file = openReadFile(SUBSCRIPTION_FILE);
	updateSubscriptionRegistry(file, addr, sub);
	closeFile(file);
	return 1;
}

void bubbleSortSubscritpionsByChampionshipGame(){
	createFileIfNotExists(SUBSCRIPTION_FILE);
	FILE *file = openReadFile(SUBSCRIPTION_FILE);
	int i, total = getTotalSubscriptionsQuantity();
	_subscription sub, sub1;
	for(i=1; i<total; i++){
		for(int j=0; j < total - i; j++){
			int addr1 = j*sizeof(_subscription);
			int addr2 = (j*sizeof(_subscription)) + sizeof(_subscription);
			_subscription sub = getAddressedSubscription(file, 0, addr1);
			_subscription sub1 = getAddressedSubscription(file, 0, addr2);
			
			int codeFirst = getGame(0, binarySearchGameByCode(getChampionship(0, exaustiveSearchChampionshipByCode(sub.championship)).code)).code;
			int codeSecond = getGame(0, binarySearchGameByCode(getChampionship(0, exaustiveSearchChampionshipByCode(sub1.championship)).code)).code;
			
			if(codeFirst > codeSecond){
				updateSubscription(sub, j*sizeof(_subscription) + sizeof(_subscription));
				updateSubscription(sub1, j*sizeof(_subscription));
			}
		}
	}
	closeFile(file);
}

void selectionSortSubscritpionsByChampionshipCode(){
	createFileIfNotExists(SUBSCRIPTION_FILE);
	FILE *file = openReadFile(SUBSCRIPTION_FILE);
	int j, total = getTotalSubscriptionsQuantity();
	_subscription sub, biggerCodeSub;
	int biggerCode, biggerJ;
	for(int i=1; i<total; i++){
		biggerCode = 0;
		for(j=0; j < total - i; j++){	
			int addr1 = j*sizeof(_subscription);
			_subscription sub = getAddressedSubscription(file, 0, addr1);			
			int code = getChampionship(0, exaustiveSearchChampionshipByCode(sub.championship)).code;
			
			if(code > biggerCode){
				biggerCode = code;
				biggerCodeSub = sub;
				biggerJ = j;
			}
		}
		
		if(biggerJ != j){
			updateSubscription(sub, biggerJ*sizeof(_subscription));
			updateSubscription(biggerCodeSub, (total-i)*sizeof(_subscription));
		}
	}
	closeFile(file);
}


int indexedSearchChampionshipSubscriptionByGamerCode(int championshipCode, int gamerCode){
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

int indexedSearchChampionshipSubscriptionByGamerName(int championshipCode, char gamerName[]){
	int gamerCode = getGamer(0, sentinelSearchGamerByName(gamerName)).code;
	return indexedSearchChampionshipSubscriptionByGamerCode(championshipCode, gamerCode);
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

int removeSubscription(int CHcode, int GMcode){
	createFileIfNotExists(SUBSCRIPTION_FILE);
	FILE *file = openReadFile(SUBSCRIPTION_FILE);
	FILE *temp = openWriteFile(SUBS_TEMP_FILE);
	_subscription sub;
	do{
		sub = getNextSubscription(file);
		if(CHcode != sub.championship || GMcode != sub.gamer)
			persistNewSubscription(temp, sub);
	}while(!isEndFile(file));
	closeFile(file);
	closeFile(temp);
	removeFile(SUBSCRIPTION_FILE);
	renameFile(SUBS_TEMP_FILE, SUBSCRIPTION_FILE);
	return 1;
}

_subscription getSubscription(int next = 1, int address = 0, int from = 0){
	FILE *file;
	file = openReadFile(SUBSCRIPTION_FILE);
	_subscription gotSubscription;
	if(next)
		gotSubscription = getNextSubscription(file);
	else
		gotSubscription = getAddressedSubscription(file, from, address);
	
	closeFile(file);
	
	return gotSubscription;
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

