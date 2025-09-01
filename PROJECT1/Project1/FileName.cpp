#include "Bullet.h"
#include "Player.h"
#include "Enemy.h"

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>


int main() {
    // Lấy độ phân giải màn hình
    auto mode = sf::VideoMode::getDesktopMode();
    const unsigned WIDTH = mode.width;
    const unsigned HEIGHT = mode.height;

    sf::RenderWindow window(mode, "Duck Shooter OOP - SFML 2.6", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    std::random_device rd;
    std::mt19937 gen(rd());

    const int ROWS = 5, COLS = 10;
    float enemySpacingX = static_cast<float>(WIDTH) / (COLS + 2);
    float enemySpacingY = static_cast<float>(HEIGHT) / 12.f;
    std::vector<Enemy> enemies;
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLS; ++c)
            enemies.emplace_back(enemySpacingX + c * enemySpacingX, 120.f + r * enemySpacingY);

    // Player ở dưới cùng
    float playerY = HEIGHT - 100.f;
    Player player(static_cast<float>(WIDTH) / 2.f, playerY);

    std::vector<Bullet> bullets;
    int score = 0;
    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.key.code == sf::Keyboard::Escape)
                window.close();
        }

        player.handleInput(dt, bullets, WIDTH, HEIGHT);

        for (auto& enemy : enemies) enemy.update(dt, bullets, gen);
        for (auto& b : bullets) b.update(dt, HEIGHT);

        // Player bullets -> enemies
        for (auto& b : bullets) {
            if (!b.alive || !b.fromPlayer) continue;
            for (auto& enemy : enemies) {
                if (!enemy.alive) continue;
                if (enemy.shape.getGlobalBounds().intersects(b.shape.getGlobalBounds())) {
                    enemy.alive = false;
                    b.alive = false;
                    score += 10;
                }
            }
        }

        // Enemy bullets -> player
        for (auto& b : bullets) {
            if (!b.alive || b.fromPlayer) continue;
            if (player.shape.getGlobalBounds().intersects(b.shape.getGlobalBounds())) {
                b.alive = false;
                player.lives--;
                if (player.lives <= 0) {
                    window.close();
                }
            }
        }

        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b) { return !b.alive; }), bullets.end());

        window.clear(sf::Color(10, 10, 30));

        window.draw(player.shape);

        window.display();
    }

    return 0;
}

