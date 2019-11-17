void showInsertSubscriptionScreen(_championship championship){
	_subscription subscription;
	subscription.championship = championship.code;
	int equal;
	do{
		subscription.gamer = 0;
		clearCanvas();
		printCenter("Cadastrar Competidores", 7);
		gotoxy(10, 10);printf("Campeonato: %s", championship.name);
		gotoxy(40, 10);printf("Competidor: ");//52
		gotoxy(10, 12);printf("Nickname: "); //20
		
		char gamerName[50];
		int found, valid;
		do{
			valid = 0;
			readString(gamerName, 52, 10, 25);
			if(stricmp(gamerName, "\0") == 0 )
				valid = 1;
			else{
				if(sentinelSearchGamerByName(gamerName) != -1){
					found = indexedSearchChampionshipSubscriptionByGamerName(championship.code, gamerName);
					if(found == -1)
						valid = 1;
					else
						showToast("Jogador ja inscrito nesse campeonato!", TOAST_WARNING);
				}
				else
					showToast("Jogador nao existe!", TOAST_WARNING);
			}
		}while(!valid);
				
		if(stricmp(gamerName, "\0") != 0  ){
			removeToast();
			
			subscription.gamer = getGamer(0, sentinelSearchGamerByName(gamerName)).code;
			
			do{
				readString(subscription.nickname, 20, 12, 20);
				found = exaustiveSearchChampionshipSubscriptionByNickname(championship.code, subscription.nickname);
				
				if(stricmp(subscription.nickname, "\0") == 0)
					showToast("Nickname nao pode ser nulo!", TOAST_WARNING);
				if(found != -1)
					showToast("Nickname ja utilizado nesse campeonato!", TOAST_WARNING);					
			}while(stricmp(subscription.nickname, "\0") == 0 || found != -1);
			
			if(insertSubscription(subscription))
				showToast("Cadastrado com sucesso!", TOAST_SUCCESS);
			else
				showToast("Erro no cadastro!", TOAST_ERROR);
		}
	}while(subscription.gamer != 0);
}

void showSubscription(_championship championship, _subscription sub){
	printf("\nGM code: %d", sub.gamer);
	int addrGamer = binarySearchGamerByCode(sub.gamer);
	_gamer gamer = getGamer(0, addrGamer);
	printf("\nGM code: %d",gamer.code);
	clearCanvas();
	printCenter("Cadastrar Competidores", 7);
	gotoxy(10, 10);printf("Campeonato: %s", championship.name);
	gotoxy(40, 10);printf("Competidor: %s", gamer.name);
	gotoxy(10, 12);printf("Nickname: %s", sub.nickname);
	getch();
}

void showConsultSubscriptionScreen(_championship championship, int method){
	int addr;
	char nick[30];
	_subscription subscription;
	int searchOther=1;
	int option;
	do{
		clearCanvas();
		printCenter("Consultar Inscricoes", 7);
		printCenter(championship.name, 8);
		printCenter(method? "Buscar por Nickname" : "Buscar por Nome", 9);
		gotoxy(20, 11);printf("Pesquisar: ");
		if(searchOther)
			readString(nick, 31, 11, 30);
		else{
			gotoxy(31, 11);printf("%s", nick);
		}
		if(method)
			addr = exaustiveSearchChampionshipSubscriptionByNickname(championship.code, nick);
		else{
			addr = sentinelSearchGamerByName(nick);
			if(addr != -1){
				addr = indexedSearchChampionshipSubscriptionByGamerName(championship.code, nick);
			}
		}
		
		if(addr != -1 && stricmp(nick, "\0") != 0){
			_subscription sub = getSubscription(0, addr);
			searchOther = 0;
			removeToast();
			menu subsMenu = setMenu(12);
			addMenuOption(subsMenu, "Mudar pesquisa");
			addMenuOption(subsMenu, "Exibir");
			addMenuOption(subsMenu, "Alterar");
			addMenuOption(subsMenu, "Remover");
			option = showMenu(subsMenu);
			clearCanvas();
			switch(option){
				case 0:
					searchOther = 1;
					break;
				case 1:
					showSubscription(championship, sub);
					break;
				case 2:
				//	showUpdateSubscriptionScreen(sub);					
					break;
				case 3:
				//	showRemoveSubscriptionScreen(sub);
					searchOther=1;
			}
		}
		else if(addr == -1){
			showToast("Inscricao nao encontrada!", TOAST_ERROR);
			searchOther = 1;
		}
	}while(stricmp(nick, "\0") != 0);
}

void showSelectConsultMethodSubscriptionScreen(_championship championship){
	int option;
	menu methodMenu = setMenu(10);
	addMenuOption(methodMenu, "Nome do Jogador");
	addMenuOption(methodMenu, "Nickname no Campeonato");
	addMenuOption(methodMenu, "Voltar");
	do{
		clearCanvas();
		printCenter("Buscar inscricoes:", 6);
		printCenter(championship.name, 7);
		printCenter("Pesquisar inscricoes por:", 8);
		
		option = showMenu(methodMenu);
		if(option != 2)
			showConsultSubscriptionScreen(championship, option);
	}while(option != 2);
}

void showSubscriptionsMainMenu(int championshipAddr){
	_championship championship = getChampionship(0, championshipAddr);
	menu mainMenu = setMenu(10);	
	int option, enableOut;
	do{
		printCenter("Gerenciar Inscricoes", 7);
		printCenter(championship.name, 8);
		clearMenuOptions(mainMenu);
		enableOut = getSubscriptionsQuantity(championship.code) ? 1 : 0;
		addMenuOption(mainMenu, "Cadastrar Participantes");
		addMenuOption(mainMenu, "Consultar Participantes", enableOut);
		addMenuOption(mainMenu, "Voltar");
		option = showMenu(mainMenu);
		clearCanvas();
		switch(option){
			case 0:
				showInsertSubscriptionScreen(championship);
				break;
			case 1:
				showSelectConsultMethodSubscriptionScreen(championship);
				break;
		}
		clearCanvas();
	}while(option != 2);
}
