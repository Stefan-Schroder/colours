#ifndef _PARTICLE_CONTROLLER_H_
#define _PARTICLE_CONTROLLER_H_

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <list>
#include <random>

#include "particle.h"
#include "math_utils.h"

#include "quadtree.h"
#include "box.h"

#define PERCEPTION 50
#define PARTICLES 5000

class ParticleController
{
public:
    ParticleController(sf::Vector2u);

    ~ParticleController();

    std::vector<Particle*> GetLocal(sf::Vector2f);

    void PhysicsUpdate(float dTime, sf::RenderWindow&, bool);

    void DrawParticles(sf::RenderWindow&);

private:
    std::vector<Particle*> particles;
};
#endif
