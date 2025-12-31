#include <SFML/Graphics.hpp>
#include <iostream>
#include "solver.hpp"
#include "renderer.hpp"

static sf::Color ColorProcedural(float dt) {
    float r = sin(dt);
    float g = sin(dt + 0.333f * 2.f * Utils::PI);
    float b = sin(dt + 0.666f * 2.f * Utils::PI);

    return sf::Color(static_cast<uint8_t>(255.f * r * r),
                     static_cast<uint8_t>(255.f * g * g),
                     static_cast<uint8_t>(255.f * b * b));
}



int main() {
    sf::RenderWindow window(sf::VideoMode({1000, 700}), "GG" );
    Solver mySolver;
    Renderer myRenderer(window);
    
    const int max_object = 1000;
    const float min_radius = 5.f;
    const float max_radius = 50.f;
    

    sf::ContextSettings settings;
    settings.antiAliasingLevel = 1;
    window.setFramerateLimit(60);

    sf::Vector2f constraint_size ={static_cast<float>(window.getSize().x/2), static_cast<float>(window.getSize().y/2)};

    mySolver.SetConstraint(constraint_size, 350.f);
    mySolver.SetSimulation(60);




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

        } //akhir polEvent()

        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
            float radius = Utils::RandomFloat(min_radius, max_radius);
            std::cout << "here1\n";
            if(mySolver.GetObjectCount() < max_object) {
                for(int i = 0; i < 3; i++) {
                    VerletObject& object = mySolver.AddObject(static_cast<sf::Vector2f>(mouse_pos), radius);
                    float t = mySolver.GetTime();
                    object.m_color = ColorProcedural(t);
                    std::cout << "here2\n";

                }
            }
        }
        
        mySolver.Update();
        std::cout << "here3\n";
        window.clear(sf::Color(100, 116, 139));
        myRenderer.render(mySolver);
        std::cout << "here4\n";
        window.display();
    }
}