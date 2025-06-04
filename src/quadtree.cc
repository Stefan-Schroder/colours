#include "quadtree.h"

// template <typename T>
// QuadTree<T>::~QuadTree()
// {
//     for (auto qNode : this->quadNodes)
//     {
//         delete qNode;
//     }
// }

QuadTree::QuadTree(Box boundary, Iterator begin, Iterator end)
{
    quadNodes.push_back(qNode(boundary));
    quadNodes[0].build(this, begin, end, 0);
}

std::vector<Particle*> QuadTree::Query(sf::Vector2f point, float distance)
{
    std::vector<Particle*> particles;
    // std::cout << "searching.." << std::endl;
    this->quadNodes[0].CollectParticles(this, particles, point, distance);
    // std::cout << "Found " << particles.size() << " particles" << std::endl;
    return particles;
}

void QuadTree::Draw(sf::RenderWindow& window)
{
    for (auto node : this->quadNodes)
    {
        Box nodeb = node.getBoundary();
        sf::RectangleShape shape;
        shape.setSize(sf::Vector2f(nodeb.br() - nodeb.tl()));
        shape.setPosition(nodeb.tl().x, nodeb.tl().y);
        shape.setOutlineColor(sf::Color::White);
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineThickness(1.0f);
        window.draw(shape);
    }
}

QuadTree::qNode::qNode(Box box): boundary(box)
{}

void QuadTree::qNode::build(QuadTree* qt, Iterator begin, Iterator end, node_id parent_id)
{
    this->begin_data = begin;
    this->end_data = end;

    if (this->end_data - this->begin_data <= CAPACITY)
    { return; }

    auto new_boxes = this->boundary.subdivide();

    auto mid_point = this->boundary.center();

    auto y_split = std::partition(begin,end,
    [&mid_point](Particle* const p)
    {
        return p->GetPosition().y < mid_point.y;
    });

    auto Top_LR_split = std::partition(begin,y_split,
    [&mid_point](Particle* const p)
    {
        return p->GetPosition().x < mid_point.x;
    });

    auto Bottom_LR_split = std::partition(y_split,end,
    [&mid_point](Particle* const p)
    {
        return p->GetPosition().x < mid_point.x;
    });

    node_id child_0 = qt->quadNodes.size();
    qt->quadNodes.push_back(new_boxes[0]);
    qt->quadNodes[parent_id].children[0] = child_0;

    node_id child_1 = qt->quadNodes.size();
    qt->quadNodes.push_back(new_boxes[1]);
    qt->quadNodes[parent_id].children[1] = child_1;

    node_id child_2 = qt->quadNodes.size();
    qt->quadNodes.push_back(new_boxes[2]);
    qt->quadNodes[parent_id].children[2] = child_2;

    node_id child_3 = qt->quadNodes.size();
    qt->quadNodes.push_back(new_boxes[3]);
    qt->quadNodes[parent_id].children[3] = child_3;

    // Debug output
    // std::cout << "Inside " << parent_id << std::endl;
    // std::cout << "TL " << child_0 << " is getting: " << top_left.size() << " parts" << std::endl;
    // std::cout << "TR " << child_1 << " is getting: " << top_right.size() << " parts" << std::endl;
    // std::cout << "BL " << child_2 << " is getting: " << bottom_left.size() << " parts" << std::endl;
    // std::cout << "BR " << child_3 << " is getting: " << bottom_right.size() << " parts" << std::endl;

    // Recursively build child nodes
    qt->quadNodes[child_0].build(qt, this->begin_data, Top_LR_split, child_0);
    qt->quadNodes[child_1].build(qt, Top_LR_split, y_split, child_1);
    qt->quadNodes[child_2].build(qt, y_split, Bottom_LR_split, child_2);
    qt->quadNodes[child_3].build(qt, Bottom_LR_split, this->end_data, child_3);
}

void QuadTree::qNode::CollectParticles(QuadTree* qt, std::vector<Particle*>& parts, sf::Vector2f origin, float distance)
{
    switch (this->boundary.CheckCoverage(origin, distance))
    {
        case Box::NONE:
            // std::cout << "No coverage... thats crazy" << std::endl;
            return;
        case Box::PARTUAL:
            if (this->children[0] != null)
            {
                // std::cout << "partial going deaper" << std::endl;
                qt->quadNodes[this->children[0]].CollectParticles(qt, parts, origin, distance);
                qt->quadNodes[this->children[1]].CollectParticles(qt, parts, origin, distance);
                qt->quadNodes[this->children[2]].CollectParticles(qt, parts, origin, distance);
                qt->quadNodes[this->children[3]].CollectParticles(qt, parts, origin, distance);
            }
            else
            {
                // std::cout << "partial leaf" << std::endl;
                auto iter = this->begin_data;
                while (iter != this->end_data)
                {
                    if (mu::cheap_dist(origin, (*iter)->GetPosition()) < distance*distance)
                    {
                        parts.push_back(*iter);
                    }
                    iter++;
                }
            }
            break;
        case Box::FULLY:
            // std::cout << "fully covered that" << std::endl;
            auto iter = this->begin_data;
            while (iter != this->end_data)
            {
                parts.push_back(*iter);
                iter++;
            }
            break;
    }
}
