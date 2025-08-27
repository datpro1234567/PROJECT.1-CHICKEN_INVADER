#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <vector>
#include <random>
#include <string>
#include <algorithm>
#include <iostream>

class Bullet {
public:
    sf::RectangleShape shape;
    float speed;
    bool alive;
    bool fromPlayer;

    Bullet(const sf::Vector2f& pos, float speed, bool fromPlayer) {
        this->speed = speed;
        this->fromPlayer = fromPlayer;
        alive = true;
        shape.setSize(fromPlayer ? sf::Vector2f{ 4.f, 12.f } : sf::Vector2f{ 6.f, 14.f });
        auto sz = shape.getSize();
        shape.setOrigin(sf::Vector2f{ sz.x * 0.5f, sz.y * 0.5f });
        shape.setPosition(pos);
        shape.setFillColor(fromPlayer ? sf::Color::Red : sf::Color::White);
    }

    void update(float dt, unsigned windowHeight) {
        if (!alive) return;
        shape.move(sf::Vector2f{ 0.f, speed * dt });
        auto pos = shape.getPosition();
        if (pos.y < 0.f || pos.y > static_cast<float>(windowHeight)) alive = false;
    }
};

class Enemy {
public:
    sf::RectangleShape shape;
    bool alive;
    float shootCooldown;

    Enemy(float x, float y) {
        shape.setSize(sf::Vector2f{ 30.f, 30.f });
        shape.setOrigin(sf::Vector2f{ 15.f, 15.f });
        shape.setPosition(sf::Vector2f{ x, y });
        shape.setFillColor(sf::Color::Yellow);
        alive = true;
        shootCooldown = 1.0f; // nhanh hơn
    }

    void update(float dt, std::vector<Bullet>& bullets, std::mt19937& gen) {
        if (!alive) return;
        shootCooldown -= dt;
        if (shootCooldown <= 0.f) {
            std::uniform_real_distribution<float> r01(0.0f, 1.0f);
            if (r01(gen) < 0.08f) { // tăng xác suất bắn
                bullets.emplace_back(shape.getPosition(), 300.f, false);
            }
            shootCooldown = 0.8f + r01(gen) * 1.5f; // hồi chiêu nhanh hơn
        }
    }
};

class Player {
public:
    sf::RectangleShape shape;
    float speed;
    float shootDelay;
    float shootTimer;
    int lives;

    Player(float x, float y) {
        shape.setSize(sf::Vector2f{ 40.f, 20.f });
        shape.setOrigin(sf::Vector2f{ 20.f, 10.f });
        shape.setPosition(sf::Vector2f{ x, y });
        shape.setFillColor(sf::Color::Green);
        speed = 350.0f;
        shootDelay = 0.25f;
        shootTimer = 0.0f;
        lives = 3;
    }

    void handleInput(float dt, std::vector<Bullet>& bullets, unsigned windowWidth, unsigned windowHeight) {
        auto p = shape.getPosition();

        // Left & Right
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            shape.move(sf::Vector2f{ -speed * dt, 0.f });
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            shape.move(sf::Vector2f{ speed * dt, 0.f });
        }

        // Up & Down
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            shape.move(sf::Vector2f{ 0.f, -speed * dt });
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            shape.move(sf::Vector2f{ 0.f, speed * dt });
        }

        // Giữ player trong màn hình
        p = shape.getPosition();
        if (p.x < 20.f) p.x = 20.f;
        if (p.x > static_cast<float>(windowWidth) - 20.f) p.x = static_cast<float>(windowWidth) - 20.f;
        if (p.y < 20.f) p.y = 20.f;
        if (p.y > static_cast<float>(windowHeight) - 20.f) p.y = static_cast<float>(windowHeight) - 20.f;
        shape.setPosition(p);

        // Shooting
        shootTimer += dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shootTimer >= shootDelay) {
            bullets.emplace_back(shape.getPosition(), -500.f, true);
            shootTimer = 0.f;
        }
    }
};

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
            if (ev.type == sf::Event::Closed ||
                (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape))
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
        for (auto& enemy : enemies) if (enemy.alive) window.draw(enemy.shape);
        for (auto& b : bullets) window.draw(b.shape);

        window.display();
    }

    return 0;
}
