// main.cpp
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <random>
#include <vector>
#include <string>

#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"

// circle–circle (enemy vs bullet)
static bool circleHitsCircle(const sf::CircleShape& a, const sf::CircleShape& b) {
    sf::Vector2f pa = a.getPosition(), pb = b.getPosition();
    float dx = pa.x - pb.x, dy = pa.y - pb.y;
    float rr = a.getRadius() + b.getRadius();
    return (dx * dx + dy * dy) <= rr * rr;
}

// rect–circle (player vs enemy bullet)
static bool rectHitsCircle(const sf::RectangleShape& rect, const sf::CircleShape& circ) {
    auto rb = rect.getGlobalBounds();
    sf::Vector2f c = circ.getPosition();
    float cx = std::max(rb.left, std::min(c.x, rb.left + rb.width));
    float cy = std::max(rb.top, std::min(c.y, rb.top + rb.height));
    float dx = c.x - cx, dy = c.y - cy;
    float r = circ.getRadius();
    return (dx * dx + dy * dy) <= r * r;
}

int main() {
    const unsigned WIN_W = 800, WIN_H = 600;
    sf::RenderWindow window(sf::VideoMode(WIN_W, WIN_H), "Mini Invaders");
    window.setFramerateLimit(120);

    // RNG (shared)
    std::random_device rd;
    std::mt19937 gen(rd());

    // Player (uses your lives field)
    Player player({ WIN_W * 0.5f, WIN_H - 60.f }, 300.f);

    // Enemies (grid)
    std::vector<Enemy> enemies;
    const int COLS = 10, ROWS = 4;
    const float startX = 120.f, startY = 100.f, gapX = 56.f, gapY = 52.f;
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLS; ++c)
            enemies.emplace_back(startX + c * gapX, startY + r * gapY);

    // Bullets (player & enemies share this)
    std::vector<Bullet> bullets;

    // Swarm motion
    float swarmVX = 60.f;
    const float sideMargin = 30.f;
    const float dropOnReverse = 16.f;

    sf::Clock clock;
    while (window.isOpen()) {
        // events
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) window.close();
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
                window.close();
        }

        float dt = clock.restart().asSeconds();

        // player update (your signature wants H then W)
        player.update(dt, bullets, static_cast<float>(WIN_H), static_cast<float>(WIN_W));

        // move enemies as a swarm and detect edges
        bool hitEdge = false;
        for (auto& en : enemies) {
            if (!en.isAlived) continue;
            en.shape.move(swarmVX * dt, 0.f);
            float x = en.shape.getPosition().x;
            if (x < sideMargin || x >(WIN_W - sideMargin))
                hitEdge = true;
        }
        if (hitEdge) {
            swarmVX = -swarmVX;
            for (auto& en : enemies)
                if (en.isAlived) en.shape.move(0.f, dropOnReverse);
        }

        // enemies shoot
        for (auto& en : enemies) en.update(dt, bullets, gen);

        // update bullets
        for (auto& b : bullets) b.update(dt, static_cast<float>(WIN_H));
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b) { return !b.isAlive; }),
            bullets.end());

        // collisions
        // A) player bullets vs enemies
        for (auto& b : bullets) {
            if (!b.isAlive || !b.isPlayer) continue;
            for (auto& en : enemies) {
                if (!en.isAlived) continue;
                if (circleHitsCircle(en.shape, b.shape)) {
                    en.isAlived = false;
                    b.isAlive = false;
                    break;
                }
            }
        }
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b) { return !b.isAlive; }),
            bullets.end());

        // B) enemy bullets vs player (reduce lives)
        for (auto& b : bullets) {
            if (!b.isAlive || b.isPlayer) continue;
            if (rectHitsCircle(player.shape, b.shape)) {
                b.isAlive = false;
                if (player.lives > 0) player.lives -= 1;
            }
        }
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b) { return !b.isAlive; }),
            bullets.end());

        // show lives
        window.setTitle("Mini Invaders  |  Lives: " + std::to_string(player.lives));

        // end condition (simple)
        if (player.lives <= 0) window.close();

        // draw
        window.clear(sf::Color(15, 18, 30));
        for (auto& en : enemies) if (en.isAlived) window.draw(en.shape);
        window.draw(player.shape);
        for (auto& b : bullets) window.draw(b.shape);
        window.display();
    }

    return 0;
}
