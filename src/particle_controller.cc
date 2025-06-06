#include "particle_controller.h"

ParticleController::ParticleController(sf::Vector2u screen_size)
{
    for (size_t i=0; i<PARTICLES; i++)
    {
        Particle* new_part = new Particle(sf::Vector2f(mu::grVec2u(screen_size.x, screen_size.y)));
        particles.push_back(new_part);
    }
}

ParticleController::~ParticleController()
{
    for (Particle* part : this->particles)
    {
        delete part;
    }
    this->particles.clear();
}

std::vector<Particle*> ParticleController::GetLocal(sf::Vector2f position)
{
    std::vector<Particle*> local_particles;

    for (auto part : this->particles)
    {
        if (mu::dist(position, part->GetPosition()) <= PERCEPTION)
        {
            local_particles.push_back(part);
        }
    }
    return local_particles;
}

void ParticleController::PhysicsUpdate(float dTime, sf::RenderWindow& window, bool is_pressed)
{
    QuadTree qt(Box(sf::Vector2f(0,0), sf::Vector2f(window.getSize())), this->particles.begin(), this->particles.end());
    // qt.Draw(window);

    size_t random_part = 2500; //mu::grU(this->particles.size());

    size_t i = 0;

    // std::vector<Particle*> chosen_seen;
    for (auto particle : this->particles)
    {
        // particle->Update(dTime, window.getSize(), GetLocal(particle->GetPosition()), is_pressed);

        auto seen = qt.Query(particle->GetPosition(), PERCEPTION);
        particle->Update(dTime, window.getSize(), seen , is_pressed);

        // if (i == random_part)
        // {
        //     chosen_seen = seen;
        // }

        if (!particle->isAlive())
        {
            unsigned int x,y;
            switch (mu::grU(3))
            {
                case 0: // north
                    x = mu::grU(window.getSize().x);
                    y = 0;
                    break;
                case 1: // south
                    x = mu::grU(window.getSize().x);
                    y = window.getSize().y;
                    break;
                case 2: // east
                    x = window.getSize().x;
                    y = mu::grU(window.getSize().y);
                    break;
                case 3: // west
                    x = 0;
                    y = mu::grU(window.getSize().y);
                    break;
                default:
                    std::cout << "how did we screw that up" << std::endl;
            }
            particle->Reset(sf::Vector2f(x,y));
        }
        i++;
    }

    // sf::CircleShape shape(4.0f);
    // shape.setFillColor(sf::Color(255,0,255,255));
    // shape.setPosition(particles[random_part]->GetPosition());
    // window.draw(shape);

    // sf::CircleShape view(PERCEPTION);
    // view.setOutlineColor(sf::Color::Green);
    // view.setOutlineThickness(2.0f);
    // view.setFillColor(sf::Color::Transparent);
    // view.setPosition(particles[random_part]->GetPosition());
    // view.setOrigin(sf::Vector2f(PERCEPTION, PERCEPTION));
    // window.draw(view);

    // sf::CircleShape pshape(3.0f);
    // pshape.setFillColor(sf::Color(255,255,0,255));
    // for (auto seen_parts : chosen_seen)
    // {
    //     pshape.setPosition(seen_parts->GetPosition());
    //     window.draw(pshape);
    // }
}

void ParticleController::DrawParticles(sf::RenderWindow& window)
{
    sf::CircleShape shape(10.0f);
    shape.setFillColor(sf::Color(255,255,255 ,10));
    shape.setOrigin(5,5);

    // sf::CircleShape shape(1.0f);
    // shape.setFillColor(sf::Color(255,255,255,255));

    for (auto particle : this->particles)
    {
        if (!particle->isAlive())
            continue;

        shape.setPosition(particle->GetPosition());
        window.draw(shape);
    }
}
