void showInsertChampionshipScreen(){
	_championship championship;
	int equal;
	do{
		clearCanvas();
		printCenter("Cadastrar Campeonatos", 7);
		gotoxy(10, 10);printf("Codigo: %d", getChampionshipsQuantity()+1);
		gotoxy(25, 10);printf("Nome do Campeonato: ");
		gotoxy(10, 12);printf("Responsavel: ");
		gotoxy(60, 12);printf("Ano: ");
		gotoxy(10, 14);printf("Jogo: ");
		gotoxy(10, 16);printf("Descricao: ");
		
		do{
			readString(championship.name, 45, 10, 30);
			if(exaustiveSearchChampionshipByName(championship.name) == -1)
				showToast("Campeonato ja cadastrado!", TOAST_WARNING);
		}while(exaustiveSearchChampionshipByName(championship.name) == -1);
				
		if(stricmp(championship.name, "\0") != 0 ){
			removeToast();
			
			do{
				readString(championship.organizer, 25, 12, 30);
				if(stricmp(championship.organizer, "\0") == 0)
					showToast("Responsável nao pode ser nulo!", TOAST_WARNING);
			}while(stricmp(championship.organizer, "\0") == 0);
			
			removeToast();
			
			do{
				championship.year = readInt(65, 12, 4);
				if(championship.year < 1000 || championship.year > 9999)
					showToast("Ano deve ter quatro dígitos!", TOAST_WARNING);
			}while(championship.year < 1000 || championship.year > 9999);
			
			char gameName[30];
		
			do{
				readString(gameName, 16, 14, 30);
				if(exaustiveSearchGameByName(gameName) == -1)
					showToast("Jogo nao registrado!", TOAST_WARNING);
			}while(exaustiveSearchGameByName(gameName) == -1);
		
			championship.game = exaustiveSearchGameByName(gameName);
			
			do{
				readString(championship.description, 3, 17, 74);
				if(stricmp(championship.description, "\0") == 0)
					showToast("Descricao nao pode ser nula!", TOAST_WARNING);
			}while(stricmp(championship.description, "\0") == 0);
		
			if(insertChampionship(championship))
				showToast("Cadastrado com sucesso!", TOAST_SUCCESS);
			else
				showToast("Erro no cadastro!", TOAST_ERROR);
		}
	}while(stricmp(championship.name, "\0") != 0);
}

void showChampionship(int addr){
	_championship championship = getChampionship(0, addr);
	printCenter("Exibir Campeonato", 7);
	gotoxy(10, 10); printf("Codigo: %d", championship.code);
	gotoxy(25, 10); printf("Nome do Campeonato: %s", championship.name);
	getch();
}

void showUpdateChampionshipScreen(int addr){
	_championship championship = getChampionship(0, addr);
	printCenter("Alterar Campeonato", 7);
	gotoxy(10, 10); printf("Codigo: %d", championship.code);
	gotoxy(25, 10); printf("Nome do Campeonato: ");
	int equal;
	char temp[20];
	strcpy(temp, championship.name);
	do{
		readString(temp, 45, 10, 30, 1);
		equal = exaustiveSearchChampionshipByName(temp);
		if(equal != -1 && equal != addr){
			showToast("Jogo ja cadastrado!", TOAST_WARNING);
			strcpy(temp, championship.name);
		}
			
	}while(equal != -1 && equal != addr);
	strcpy(championship.name, temp);
	if(updateChampionship(addr, championship))
		showToast("Alterado com sucesso", TOAST_SUCCESS);
	else
		showToast("Erro na alteração", TOAST_ERROR);
}

void showRemoveChampionshipScreen(int addr){
	printCenter("Remover Campeonato", 7);
	menu removeMenu = setMenu(10);
	char title[20];
	strcpy(title, "Deseja mesmo remover ");
	strcat(title, getChampionship(0, addr).name);
	strcat(title, "?");
	printCenter(title, 9);
	addMenuOption(removeMenu, "Sim");
	addMenuOption(removeMenu, "Cancelar");
	switch(showMenu(removeMenu)){
		case 0:
			if(removeChampionship(addr))
				showToast("Removido com sucesso!", TOAST_SUCCESS);
			else
				showToast("Erro na remoção!", TOAST_ERROR);
	}
}

void showConsultChampionshipScreen(){
	int addr;
	char name[30];
	_championship championship;
	int searchOther=1;
	int option;
	do{
		clearCanvas();
		printCenter("Consultar Campeonatos", 7);
		gotoxy(20, 10);printf("Pesquisar: ");
		if(searchOther)
			readString(name, 31, 10, 30);
		else{
			gotoxy(31, 10);printf("%s", name);
		}
		addr = exaustiveSearchChampionshipByName(name);
		if(addr != -1 && stricmp(name, "\0") != 0){
			searchOther = 0;
			removeToast();
			menu championshipMenu = setMenu(12);
			addMenuOption(championshipMenu, "Mudar pesquisa");
			addMenuOption(championshipMenu, "Exibir");
			addMenuOption(championshipMenu, "Alterar");
			addMenuOption(championshipMenu, "Remover");
			addMenuOption(championshipMenu, "Competidores", 0);
			option = showMenu(championshipMenu);
			clearCanvas();
			switch(option){
				case 0:
					searchOther = 1;
					break;
				case 1:
					showChampionship(addr);
					break;
				case 2:
					showUpdateChampionshipScreen(addr);
					break;
				case 3:
					showRemoveChampionshipScreen(addr);
					searchOther=1;
			}
		}
		else if(addr == -1){
			showToast("Campeonato nao encontrado!", TOAST_ERROR);
			searchOther = 1;
		}
	}while(stricmp(name, "\0") != 0);
}

showChampionshipsMainMenu(){
	printCenter("Gerenciar Campeonatoes", 7);
	menu mainMenu = setMenu(9);	
	int option, enableOut;
	do{
		clearMenuOptions(mainMenu);
		enableOut = getGamesQuantity() ? 1 : 0;
		addMenuOption(mainMenu, "Cadastrar Campeonatoes");
		addMenuOption(mainMenu, "Consultar Campeonatoes", enableOut);
		addMenuOption(mainMenu, "Voltar");
		option = showMenu(mainMenu);
		clearCanvas();
		switch(option){
			case 0:
				showInsertChampionshipScreen();
				break;
			case 1:
				showConsultChampionshipScreen();
				break;
		}
		clearCanvas();
	}while(option != 2);
}
