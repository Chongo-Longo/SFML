#include<iostream>
#include"SFML\Graphics.hpp"
#include"SFML\Window.hpp"
#include"SFML\System.hpp"
#include<math.h>
#include<vector>
#include<cstdlib>

using namespace sf;

class Bullet {
	public:
		CircleShape shape;
		Vector2f currVelocity;
		float maxSpeed;

		Bullet(float radius = 5.f)
			: currVelocity(0.f,0.f), maxSpeed(15.f)
		{
			this->shape.setRadius(radius);
			this->shape.setFillColor(Color::Yellow);
		}
};

int main() {
	srand(time(NULL));
	RenderWindow window(VideoMode(800,460),"SampleShooting");
	window.setFramerateLimit(60);

	//Player
	CircleShape player(25.f);
	player.setFillColor(Color::Blue);

	//Bullets
	Bullet b1;

	std::vector<Bullet> bullets;

	//Enemy
	RectangleShape enemy;
	enemy.setFillColor(Color::White);
	enemy.setSize(Vector2f(50.f, 50.f));
	int spawnCounter = 0;

	std::vector<RectangleShape> enemies;

	//Vectors
	Vector2f playerCenter;
	Vector2f mousePosWindow;
	Vector2f aimDir;
	Vector2f aimDirNorm;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		//UPDATE

		//VECTORS
		playerCenter = Vector2f(player.getPosition().x + player.getRadius(), (player.getPosition().y + player.getRadius()));
		mousePosWindow = Vector2f(Mouse::getPosition(window));
		aimDir = mousePosWindow - playerCenter;
		aimDirNorm = aimDir / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));
	
		//PLAYER
		if (Keyboard::isKeyPressed(Keyboard::A))
			player.move(-10.f, 0.f);
		if (Keyboard::isKeyPressed(Keyboard::D))
			player.move(10.f, 0.f);
		if (Keyboard::isKeyPressed(Keyboard::W))
			player.move(0.f, -10.f);
		if (Keyboard::isKeyPressed(Keyboard::S))
			player.move(0.f, 10.f);
		//ENEMIES
		if (spawnCounter < 20)
		{
			spawnCounter++;
		}
		if(spawnCounter >= 20 && enemies.size() < 40)
		{
			enemy.setPosition(Vector2f(rand() % window.getSize().x, rand() % window.getSize().y));
			enemies.push_back(RectangleShape(enemy));

			spawnCounter = 0;
		}

		//SHOOTING
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			b1.shape.setPosition(playerCenter);
			b1.currVelocity = aimDirNorm * b1.maxSpeed;
			
			bullets.push_back(Bullet(b1));
		}
		for (size_t i = 0; i < bullets.size(); i++)
		{
			bullets[i].shape.move(bullets[i].currVelocity);
			//Out of bounds
			if (bullets[i].shape.getPosition().x < 0 || bullets[i].shape.getPosition().x > window.getSize().x
				|| bullets[i].shape.getPosition().y < 0 || bullets[i].shape.getPosition().y > window.getSize().y)
			{
			    bullets.erase(bullets.begin() + i);
						
			}
			else{
				//Enemy collision
				for (size_t k = 0; k < enemies.size(); k++){
					if (bullets[i].shape.getGlobalBounds().intersects(enemies[k].getGlobalBounds())){
						bullets.erase(bullets.begin() + i);
						enemies.erase(enemies.begin() + k);
						break;
					}
				}
			}
		}

		//DRAW
		window.clear();
		
		for (size_t i = 0; i < enemies.size(); i++)
		{
			window.draw(enemies[i]);
		}

		window.draw(player);

		for (size_t i = 0; i < bullets.size(); i++)
		{
			window.draw(bullets[i].shape);
		}

		window.display();
	
	
	}
	
	return 0;
}