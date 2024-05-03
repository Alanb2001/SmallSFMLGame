#include "Rocket.h"

Rocket::Rocket()
{
}

Rocket::~Rocket()
{
}

void Rocket::Init(std::string textureName, sf::Vector2f position, float speed)
{
	m_Speed = speed;
	m_Position = position;

	//Load a Texture
	m_Texture.loadFromFile(textureName.c_str());

	//Create Sprite And Attach A Texture
	m_Sprite.setTexture(m_Texture);
	m_Sprite.setPosition(m_Position);
	m_Sprite.setOrigin(m_Texture.getSize().x / 2, m_Texture.getSize().y / 2);
}

void Rocket::Update(float deltaTime)
{
	m_Sprite.move(m_Speed * deltaTime, 0);
}

sf::Sprite Rocket::GetSprite()
{
	return m_Sprite;
}
