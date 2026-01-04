#pragma once
#include <SFML/Graphics.hpp>
#include "solver.hpp"

class Renderer {
private:
    sf::RenderTarget &m_target;

public:
    Renderer(sf::RenderTarget &target) : m_target(target) {}

    void render(Solver &solver) const {
            const sf::Vector3f constraint = solver.getConstraint();
        sf::CircleShape constraint_background{constraint.z};
        constraint_background.setOrigin({constraint.z, constraint.z});
        constraint_background.setFillColor(sf::Color::Black);
        constraint_background.setPosition({constraint.x, constraint.y});
        constraint_background.setPointCount(32);
        m_target.draw(constraint_background);

        // Render objects
        sf::CircleShape circle{1.0f};
        circle.setPointCount(256);
        circle.setOrigin({1.0f, 1.0f});

        const auto& objects = solver.getObjects();
        for (const auto& obj : objects) {
            circle.setPosition(obj.m_current_position);
            circle.setScale({obj.m_radius, obj.m_radius});
            circle.setFillColor(obj.m_color);
            m_target.draw(circle);
        }
    }

};