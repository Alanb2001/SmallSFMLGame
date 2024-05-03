#include "SFML-2.6.0/include/SFML/Graphics.hpp"
#include "SFML-2.6.0/include/SFML/Audio.hpp"
#include <vector>
#include "Hero.h"
#include "Enemy.h"
#include "Rocket.h"

sf::Vector2f viewSize(1024.0f, 768.0f);
sf::VideoMode videoMode(viewSize.x, viewSize.y);
sf::RenderWindow window(videoMode, "Hello SFMLGame !!!", sf::Style::Default);

void SpawnEnemy();
void Shoot();

bool CheckCollision(sf::Sprite sprite1, sf::Sprite sprite2);
void Reset();

sf::Texture skyTexture;
sf::Sprite skySprite;

sf::Texture backgroundTexture;
sf::Sprite backgroundSprite;

Hero hero;

std::vector<Enemy*> enemies;
std::vector<Rocket*> rockets;

float currentTime;
float previousTime = 0.0f;

int score = 0;
bool gameOver = true;

//Text
sf::Font headingFont;
sf::Text headingText;

sf::Font scoreFont;
sf::Text scoreText;

sf::Text tutorialText;

sf::Music backgroundMusic;

sf::SoundBuffer fireBuffer;
sf::SoundBuffer hitBuffer;

sf::Sound fireSound(fireBuffer);
sf::Sound hitSound(hitBuffer);

void Init()
{
	//Load Sky Texture
	skyTexture.loadFromFile("Assets/Graphics/sky.png");
	backgroundTexture.loadFromFile("Assets/Graphics/bg.png");
	//Set And Attach A Texture To The Sprite
	skySprite.setTexture(skyTexture);
	backgroundSprite.setTexture(backgroundTexture);

	//Load Font
	headingFont.loadFromFile("Assets/Fonts/SnackerComic.ttf");

	//Set Heading Text
	headingText.setFont(headingFont);
	headingText.setString("Tiny Bazooka");
	headingText.setCharacterSize(84);
	headingText.setFillColor(sf::Color::Red);

	sf::FloatRect headingBounds = headingText.getLocalBounds();
	headingText.setOrigin(headingBounds.width / 2, headingBounds.height / 2);
	headingText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.10f));

	scoreFont.loadFromFile("Assets/Fonts/arial.ttf");

	//Set Score Text
	scoreText.setFont(scoreFont);
	scoreText.setString("Scoree: 0");
	scoreText.setCharacterSize(45);
	scoreText.setFillColor(sf::Color::Red);

	sf::FloatRect scoreBounds = scoreText.getLocalBounds();
	scoreText.setOrigin(scoreBounds.width / 2, scoreBounds.height / 2);
	scoreText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.10f));

	//Set Tutorial Text
	tutorialText.setFont(scoreFont);
	tutorialText.setString("Press Down Arrow To Fire And Start Game, Up Arrow To Jump");
	tutorialText.setCharacterSize(35);
	tutorialText.setFillColor(sf::Color::Red);

	sf::FloatRect tutorialBounds = tutorialText.getLocalBounds();
	tutorialText.setOrigin(tutorialBounds.width / 2, tutorialBounds.height / 2);
	tutorialText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.20f));

	hero.Init("Assets/Graphics/heroAnim.png", 4, 1.0f, sf::Vector2f(viewSize.x * 0.25f, viewSize.y * 0.5f), 200);

	srand((int)time(0));

	//Audio
	backgroundMusic.openFromFile("Assets/Audio/bgMusic.ogg");
	backgroundMusic.play();

	hitBuffer.loadFromFile("Assets/Audio/hit.ogg");
	fireBuffer.loadFromFile("Assets/Audio/fire.ogg");
}

void Draw()
{
	window.draw(skySprite);
	window.draw(backgroundSprite);
	window.draw(hero.GetSprite());

	if (gameOver)
	{
		window.draw(headingText);
		window.draw(tutorialText);
	}
	else
	{
		window.draw(scoreText);
	}

	for (Enemy *enemy : enemies)
	{
		window.draw(enemy->GetSprite());
	}

	for (Rocket *rocket : rockets)
	{
		window.draw(rocket->GetSprite());
	}
}

void UpdateInput()
{
	sf::Event event;

	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Up)
			{
				hero.Jump(750.0f);
			}

			if (event.key.code == sf::Keyboard::Down)
			{
				if (gameOver)
				{
					gameOver = false;
					Reset();
				}
				else
				{
					Shoot();
				}
			}
		}

		if (event.key.code == sf::Keyboard::Escape || event.type == sf::Event::Closed)
		{
			window.close();
		}
	}
}

