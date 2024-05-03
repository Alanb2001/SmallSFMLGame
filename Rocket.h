#pragma once

#include "SFML-2.6.0/include/SFML/Graphics.hpp"

class Rocket
{
public:
	Rocket();
	~Rocket();

	void Init(std::string textureName, sf::Vector2f position, float speed);
	void Update(float deltaTime);
	sf::Sprite GetSprite();

private:
	sf::Texture m_Texture;
	sf::Sprite m_Sprite;
	sf::Vector2f m_Position;
	float m_Speed;
};

