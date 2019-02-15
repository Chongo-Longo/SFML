#include<iostream>
#include"SFML\Graphics.hpp"
#include"SFML\Window.hpp"
#include"SFML\System.hpp"
#include"SFML\Audio.hpp"
#include<math.h>
#include<vector>
#include<cstdlib>


//Used public variables for quick SFML features learning

using namespace sf;
class Enemy {
public:
	Sprite shape;

	int HP;
	int HPMax;

	Enemy(Texture *texture, Vector2u windowSize)
	{
		this->HPMax = rand() % 4 + 1;
		this->HP = this->HPMax;

		this->shape.setTexture(*texture);

		this->shape.setScale(0.5f, 0.3f);

		this->shape.setPosition(rand() % (int)(windowSize.x - this->shape.getGlobalBounds().width), -shape.getGlobalBounds().height);
	}
};

class Shot {
public:
	Sprite shape;

	Shot(Texture *texture, float x, float y) {

		this->shape.setTexture(*texture);
		this->shape.setScale(0.3f, 0.3f);

		this->shape.setPosition(x, y);
	}
};

class Player {
public:
	Sprite shape;
	int HP;
	int HPMax;


	std::vector<Shot> bullets;

	Player(Texture *texture) {
		this->HPMax = 10;
		this->HP = this->HPMax;


		this->shape.setTexture(*texture);
		this->shape.setScale(0.5f, 0.5f);
		this->shape.setPosition(380.f, 520.f);
	}

};



