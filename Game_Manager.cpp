#include "Game_Manager.h"


Game_Manager::Game_Manager() {

	this->state = GAMESTATE::MAINMENU;
}

void Game_Manager::switch_state(GAMESTATE nstate) {
	state = nstate;
}