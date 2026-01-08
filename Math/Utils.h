#pragma once
#include <random>
#include <SFML/Graphics.hpp>
#include <cmath>

namespace Utils {
    std::random_device rd;
    std::mt19937 gen(rd());

    float RandomFloat(float min, float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(gen);
    }

    int RandomInt(int min, int max) {
        std::uniform_int_distribution dist(min, max);
        return dist(gen);
    }

        static constexpr float PI = 3.1415936f;

    static float Dot(sf::Vector2f v1, sf::Vector2f v2) {
        return v1.x*v2.x + v1.y*v2.y;
    }

    static float Magnitude(sf::Vector2f v) {
        return std::sqrtf(v.x*v.x + v.y*v.y);
    }

    static float DistanceTwoPoint(sf::Vector2f u1, sf::Vector2f u2) {
        float x = std::pow(u1.x-u2.x, 2);
        float y = std::pow(u1.y-u2.y, 2);
        return sqrtf(x+y);
    }

    static float MagnitudeSqrd(sf::Vector2f v) {
        return v.x*v.x + v.y*v.y;
    }

    static sf::Vector2f Normalize(sf::Vector2f v) {
        float length = Magnitude(v);
        if(length == 0.f) {
            return sf::Vector2f{0.f, 0.f};
        }
        return {v/length};
    }

    static sf::Color ColorProcedural(float dt) {
    float r = sin(dt);
    float g = sin(dt + .333f * 2.f * Utils::PI);
    float b = sin(dt + .666f * 2.f * Utils::PI);

    return sf::Color(static_cast<uint8_t>(255.f * r * r),
                     static_cast<uint8_t>(255.f * g * g),
                     static_cast<uint8_t>(255.f * b * b));
}


}