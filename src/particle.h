#pragma once


#include "particleType.h"

struct particle {
    ParticleTypeDetails type = ParticleType::Air;
    sf::Vector2f velocity = sf::Vector2f(0,0);
    sf::Color baseColor = sf::Color::Black;
    sf::Color color = sf::Color::Black;
    bool liquidMovingLeft = false;
    double temp = 20.0;
    double adjustedDensity = 0;
    bool ignition = false;
};