#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Utils.h"
#include <iostream>

struct VerletObject {
    sf::Vector2f m_current_position;
    sf::Vector2f m_old_position;
    sf::Vector2f acceleration = {0.f, 1000.f};
    float m_radius = 10.f;
    sf::Color m_color = sf::Color::White;

    VerletObject(sf::Vector2f current_position_, float radius_) 
        : m_current_position(current_position_),
          m_radius(radius_) {}

    void Update(float dt) {
        const sf::Vector2f displacement = m_current_position - m_old_position;

        m_old_position = m_current_position;
        m_current_position = m_current_position + displacement + acceleration * (dt*dt);
        acceleration = {};
    }

    void Accelerate(sf::Vector2f v) {
        acceleration += v;
    }

    void SetVelocity(sf::Vector2f v, float dt) {
        m_old_position = m_current_position + v * dt;
    }

    void AddVelocity(sf::Vector2f v, float dt) {
        m_old_position -= v * dt;
    }

    [[nodiscard]]
    sf::Vector2f GetVelocity(float dt) {
        return (m_current_position - m_old_position) / dt;
    }

    sf::Vector2f GetPosition() {
        return m_current_position;
    }

    float GetRadius() {
        return m_radius;
    }

    

 
};

class Solver {
private:
    uint32_t                  m_sub_steps          = 1;
    sf::Vector2f              m_gravity            = {0.0f, 100.0f};
    sf::Vector2f              m_constraint_center;
    float                     m_constraint_radius  = 400.0f;
    std::vector<VerletObject> m_objects;
    float                     m_time               = 0.0f;
    float                     m_frame_dt           = 0.0f;
    
    void ApplyConstraint() {
        for(auto &obj : m_objects) {
            sf::Vector2f v = m_constraint_center - obj.GetPosition();
            float distance = Utils::Magnitude(v);
    
            if(distance > (m_constraint_radius - obj.GetRadius())) {
                std::cout << "before normal\n";
                sf::Vector2f normal = Utils::normalize(v);
                std::cout << "normal\n";
                obj.m_current_position = m_constraint_center - normal * (m_constraint_radius - obj.GetRadius());
            }
        }
    }
    
    void ApplyGravity() {
        for(auto &obj : m_objects) {
            obj.Accelerate(m_gravity);
        }
    }

    void CheckCollision() {
        for(auto &obj1 : m_objects) {
            for(auto &obj2 : m_objects) {
                sf::Vector2f delta_pos = obj1.GetPosition() - obj2.GetPosition();
                float distance = Utils::MagnitudeSqrd(delta_pos);
                float min_distance = obj1.GetRadius() + obj2.GetRadius();
                std::cout << delta_pos.x << " " << delta_pos.y;


                if (distance < (min_distance)) {
                    std::cout << "bef normal collision\n";
                    sf::Vector2f normal = Utils::normalize(delta_pos);
                    std::cout << "normal collision\n";
                    float overlap = min_distance - distance;
                    obj1.m_old_position = obj1.m_current_position;
                    obj1.m_current_position += normal * (overlap/2);

                    obj2.m_old_position = obj2.m_current_position;
                    obj2.m_current_position += normal * (overlap/2);

                }
            }
        }
    }

    void UpdateObject(float dt) {
        for(auto& obj : m_objects) {
            obj.Update(dt);
        }
    }
    
public:
    Solver() = default;
    
    VerletObject& AddObject(sf::Vector2f pos, float radius) {
        return m_objects.emplace_back(pos, radius);
    }

    void SetSubStep(int sub_step) {
        m_sub_steps = sub_step;
    }

    void SetSimulation(float dt) {
        m_frame_dt = 1.f / dt;
    }

    [[nodiscard]]
    sf::Vector3f getConstraint() const
    {
        return {m_constraint_center.x, m_constraint_center.y, m_constraint_radius};
    }

    [[nodiscard]]
    const std::vector<VerletObject>& getObjects() const
    {
        return m_objects;
    }


    uint32_t GetObjectCount() {
        return m_objects.size();
    }

    [[nodiscard]]
    float GetStepDt() const
    {
        return m_frame_dt / static_cast<float>(m_sub_steps);
    }


    void SetConstraint(sf::Vector2f position, float radius) {
        m_constraint_center = position;
        m_constraint_radius = radius;
    }

    
    float GetTime() {
        return m_time;
    }

    void Update() {
        m_time += m_frame_dt;

        for(int i = 0; i < m_sub_steps; i++) {
            ApplyGravity();
            CheckCollision();
            ApplyConstraint();
            UpdateObject(GetStepDt());
        }
    }
};