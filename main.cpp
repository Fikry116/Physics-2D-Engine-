#include <SFML/Graphics.hpp>
#include <iostream>
#include "solver.hpp"
#include "renderer.hpp"

static sf::Color ColorProcedural(float dt) {
    float r = sin(dt);
    float g = sin(dt + .333f * 2.f * Utils::PI);
    float b = sin(dt + .666f * 2.f * Utils::PI);

    return sf::Color(static_cast<uint8_t>(255.f * r * r),
                     static_cast<uint8_t>(255.f * g * g),
                     static_cast<uint8_t>(255.f * b * b));
}

int main() {
    sf::RenderWindow window(sf::VideoMode({1000, 1000}), "GG" );
    Solver mySolver;
    Renderer myRenderer(window);
    
    const int max_object = 500;
    const float min_radius = 15.f;
    const float max_radius = 25.f;
    
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 1;
    window.setFramerateLimit(120);

    sf::Vector2f constraint_size ={static_cast<float>(window.getSize().x/2), static_cast<float>(window.getSize().y/2)};
    sf::Clock clock;
    clock.start();

    mySolver.SetConstraint(constraint_size, 350.f);
    mySolver.SetSimulation(120);
    mySolver.SetSubStep(4);

    while(window.isOpen()) {
        while(auto eventOpt = window.pollEvent()) {
            auto &event = *eventOpt;
            if(auto *resize = event.getIf<sf::Event::Resized>()) {
                sf::View view(sf::FloatRect({0.f, 0.f},
                             {static_cast<float>(resize->size.x), static_cast<float>(resize->size.y)}));
              window.setView(view); //mengatur ukuran baru window.))
            }

            if(event.is<sf::Event::Closed>()) {
                window.close();
            }

            if(auto *keyboard = event.getIf<sf::Event::KeyPressed>()) {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::R)) {
                    mySolver.ClearObject();
                }
            }

        } //akhir polEvent()

        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
            if(mySolver.GetObjectCount() < max_object && clock.getElapsedTime().asSeconds() > 0.085) {
                clock.restart();
                // for(int i = 0; i < 3; i++) {
                    float radius = Utils::RandomFloat(min_radius, max_radius);
                    VerletObject& object = mySolver.AddObject(static_cast<sf::Vector2f>(mouse_pos), radius);
                    float t = mySolver.GetTime();
                    const float angle  = t * std::sin(t) + Utils::PI * 0.5f;
                    mySolver.setObjectVelocity(object, sf::Vector2f{std::cos(angle), std::sin(angle)});
                    object.m_color = ColorProcedural(t);
                // }
            }
        }
        
        window.clear(sf::Color(100, 116, 139));
        mySolver.Update();
        myRenderer.render(mySolver);
        window.display();
    }
}