#include "Bullet.h"
#include "Enemy.h"
#include "Player.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <vector>

void main()
{
	auto mode = sf::VideoMode::getDesktopMode();
	int width = mode.width;
	int height = mode.height;

	sf::RenderWindow window(mode, "chicken invader", sf::Style::Fullscreen);
	window.setFramerateLimit(60);

	std::random_device rd;
	std::mt19937 gen(rd());

	int row = 6, col = 10;
	float enemySpaceX = static_cast<float>(width) / (col + 5);
	float enemySpaceY = static_cast<float>(height) / (row + 7);

	std::vector<Enemy> enemys;
	float tempY = enemySpaceY;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			enemys.emplace_back(Enemy(enemySpaceX + j * enemySpaceX, enemySpaceY));
		}
		enemySpaceY += tempY;
	}

	Player player(sf::Vector2f(width / 2, height - 100.f), 200.f);

	sf::Clock clock;
	std::vector<Bullet> bullets;
	while (window.isOpen())
	{

		sf::Event ev;
		while (window.pollEvent(ev))
		{
			if (ev.key.code == sf::Keyboard::Escape)
				window.close();
		}

		float dt = clock.restart().asSeconds();
		player.update(dt, bullets, height, width);

		for (Enemy& enemy : enemys)
			enemy.update(dt, bullets, gen);
		for (Bullet& bullet : bullets)
			bullet.update(dt, height);

		for (Bullet& b : bullets)
		{
			if (!b.isAlive || !b.isPlayer)
				continue;
			for (Enemy& e : enemys)
			{
				if (!e.isAlived)
					continue;
				if (e.shape.getGlobalBounds().intersects(b.shape.getGlobalBounds()))
				{
					e.isAlived = false;
					b.isAlive = false;
				}
			}
		}

		for (Bullet& b : bullets)
		{
			if (!b.isAlive || b.isPlayer)
				continue;
			if (player.shape.getGlobalBounds().intersects(b.shape.getGlobalBounds()))
			{
				player.lives -= 1;
				b.isAlive = false;
				if (player.lives == 0)
					window.close();
			}
		}

		bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet& b) { return !b.isAlive; }), bullets.end());

		window.clear(sf::Color(10,10,20));

		window.draw(player.shape);
		for (Enemy e : enemys)
			if(e.isAlived)
				window.draw(e.shape);

		for (Bullet b : bullets)
			window.draw(b.shape);

		window.display();
	}
}