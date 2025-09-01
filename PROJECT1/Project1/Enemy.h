#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Bullet.h"
#include <random>

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