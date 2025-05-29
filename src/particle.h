#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include <iostream>
#include <SFML/System.hpp>
#include <list>
#include "math_utils.h"

#define MAX_SPEED 200

class Particle
{
public:
    Particle(sf::Vector2f);

    void Update(float, sf::Vector2u, std::vector<Particle*>, bool);

    sf::Vector2f GetPosition() { return this->position; }
    sf::Vector2f GetVelocity() { return this->velocity; }

    void Reset(sf::Vector2f);

    bool isAlive() { return this->is_alive; }

private:
    sf::Vector2f AlignVelocity(std::vector<Particle*>);
    sf::Vector2f CohesionVelocity(std::vector<Particle*>);
    sf::Vector2f SeparationVelocity(std::vector<Particle*>);

    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;

    bool is_alive = true;
};

#endif
