#pragma once
#include <SFML/Graphics.hpp>

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
        shape.setSize(fromPlayer ? sf::Vector2f{ 12.f, 12.f } : sf::Vector2f{ 12.f, 14.f });
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