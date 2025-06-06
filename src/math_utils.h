#ifndef _MATH_UTIL_H_
#define _MATH_UTIL_H_
#include <SFML/Graphics.hpp>
#include <random>

class mu
{
public:
    static int grU(int max) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, max);
        return dis(gen);
    }

    static sf::Vector2u grVec2u(int max_x, int max_y)
    {
        int randomX = grU(max_x);
        int randomY = grU(max_y);
        return sf::Vector2u(randomX, randomY);
    }

    template <typename T>
    static T dist(const sf::Vector2<T>& v1, const sf::Vector2<T>& v2) {
        T dx = v2.x - v1.x;
        T dy = v2.y - v1.y;
        return std::sqrt(dx * dx + dy * dy); // Euclidean distance
    }

    template <typename T>
    static T cheap_dist(const sf::Vector2<T>& v1, const sf::Vector2<T>& v2) {
        T dx = v2.x - v1.x;
        T dy = v2.y - v1.y;
        return dx * dx + dy * dy; // no sqrt
    }

    template <typename T>
    static sf::Vector2<T> limitMag(const sf::Vector2<T>& vec, T maxMagnitude) {
        T magnitude = std::sqrt(vec.x * vec.x + vec.y * vec.y);
        if (magnitude > maxMagnitude) {
            T scale = maxMagnitude / magnitude;
            return sf::Vector2<T>(vec.x * scale, vec.y * scale);
        }
        return vec;
    }

    template <typename T>
    static sf::Vector2<T> setMag(const sf::Vector2<T>& vec, T newMagnitude) {
        T currentMagnitude = std::sqrt(vec.x * vec.x + vec.y * vec.y);
        if (currentMagnitude == 0) {
            return sf::Vector2<T>(0, 0); // Avoid division by zero
        }
        T scale = newMagnitude / currentMagnitude;
        return sf::Vector2<T>(vec.x * scale, vec.y * scale);
    }

    template <typename T>
    static sf::Vector2<T> mult(const sf::Vector2<T>& vec, T scalar) {
        return sf::Vector2<T>(vec.x * scalar, vec.y * scalar);
    }

};

#endif
