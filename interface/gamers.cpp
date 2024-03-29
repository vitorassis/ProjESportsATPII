void showInsertGamerScreen(){
	_gamer gamer;
	int equal;
	do{
		clearCanvas();
		printCenter("Cadastrar Jogadores", 7);
		gotoxy(10, 10);printf("Codigo: %d", getGamersQuantity()+1);
		gotoxy(30, 10);printf("Nome do Jogador: ");
		
		do{
			readString(gamer.name, 47, 10, 30);
			equal = sentinelSearchGamerByName(gamer.name);
			if(equal != -1 && !isGamerRemoved(getGamer(0, equal)))
				showToast("Jogador ja cadastrado!", TOAST_WARNING);
		}while(equal != -1);
		removeToast();
		if(stricmp(gamer.name, "\0") != 0 ){
			if(insertGamer(gamer))
				showToast("Cadastrado com sucesso!", TOAST_SUCCESS);
			else
				showToast("Erro no cadastro!", TOAST_ERROR);
		}
	}while(stricmp(gamer.name, "\0") != 0);
}

void showGamer(int addr){
	_gamer gamer = getGamer(0, addr);
	printCenter("Exibir Jogador", 7);
	gotoxy(10, 10); printf("Codigo: %d", gamer.code);
	gotoxy(40, 10); printf("Nome do Jogador: %s", gamer.name);
	getch();
}

void showUpdateGamerScreen(int addr){
	_gamer gamer = getGamer(0, addr);
	printCenter("Alterar Jogador", 7);
	gotoxy(10, 10); printf("Codigo: %d", gamer.code);
	gotoxy(30, 10); printf("Nome do Jogador: ");
	int equal;
	char temp[20];
	strcpy(temp, gamer.name);
	do{
		readString(temp, 47, 10, 30, 1);
		equal = sentinelSearchGamerByName(temp);
		if(equal != -1 && equal != addr){
			showToast("Jogo ja cadastrado!", TOAST_WARNING);
			strcpy(temp, gamer.name);
		}
			
	}while(equal != -1 && equal != addr);
	strcpy(gamer.name, temp);
	if(updateGamer(addr, gamer))
		showToast("Alterado com sucesso", TOAST_SUCCESS);
	else
		showToast("Erro na altera��o", TOAST_ERROR);
}

void showRemoveGamerScreen(int addr){
	printCenter("Remover Jogador", 7);
	menu removeMenu = setMenu(10);
	char title[20];
	strcpy(title, "Deseja mesmo remover ");
	strcat(title, getGamer(0, addr).name);
	strcat(title, "?");
	printCenter(title, 9);
	addMenuOption(removeMenu, "Sim");
	addMenuOption(removeMenu, "Cancelar");
	switch(showMenu(removeMenu)){
		case 0:
			if(removeGamer(addr))
				showToast("Removido com sucesso!", TOAST_SUCCESS);
			else
				showToast("Erro na remo��o!", TOAST_ERROR);
	}
}

void showReportGamerScreen(int addr){
	_gamer gamer = getGamer(0, addr);
	_subscription sub;
	bubbleSortSubscritpionsByChampionshipGame();
	char fileName[40];
	strcpy(fileName, "relatorios\\jogadores\\");
	strcat(fileName, gamer.name);
	strcat(fileName, ".txt");
	FILE *report = fopen(fileName, "w");
	int subsAddr = 0;
	int gameCode = 0;
	int champsTotal = 0, champsSubTotal = 0;
	_championship champ;
	_game game;
	
	fprintf(report, "Jogador selecionado: %s", gamer.name);
	sub = getSubscription(0, subsAddr);
	while(sub.championship != 0){
		champ = getChampionship(0, exaustiveSearchChampionshipByCode(sub.championship));
		game = getGame(0, binarySearchGameByCode(champ.game)*sizeof(_game));
		if(sub.gamer == gamer.code && !isGameRemoved(game)){
			if(gameCode != game.code){
				if(gameCode)
					fprintf(report, "\nTotal de campeonatos: %d", champsSubTotal);
				fprintf(report, "\n------------------------------------------------------------------------------------------------------------------- ");
				fprintf(report, "\n-> Jogo: %s",game.name);
				gameCode = game.code;
				champsTotal += champsSubTotal;
				champsSubTotal = 0;
			}
			if(!isChampionshipRemoved(champ)){
				fprintf(report, "\n\t%s -- Respons�vel: %s (%d) -- Nick: %s", champ.name, champ.organizer, champ.year, sub.nickname);
				champsSubTotal++;
			}
		}
		subsAddr += sizeof(sub);
		sub = getSubscription(0, subsAddr);
	}
	if(gameCode){
		fprintf(report, "\nTotal de campeonatos: %d", champsSubTotal);
		champsTotal += champsSubTotal;
	}
	fprintf(report, "\n------------------------------------------------------------------------------------------------------------------- ");
	fprintf(report, "\nTotal de campeonatos do participante: %d", champsTotal);
	closeFile(report);
	selectionSortSubscritpionsByChampionshipCode();
	system(fileName);
}

void showConsultGamerScreen(){
	int addr;
	char name[30];
	_gamer gamer;
	int searchOther=1;
	int option;
	do{
		clearCanvas();
		printCenter("Consultar Jogadores", 7);
		gotoxy(20, 10);printf("Pesquisar: ");
		if(searchOther)
			readString(name, 31, 10, 30);
		else{
			gotoxy(31, 10);printf("%s", name);
		}
		addr = sentinelSearchGamerByName(name);
		if(addr != -1 && stricmp(name, "\0") != 0){
			searchOther = 0;
			removeToast();
			menu gamerMenu = setMenu(12);
			addMenuOption(gamerMenu, "Mudar pesquisa");
			addMenuOption(gamerMenu, "Exibir");
			addMenuOption(gamerMenu, "Alterar");
			addMenuOption(gamerMenu, "Remover");
			addMenuOption(gamerMenu, "Gerar Relatorio");
			addMenuOption(gamerMenu, "Voltar ao menu");
			option = showMenu(gamerMenu);
			clearCanvas();
			switch(option){
				case 0:
					searchOther = 1;
					break;
				case 1:
					showGamer(addr);
					break;
				case 2:
					showUpdateGamerScreen(addr);
					break;
				case 3:
					showRemoveGamerScreen(addr);
					searchOther=1;
					break;
				case 4:
					showReportGamerScreen(addr);
					break;
				case 5:
					strcpy(name, "\0");
			}
		}
		else if(addr == -1){
			showToast("Jogador nao encontrado!", TOAST_ERROR);
			searchOther = 1;
		}
	}while(stricmp(name, "\0") != 0);
}

showGamersMainMenu(){
	printCenter("Gerenciar Jogadores", 7);
	menu mainMenu = setMenu(9);	
	int option, enableOut;
	do{
		clearMenuOptions(mainMenu);
		enableOut = getGamesQuantity() ? 1 : 0;
		addMenuOption(mainMenu, "Cadastrar Jogadores");
		addMenuOption(mainMenu, "Consultar Jogadores", enableOut);
		addMenuOption(mainMenu, "Voltar");
		option = showMenu(mainMenu);
		clearCanvas();
		switch(option){
			case 0:
				showInsertGamerScreen();
				break;
			case 1:
				showConsultGamerScreen();
				break;
		}
		clearCanvas();
	}while(option != 2);
}
