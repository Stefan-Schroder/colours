#ifndef _QUAD_TREE_H_
#define _QUAD_TREE_H_

#include <vector>
#include <SFML/Graphics.hpp>
#include "box.h"
#include "particle.h"

#define CAPACITY 100

typedef std::vector<Particle*>::iterator Iterator;

class QuadTree
{
    using node_id = std::uint32_t;
    static constexpr node_id null = node_id(-1);

    struct qNode {
    public:
        qNode(Box);

        void build(QuadTree*, Iterator, Iterator, node_id);
        Box getBoundary() { return boundary; };

        void CollectParticles(QuadTree*, std::vector<Particle*>&, sf::Vector2f, float);

    private:
        Box boundary;

        node_id children [4] = {
            null, null,
            null, null
        };

        Iterator begin_data;
        Iterator end_data;
    };

public:
    QuadTree(Box boundary, Iterator begin, Iterator end);
    // ~QuadTree();

    std::vector<Particle*> Query(sf::Vector2f point, float distance);

    void Draw(sf::RenderWindow&);

private:
    std::vector<qNode> quadNodes; // root is always the first qNode
};
#endif
