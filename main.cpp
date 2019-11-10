#include <stdlib.h>
#include <stdio.h>
#include "interface/interface.cpp"
#include "modules/modules.cpp"

int main(){
	_game game;
	game.code = 1;
	strcpy(game.name, "jogo2");
	printf("%d",insertGame(game));
	strcpy(game.name, "jogo2");
	printf("%d",insertGame(game));
	printf("%s", getGame(0, sizeof(game)*2).name);
	updateGame(0, game);
	printf("%s", getGame(0, 0).name);
	
}
