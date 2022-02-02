#pragma once

enum GAMESTATE{MAINMENU,GAME,LOOKKINGFOROPONENT};

class Game_Manager
{
public:
	Game_Manager();
	void switch_state(GAMESTATE);

	GAMESTATE state;

private:

	

};

