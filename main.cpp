#include <SFML/Graphics.hpp>
#include <iostream>
#include "solver.hpp"
#include "renderer.hpp"



int main() {
    sf::RenderWindow window(sf::VideoMode({1000, 1000}), "GG" );
    Solver mySolver;
    Renderer myRenderer(window);

    sf::Font font;
    if (!font.openFromFile("ARIAL.ttf")) { 
        std::cerr << "Error loading font" << std::endl;
        return -1;
    }

    sf::Text fpsText(font);
    fpsText.setCharacterSize(20);
    fpsText.setFillColor(sf::Color::Green);
    fpsText.setPosition({10.f, 10.f});

    sf::Clock clock_fps;
    
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    int frameCount = 0;
    
    const int max_object = 1000;
    const float min_radius = 15.f;
    const float max_radius = 25.f;
    
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 1;
    window.setFramerateLimit(120);
    sf::CircleShape circle(50.f);
    sf::Vector2f constraint_size ={static_cast<float>(window.getSize().x/2), static_cast<float>(window.getSize().y/2)};

    circle.setPosition(constraint_size);
    
    sf::Clock clock;
    clock.start();

    mySolver.SetConstraint(constraint_size, 350.f);
    mySolver.SetSimulation(120.f);
    mySolver.SetSubStep(2);

    while(window.isOpen()) {

        sf::Time dt = clock_fps.restart();
        float dtSeconds = dt.asSeconds();

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

        timeSinceLastUpdate += dt;
        frameCount++;

        if (timeSinceLastUpdate.asSeconds() >= 0.5f) {
            float fps = frameCount / timeSinceLastUpdate.asSeconds();
            float ms = (timeSinceLastUpdate.asSeconds() / frameCount) * 1000.0f;

            std::stringstream ss;
            ss << "FPS: " << std::fixed << std::setprecision(0) << fps << "\n"
               << "Frame Time: " << std::setprecision(2) << ms << " ms\n"
               << "Objects: " << mySolver.GetObjectCount();
            
            fpsText.setString(ss.str());

            // Reset counter
            timeSinceLastUpdate = sf::Time::Zero;
            frameCount = 0;
        }


        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            sf::Vector2f mouse_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
            if(mySolver.GetObjectCount() < max_object && clock.getElapsedTime().asSeconds() > 0.025 && mySolver.IsInConstraint(mouse_pos)) {
                clock.restart();
                float radius = Utils::RandomFloat(min_radius, max_radius);
                float t = mySolver.GetTime();
                
                VerletObject& object = mySolver.AddObject(mouse_pos, 5.f);
                mySolver.setObjectVelocity(object, -mySolver.SetSpeedWhenSpawn(mouse_pos) * 3.f);
                object.m_color = sf::Color::Transparent;
            }
        }


        window.clear(sf::Color(100, 116, 139));
        window.draw(fpsText);
        mySolver.Update();
        myRenderer.render(mySolver);
        window.display();
    }
}