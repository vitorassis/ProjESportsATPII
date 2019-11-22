int getGamesQuantity();

void showGame(int addr){
	_game game = getGame(0, addr);
	printCenter("Exibir Jogo", 7);
	gotoxy(10, 10); printf("Codigo: %d", game.code);
	gotoxy(40, 10); printf("Nome do Jogo: %s", game.name);
	getch();
}

void showUpdateGameScreen(int addr){
	_game game = getGame(0, addr);
	printCenter("Alterar Jogo", 7);
	gotoxy(10, 10); printf("Codigo: %d", game.code);
	gotoxy(30, 10); printf("Nome do Jogo: ");
	int equal;
	char temp[20];
	strcpy(temp, game.name);
	do{
		readString(temp, 44, 10, 30, 1);
		equal = exaustiveSearchGameByName(temp);
		if(equal != -1 && equal != addr){
			showToast("Jogo ja cadastrado!", TOAST_WARNING);
			strcpy(temp, game.name);
		}
			
	}while(equal != -1 && equal != addr);
	strcpy(game.name, temp);
	if(updateGame(addr, game))
		showToast("Alterado com sucesso", TOAST_SUCCESS);
	else
		showToast("Erro na alteração", TOAST_ERROR);
}

void showRemoveGameScreen(int addr){
	printCenter("Remover Jogo", 7);
	menu removeMenu = setMenu(10);
	char title[20];
	strcpy(title, "Deseja mesmo remover ");
	strcat(title, getGame(0, addr).name);
	strcat(title, "?");
	printCenter(title, 9);
	addMenuOption(removeMenu, "Sim");
	addMenuOption(removeMenu, "Cancelar");
	switch(showMenu(removeMenu)){
		case 0:
			if(removeGame(addr))
				showToast("Removido com sucesso!", TOAST_SUCCESS);
			else
				showToast("Erro na remoção!", TOAST_ERROR);
	}
}

void showConsultGameScreen(){
	int addr;
	char name[30];
	_game game;
	int searchOther=1;
	int option;
	do{
		clearCanvas();
		printCenter("Consultar Jogos", 7);
		gotoxy(20, 10);printf("Pesquisar: ");
		if(searchOther)
			readString(name, 31, 10, 30);
		else{
			gotoxy(31, 10);printf("%s", name);
		}
		addr = exaustiveSearchGameByName(name);
		
		if(addr != -1 && stricmp(name, "\0") != 0){
			searchOther = 0;
			removeToast();
			menu gameMenu = setMenu(12);
			addMenuOption(gameMenu, "Mudar pesquisa");
			addMenuOption(gameMenu, "Exibir");
			addMenuOption(gameMenu, "Alterar");
			addMenuOption(gameMenu, "Remover");
			addMenuOption(gameMenu, "Voltar ao menu");
			option = showMenu(gameMenu);
			clearCanvas();
			switch(option){
				case 0:
					searchOther = 1;
					break;
				case 1:
					showGame(addr);
					break;
				case 2:
					showUpdateGameScreen(addr);					
					break;
				case 3:
					showRemoveGameScreen(addr);
					searchOther=1;
					break;
				case 4:
					strcpy(name, "\0");
			}
		}
		else if(addr == -1){
			showToast("Jogo nao encontrado!", TOAST_ERROR);
			searchOther = 1;
		}
	}while(stricmp(name, "\0") != 0);
}

void showInsertGameScreen(){
	_game game;
	int equal;
	do{
		clearCanvas();
		printCenter("Cadastrar Jogos", 7);
		gotoxy(10, 10);printf("Codigo: %d", getGamesQuantity()+1);
		gotoxy(30, 10);printf("Nome do Jogo: ");
		
		do{
			readString(game.name, 44, 10, 30);
			equal = exaustiveSearchGameByName(game.name);
			if(equal != -1 && !isGameRemoved(getGame(0, equal)))
				showToast("Jogo ja cadastrado!", TOAST_WARNING);
		}while(equal != -1);
		removeToast();
		if(stricmp(game.name, "\0") != 0 ){
			if(insertGame(game))
				showToast("Cadastrado com sucesso!", TOAST_SUCCESS);
			else
				showToast("Erro no cadastro!", TOAST_ERROR);
		}
	}while(stricmp(game.name, "\0") != 0);
}

void showGamesMainMenu(){
	printCenter("Gerenciar Jogos", 7);
	menu mainMenu = setMenu(9);	
	int option, enableOut;
	do{
		clearMenuOptions(mainMenu);
		enableOut = getGamesQuantity() ? 1 : 0;
		addMenuOption(mainMenu, "Cadastrar Jogos");
		addMenuOption(mainMenu, "Consultar Jogos", enableOut);
		addMenuOption(mainMenu, "Voltar");
		option = showMenu(mainMenu);
		clearCanvas();
		switch(option){
			case 0:
				showInsertGameScreen();
				break;
			case 1:
				showConsultGameScreen();
				break;
		}
		clearCanvas();
	}while(option != 2);
}
