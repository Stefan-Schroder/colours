#include "quadtree.h"

template <typename T>
QuadTree<T>::QuadTree(sf::Vector2u top_left, sf::Vector2u bottom_right): top_left(top_left), bottom_right(bottom_right)
{}

template <typename T>
QuadTree<T>::~QuadTree()
{
    items.clear();

    if (NW == nullptr)
        return;

    delete this->NW;
    delete this->NE;
    delete this->SE;
    delete this->SW;
}

template <typename T>
QuadTree<T>* QuadTree<T>::Contained(sf::Vector2u position)
{
    if (NW == nullptr)
        return this;

    sf::Vector2i mid_point( (bottom_right.x - top_left.x)/2 + top_left.x,
                            (bottom_right.y - top_left.y)/2 + top_left.y);

    sf::Vector2i difference = mid_point - sf::Vector2i(position);

    if (difference.y < 0)
    {
        if (difference.x < 0)
            return NW;
        else
            return NE;
    }
    else
    {
        if (difference.x < 0)
            return SW;
        else
            return SE;
    }
}

template <typename T>
void QuadTree<T>::Insert(sf::Vector2u position, T item)
{
    if (NW == nullptr)
    {
        if (this->items.size() < CAPACITY)
        {
            this->items.push_back(item);
            return;
        }
        else
        {
            // subdivide
            sf::Vector2u mid_point( (bottom_right.x - top_left.x)/2 + top_left.x,
                                    (bottom_right.y - top_left.y)/2 + top_left.y);

            this->NW = new QuadTree(this->top_left, mid_point);

            this->NE = new QuadTree(sf::Vector2u(mid_point.x, top_left.y),
                                    sf::Vector2u(bottom_right.x, mid_point.y));

            this->SE = new QuadTree(mid_point, bottom_right);

            this->SW = new QuadTree(sf::Vector2u(top_left.x, mid_point.y),
                                    sf::Vector2u(mid_point.x, bottom_right.y));

            for (auto old_item : items)
            {
                Contained(position)->Insert(old_item, position);
            }
        }
    }
    else
    {
        Contained(position)->Insert(item, position);
    }
    
}
