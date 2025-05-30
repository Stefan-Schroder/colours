#ifndef _QUAD_TREE_H_
#define _QUAD_TREE_H_

#include <vector>
#include <SFML/Graphics.hpp>

#define CAPACITY 100

template <typename T>
class QuadTree {
public:
    QuadTree(sf::Vector2u, sf::Vector2u);
    ~QuadTree();

    void Insert(sf::Vector2u, T);

private:
    sf::Vector2u top_left;
    sf::Vector2u bottom_right; // up to, not including

    std::vector<T> items;

    QuadTree<T>* Contained(sf::Vector2u position);

    QuadTree<T>* NW = nullptr;
    QuadTree<T>* NE = nullptr;
    QuadTree<T>* SE = nullptr;
    QuadTree<T>* SW = nullptr;
};
#endif