int main()
{
	srand(time(NULL));

	RenderWindow window(VideoMode(800, 600), "Space shooter!");
	window.setFramerateLimit(60);

	//Font
	Font font;
	font.loadFromFile("Fonts/Fluxgore.otf");

	//Textures
	Texture playerTex;
	playerTex.loadFromFile("Textures/ship1.png");

	Texture enemyTex;
	enemyTex.loadFromFile("Textures/enemy.png");

	Texture shotTex;
	shotTex.loadFromFile("Textures/beam.png");

	Texture backgrTex;
	Sprite backgr;
	backgrTex.loadFromFile("Textures/backgr.png");
	backgr.setTexture(backgrTex);
	backgr.setPosition(0.f,0.f);
	
	//UI
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(20);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(5.f,5.f);

	Text startText;
	startText.setFont(font);
	startText.setCharacterSize(40);
	startText.setColor(Color::White);



	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(60);
	gameOverText.setFillColor(Color::Red);
	gameOverText.setPosition(250.f, window.getSize().y/2-70.f);
	gameOverText.setString("GAME OVER");

	//Player
	int score = 0;
	Player player(&playerTex);
	int shootTimer = 0;
	Text hpText;
	hpText.setFont(font);
	hpText.setCharacterSize(12);
	hpText.setFillColor(Color::White);

	//Enemy
	std::vector<Enemy> enemies;
	int enemySpawnTimer = 0;

	Text enemyHpText;
	enemyHpText.setFont(font);
	enemyHpText.setCharacterSize(12);
	enemyHpText.setFillColor(Color::White);

	//SOUND
	
	//Shot
	SoundBuffer bufferShot;
	bufferShot.loadFromFile("Audio/shot.wav");
	Sound shot;
	shot.setBuffer(bufferShot);
	shot.setVolume(10.f);
	
	//"Explosion"
	SoundBuffer bufferBoom;
	bufferBoom.loadFromFile("Audio/boom.wav");
	Sound boom;
	boom.setBuffer(bufferBoom);
	boom.setVolume(42.f);

	//Hit
	SoundBuffer bufferHit;
	bufferHit.loadFromFile("Audio/hit.wav");
	Sound hit;
	hit.setBuffer(bufferHit);
	hit.setVolume(50.f);

	
	

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		/*while (Mouse::getPosition(window).x > startText.getGlobalBounds().) {
			window.draw(startText);
		}
		*/
		//UPDATE
		if (player.HP > 0)
		{
			//Player
			if (Keyboard::isKeyPressed(Keyboard::W))
				player.shape.move(0.f, -10.f);
			if (Keyboard::isKeyPressed(Keyboard::A))
				player.shape.move(-10.f, 0.f);
			if (Keyboard::isKeyPressed(Keyboard::S))
				player.shape.move(0.f, 10.f);
			if (Keyboard::isKeyPressed(Keyboard::D))
				player.shape.move(10.f, 0.f);



			hpText.setPosition(player.shape.getPosition().x, player.shape.getPosition().y - hpText.getGlobalBounds().height);
			
			//Collision with window
			if (player.shape.getPosition().x <= 0) //Left
				player.shape.setPosition(0.f, player.shape.getPosition().y);
			if (player.shape.getPosition().x >= window.getSize().x - player.shape.getGlobalBounds().width) //Right
				player.shape.setPosition(window.getSize().x - player.shape.getGlobalBounds().width, player.shape.getPosition().y);
			if (player.shape.getPosition().y <= 0) //Top
				player.shape.setPosition(player.shape.getPosition().x, 0.f);
			if (player.shape.getPosition().y >= window.getSize().y - player.shape.getGlobalBounds().height) //Bottom
				player.shape.setPosition(player.shape.getPosition().x, window.getSize().y - player.shape.getGlobalBounds().height);

			//Shoting
			if (shootTimer < 15)
				shootTimer++;


			if (Mouse::isButtonPressed(Mouse::Left) && shootTimer >= 15) {
				
				player.bullets.push_back(Shot(&shotTex, player.shape.getPosition().x + (player.shape.getGlobalBounds().width / 2 - 33),
					player.shape.getPosition().y + 35));
				player.bullets.push_back(Shot(&shotTex, player.shape.getPosition().x + (player.shape.getGlobalBounds().width / 2 + 13),
					player.shape.getPosition().y + 35));
				shot.play();
				shootTimer = 0;
			}

			//Shots 
			for (size_t i = 0; i < player.bullets.size(); i++)
			{
				//Move
				player.bullets[i].shape.move(0.f, -20.f);

				if (player.bullets[i].shape.getPosition().x > window.getSize().x) {
					player.bullets.erase(player.bullets.begin() + i);
					break;
				}
				//Enemy collision

				for (size_t k = 0; k < enemies.size(); k++)
				{
					if (player.bullets[i].shape.getGlobalBounds().intersects(enemies[k].shape.getGlobalBounds()))
					{
						if (enemies[k].HP <= 1)
						{
							score += enemies[k].HPMax;
							enemies.erase(enemies.begin() + k);
							boom.play();
						}
						else
							enemies[k].HP--;


						player.bullets.erase(player.bullets.begin() + i);
						break;
					}
				}
			}

			//Enemy
			if (enemySpawnTimer < 35)
				enemySpawnTimer++;
			if (enemySpawnTimer >= 35) {
				enemies.push_back(Enemy(&enemyTex, window.getSize()));
				enemySpawnTimer = 0;
			}
			for (size_t i = 0; i < enemies.size(); i++)
			{
				enemies[i].shape.move(0.f, 6.f);

				if (enemies[i].shape.getPosition().y >= window.getSize().y)
				{
					enemies.erase(enemies.begin() + i);
					break;
				}

				if (enemies[i].shape.getGlobalBounds().intersects(player.shape.getGlobalBounds()))
				{
					enemies.erase(enemies.begin() + i);

					player.HP--; // PLAYER TAKE DAMAGE
					hit.play();
					break;
				}

			}
			//UI update
			scoreText.setString("SCORE: " + std::to_string(score));
		}
		hpText.setString(std::to_string(player.HP) + "/" + std::to_string(player.HPMax));
		//DRAW
		window.clear();

		//Background
		window.draw(backgr);


		//Player
		window.draw(player.shape);

		//Shots
		for (size_t i = 0; i < player.bullets.size(); i++)
		{
			window.draw(player.bullets[i].shape);
		}
		//Enemy
		for (size_t i = 0; i < enemies.size(); i++)
		{
			enemyHpText.setString(std::to_string(enemies[i].HP) + "/" + std::to_string(enemies[i].HPMax));
			enemyHpText.setPosition(enemies[i].shape.getPosition().x, enemies[i].shape.getPosition().y
				- enemyHpText.getGlobalBounds().height);
			window.draw(enemyHpText);
			window.draw(enemies[i].shape);
		}
		//UI
		std::cout << player.HP;
		window.draw(hpText);
		window.draw(scoreText);
		
		if (player.HP <= 0) {
			window.draw(gameOverText);
		}
		window.display();
	}

	return 0;
}