void Update(float deltaTime)
{
	hero.Update(deltaTime);
	currentTime += deltaTime;
	//Spawn Enemies
	if (currentTime >= previousTime + 1.125f)
	{
		SpawnEnemy();
		previousTime = currentTime;
	}

	//Update Enemies
	for (int i = 0; i < enemies.size(); i++)
	{
		Enemy* enemy = enemies[i];

		enemy->Update(deltaTime);

		if (enemy->GetSprite().getPosition().x < 0)
		{
			enemies.erase(enemies.begin() + i);
			delete(enemy);
			gameOver = true;
		}
	}

	//Update Rockets
	for (int i = 0; i < rockets.size(); i++)
	{
		Rocket* rocket = rockets[i];

		rocket->Update(deltaTime);

		if (rocket->GetSprite().getPosition().x > viewSize.x)
		{
			rockets.erase(rockets.begin() + i);
			delete(rocket);
		}
	}

	//Check Collision Between Rocket And Enemies
	for (int i = 0; i < rockets.size(); i++)
	{
		for (int j = 0; j < enemies.size(); j++)
		{
			Rocket* rocket = rockets[i];
			Enemy* enemy = enemies[j];

			if (CheckCollision(rocket->GetSprite(), enemy->GetSprite()))
			{
				hitSound.play();
				score++;
				std::string finalScore = "Score: " + std::to_string(score);
				scoreText.setString(finalScore);
				sf::FloatRect scoreBounds = scoreText.getLocalBounds();
				scoreText.setOrigin(scoreBounds.width / 2, scoreBounds.height / 2);
				scoreText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.10f));

				rockets.erase(rockets.begin() + i);
				enemies.erase(enemies.begin() + j);

				delete(rocket);
				delete(enemy);

				printf("Rocket intersects enemy \n");
			}
		}
	}
}

int main()
{
	//Initialise Game Objects
	
	//sf::RectangleShape rectangle(sf::Vector2f(500.0f, 300.0f));
	//rectangle.setFillColor(sf::Color::Yellow);
	//rectangle.setPosition(viewSize.x / 2, viewSize.y / 2);
	//rectangle.setOrigin(sf::Vector2f(rectangle.getSize().x / 2, rectangle.getSize().y / 2));
	//
	//sf::CircleShape circle(100.0f);
	//circle.setFillColor(sf::Color::Green);
	//circle.setPosition(viewSize.x / 2, viewSize.y / 2);
	//circle.setOrigin(sf::Vector2f(circle.getRadius(), circle.getRadius()));
	//
	//sf::ConvexShape triangle;
	//triangle.setPointCount(3);
	//triangle.setPoint(0, sf::Vector2f(-100.0f, 0.0f));
	//triangle.setPoint(1, sf::Vector2f(0.0f, -100.0f));
	//triangle.setPoint(2, sf::Vector2f(100.0f, 0.0f));
	//triangle.setFillColor(sf::Color(128, 0, 128, 255));
	//triangle.setPosition(viewSize.x / 2, viewSize.y / 2);
	
	sf::Clock clock;

	Init();

	while (window.isOpen())
	{
		//Handle Keyboard Events
		
		//Update Game Objects in the scene
		
		//Update Input

		UpdateInput();

		//Update Game

		sf::Time deltaTime = clock.restart();
		if (!gameOver)
		{
			Update(deltaTime.asSeconds());
		}
		window.clear(sf::Color::Red);

		//Render Game Objects
		//window.draw(rectangle);
		//window.draw(circle);
		//window.draw(triangle);
		
		//Draw Game

		Draw();
		window.display();
	}

	return 0;
}

void SpawnEnemy()
{
	int randLocation = rand() % 3;
	
	sf::Vector2f enemyPos;

	float speed;

	switch (randLocation)
	{
	case 0:
		enemyPos = sf::Vector2f(viewSize.x, viewSize.y * 0.75f);
		speed = -400;
		break;
	case 1:
		enemyPos = sf::Vector2f(viewSize.x, viewSize.y * 0.60f);
		speed = -550;
		break;
	case 2:
		enemyPos = sf::Vector2f(viewSize.x, viewSize.y * 0.40f);
		speed = -650;
		break;
	default:
		printf("Incorrect Y value \n");
		return;
	}

	Enemy* enemy = new Enemy();
	enemy->Init("Assets/Graphics/enemy.png", enemyPos, speed);

	enemies.push_back(enemy);
}

void Shoot()
{
	Rocket* rocket = new Rocket();

	rocket->Init("Assets/Graphics/rocket.png", hero.GetSprite().getPosition(), 400.0f);

	rockets.push_back(rocket);

	fireSound.play();
}

bool CheckCollision(sf::Sprite sprite1, sf::Sprite sprite2)
{
	sf::FloatRect shape1 = sprite1.getGlobalBounds();
	sf::FloatRect shape2 = sprite2.getGlobalBounds();

	if (shape1.intersects(shape2))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Reset()
{
	score = 0;
	currentTime = 0.0f;
	previousTime = 0.0f;
	scoreText.setString("Score: 0");

	for (Enemy *enemy : enemies)
	{
		delete(enemy);
	}
	for (Rocket *rocket : rockets)
	{
		delete(rocket);
	}

	enemies.clear();
	rockets.clear();
}