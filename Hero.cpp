#include "Hero.h"

Hero::Hero()
{
}

Hero::~Hero()
{
}

void Hero::Init(std::string textureName, int frameCount, float animationDuration, sf::Vector2f position, float mass)
{
	m_Position = position;
	m_Mass = mass;
	m_Grounded = false;

	m_FrameCount = frameCount;
	m_AnimationDuration = animationDuration;

	//Load a Texture
	m_Texture.loadFromFile(textureName.c_str());
	m_SpriteSize = sf::Vector2i(92, 126);

	//Create Sprite And Attach A Texture
	m_Sprite.setTexture(m_Texture);
	m_Sprite.setTextureRect(sf::IntRect(0, 0, m_SpriteSize.x, m_SpriteSize.y));
	m_Sprite.setPosition(m_Position);
	m_Sprite.setOrigin(m_SpriteSize.x / 2, m_SpriteSize.y / 2);
}

void Hero::Update(float deltaTime)
{
	//Animate Sprite
	m_ElapsedTime += deltaTime;
	int animationFrame = static_cast<int> ((m_ElapsedTime / m_AnimationDuration) * m_FrameCount) % m_FrameCount;

	m_Sprite.setTextureRect(sf::IntRect(animationFrame * m_SpriteSize.x, 0, m_SpriteSize.x, m_SpriteSize.y));

	//Update Position
	m_velocity -= m_Mass * m_Gravity * deltaTime;

	m_Position.y -= m_velocity * deltaTime;

	m_Sprite.setPosition(m_Position);

	if (m_Position.y >= 768 * 0.75f)
	{
		m_Position.y = 768 * 0.75f;
		m_velocity = 0;
		m_Grounded = true;
		m_JumpCount = 0;
	}
}

void Hero::Jump(float velocity)
{
	if (m_JumpCount < 2)
	{
		m_JumpCount++;

		m_velocity = velocity;
		m_Grounded = false;
	}
}

sf::Sprite Hero::GetSprite()
{
	return m_Sprite;
}
