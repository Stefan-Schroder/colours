#include "particle.h"

Particle::Particle(sf::Vector2f position): position(position)
{
    this->velocity = sf::Vector2f(sf::Vector2i(mu::grVec2u(100,100)) - sf::Vector2i(49,49));
    this->is_alive = true;
}

void Particle::Update(float dTime, sf::Vector2u bounds, std::vector<Particle*> local_particles, bool is_pressed)
{
    if (!this->is_alive)
        return;

    float separation_force = (is_pressed) ? 150 : 80;

    // auto align_v = mu::limitMag<float>(AlignVelocity(local_particles), 0.05);
    auto align_v = mu::setMag<float>(AlignVelocity(local_particles), 50);
    auto cohesion_v = mu::setMag<float>(CohesionVelocity(local_particles), 100);
    auto separation_v = mu::setMag<float>(SeparationVelocity(local_particles), separation_force);

    this->velocity += cohesion_v;
    this->velocity += align_v;
    this->velocity += separation_v;

    this->velocity = mu::limitMag<float>(this->velocity, MAX_SPEED);

    this->position += this->velocity * dTime;

    // kill
    if (this->position.x > bounds.x 
            || this->position.y > bounds.y 
            || this->position.x < 0 
            || this->position.y < 0)
    {
        // this->is_alive = false;
        if (this->position.x > bounds.x)
            this->position.x = 0;
        if (this->position.x < 0)
            this->position.x = bounds.x;
        if (this->position.y > bounds.y)
            this->position.y = 0;
        if (this->position.y < 0)
            this->position.y = bounds.y;
    }
}

void Particle::Reset(sf::Vector2f position)
{
    this->position = position;
    this->velocity = sf::Vector2f(sf::Vector2i(mu::grVec2u(100,100)) - sf::Vector2i(50,50));
    this->is_alive = true;
}

sf::Vector2f Particle::AlignVelocity(std::vector<Particle*> local_particles)
{
    sf::Vector2f steering;
    for (auto particle : local_particles)
    {
        if (particle == this)
            continue;

        steering += particle->GetVelocity();
    }

    if (local_particles.size() - 1 > 0)
    {
        steering = sf::Vector2f(steering.x/(local_particles.size() - 1),
                                steering.y/(local_particles.size() - 1));
    }

    return mu::setMag<float>(steering, 1);
}

sf::Vector2f Particle::CohesionVelocity(std::vector<Particle*> local_particles)
{
    sf::Vector2f steering;
    for (auto particle : local_particles)
    {
        if (particle == this)
            continue;

        steering += particle->GetPosition();
    }

    if (local_particles.size() - 1 > 0)
    {
        steering = sf::Vector2f(steering.x/(local_particles.size() - 1),
                                steering.y/(local_particles.size() - 1));
        return mu::setMag<float>(steering - this->GetPosition(), 1);
    }

    return sf::Vector2f(0,0);

}

sf::Vector2f Particle::SeparationVelocity(std::vector<Particle*> local_particles)
{
    sf::Vector2f steering;
    for (auto particle : local_particles)
    {
        if (particle == this)
            continue;

        float distance = mu::dist(this->GetPosition(), particle->GetPosition());
        sf::Vector2f difference = this->GetPosition() - particle->GetPosition();
        // steering += sf::Vector2f(250/(1+distance.x), 250/(1+distance.y));
        // steering += sf::Vector2f(1/(1+distance.x), 1/(1+distance.y));
        steering += mu::setMag(difference, 1/distance);
    }

    if (local_particles.size() - 1 > 0)
    {
        steering = sf::Vector2f(steering.x/(local_particles.size() - 1),
                                steering.y/(local_particles.size() - 1));
        return mu::setMag<float>(steering, 1);
    }

    return sf::Vector2f(0,0);

}
