#pragma once

#include "SFML-2.6.0/include/SFML/Graphics.hpp"

class Hero
{
public:
	Hero();
	~Hero();

	void Init(std::string textureName, int frameCount, float animationDuration, sf::Vector2f position, float mass);
	void Update(float deltaTime);
	void Jump(float velocity);
	sf::Sprite GetSprite();

private:
	sf::Texture m_Texture;
	sf::Sprite m_Sprite;
	sf::Vector2f m_Position;

	int m_JumpCount = 0;
	float m_Mass;
	float m_velocity;
	const float m_Gravity = 9.80f;
	bool m_Grounded;

	int m_FrameCount;
	float m_AnimationDuration;
	float m_ElapsedTime;
	sf::Vector2i m_SpriteSize;
};

