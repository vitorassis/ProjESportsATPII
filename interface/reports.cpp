void showReportScreen(){
	void debugSubs(void);
	cleanSubscriptionsFile();
	selectionSortSubscritpionsByChampionshipCode();
	bubbleSortSubscritpionsByGamerNameGroupByChampionshipCode();
	char fileName[40];
	strcpy(fileName, "relatorios\\geral.txt");
	FILE *report = fopen(fileName, "w");
	int subAddr = 0;
	int champCode = 0;
	_championship champ;
	_gamer gamer;
	_game game;
	
	_subscription sub = getSubscription(0, subAddr);
	while(sub.championship != 0){
		champ = getChampionship(0, exaustiveSearchChampionshipByCode(sub.championship));
		if(!isChampionshipRemoved(champ)){
			gamer = getGamer(0, indexedSearchGamerByCode(sub.gamer));
			if(!isGamerRemoved(gamer)){
				game = getGame(0, binarySearchGameByCode(champ.game)*sizeof(_game));
				if(!isGameRemoved(game) && champCode != sub.championship){
					if(champCode != 0)
						fprintf(report, "\n-------------------------------------------------------------------------------------------------------------------\n");
					
					champCode = sub.championship;
					
					fprintf(report, "%s", champ.name);
					fprintf(report, "\nResponsável: %s - %d - %s", champ.organizer, champ.year, game.name);
					fprintf(report, "\nDescrição:\n");
					fprintf(report, "%s", champ.description);
					fprintf(report, "\n\nParticipantes (em ordem alfabética):\n");
				}
				fprintf(report, "-%s\n", gamer.name);
			}			
		}
		
		subAddr += sizeof(sub);
		sub = getSubscription(0, subAddr);
	}
	
	closeFile(report);
	system(fileName);

	selectionSortSubscritpionsByChampionshipCode();
	bubbleSortSubscritpionsByGamerCodeGroupByChampionshipCode();
	
	debugSubs();
}
