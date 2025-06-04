#ifndef _BOX_H_
#define _BOX_H_

#include <SFML/Graphics.hpp>
#include "math_utils.h"

class Box
{
public:
    Box(sf::Vector2u tl, sf::Vector2u br): top_left(tl), bottom_right(br) {}

    sf::Vector2u tl() { return this->top_left; }
    sf::Vector2u br() { return this->bottom_right; }
    sf::Vector2u bl() { return sf::Vector2u(this->top_left.x, this->bottom_right.y); }
    sf::Vector2u tr() { return sf::Vector2u(this->bottom_right.x, this->top_left.y); }

    sf::Vector2u center()
    {
        return sf::Vector2u (
                (bottom_right.x - top_left.x)/2 + top_left.x,
                (bottom_right.y - top_left.y)/2 + top_left.y
        );
    };

    std::array<Box, 4> subdivide() 
    {
        auto mid_point = this->center();

        Box TL(this->top_left, mid_point);

        Box TR(sf::Vector2u(mid_point.x, top_left.y),
               sf::Vector2u(bottom_right.x, mid_point.y));

        Box BL(sf::Vector2u(top_left.x, mid_point.y),
                sf::Vector2u(mid_point.x, bottom_right.y));

        Box BR(mid_point, bottom_right);

        return {TL, TR, BL, BR};
    };

    enum covered {
        NONE,
        PARTUAL,
        FULLY
    };

    covered CheckCoverage(sf::Vector2f origin, float distance)
    {
        auto cheap_distance = distance*2;

        if (mu::cheap_dist(origin, sf::Vector2f(tl())) < cheap_distance &&
                mu::cheap_dist(origin, sf::Vector2f(tr())) < cheap_distance &&
                mu::cheap_dist(origin, sf::Vector2f(bl())) < cheap_distance &&
                mu::cheap_dist(origin, sf::Vector2f(br())) < cheap_distance)
        {
            return FULLY;
        }

        if (origin.x + distance < tl().x ||
            origin.x - distance > br().x ||
            origin.y + distance < tl().y ||
            origin.y - distance > br().y)
        {
            return NONE;
        }

        return PARTUAL;
    }

private:
    sf::Vector2u top_left;
    sf::Vector2u bottom_right; // up to, not including
};
#endif
