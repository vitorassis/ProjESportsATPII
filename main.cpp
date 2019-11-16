#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "modules/modules.cpp"
#include "interface/interface.cpp"

int main(){
	setCanvas('/', 1, 1);
	drawCanvas();	
	showTitle("Gerenciador de Campeonatos de e-Sports");
	
		
	menu mainMenu = setMenu(9);
	addMenuOption(mainMenu, "Gerenciar Jogos");
	addMenuOption(mainMenu, "Gerenciar Jogadores");
	addMenuOption(mainMenu, "Gerenciar Campeonatos");
	addMenuOption(mainMenu, "Relatorios", 0);
	addMenuOption(mainMenu, "Sair");
	
	int option;
	
	do{
		printCenter("Menu", 7);
		option = showMenu(mainMenu);
		
		clearCanvas();
		
		switch(option){
			case 0:
				showGamesMainMenu();
				break;
			case 1:
				showGamersMainMenu();
				break;
			case 2:
				showChampionshipsMainMenu();
				break;
		}
		clearCanvas();
	}while(option != 4);
}
