#pragma once

#include <SFML/Graphics.hpp>

class GamePlay_Button
{
public:
	GamePlay_Button(float x, float y, float width, float height, sf::Font* font, std::string text, sf::Color idleColout, sf::Color hovercolour);
	void render(sf::RenderWindow*);
	bool update(sf::Vector2f mousePos);
	sf::Text text;
	int ID;
	sf::RectangleShape shape;
	bool can_be_clicked;
private:
	sf::Font* font;
	

	sf::Color idleColour;
	sf::Color hoverColour;
};
