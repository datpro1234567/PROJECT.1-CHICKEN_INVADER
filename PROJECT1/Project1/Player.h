#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Bullet.h"

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