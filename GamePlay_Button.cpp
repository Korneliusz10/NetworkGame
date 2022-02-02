#include "GamePlay_Button.h"

GamePlay_Button::GamePlay_Button(float x, float y, float width, float height, sf::Font* font, std::string text, sf::Color nidleColout, sf::Color nhovercolour) {
	this->shape.setSize(sf::Vector2f(width, height));
	this->shape.setPosition(sf::Vector2f(x, y));
	this->font = font;
	this->text.setFont(*this->font);
	this->text.setString(text);
	this->text.setFillColor(sf::Color::Black);
	this->text.setCharacterSize(24);

	this->text.setPosition(
		this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		this->shape.getPosition().y + (this->shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height / 2.f
	);


	this->idleColour = nidleColout;
	this->hoverColour = nhovercolour;

	this->shape.setFillColor(this->idleColour);
	this->can_be_clicked = true;
}

void GamePlay_Button::render(sf::RenderWindow* window) {
	window->draw(this->shape);
	window->draw(this->text);
}

bool GamePlay_Button::update(sf::Vector2f mousePos) {

	if (can_be_clicked == true) {
		if (this->shape.getGlobalBounds().contains(mousePos)) {
			this->shape.setFillColor(hoverColour);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				//this->shape.setFillColor(sf::Color::Red);
				can_be_clicked = false;
				return true;
			}
			return false;
		}
	}
	else {
		this->shape.setFillColor(idleColour);
	}
	this->shape.setFillColor(idleColour);
}