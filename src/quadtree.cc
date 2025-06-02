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

QuadTree::qNode::qNode(Box box): boundary(box)
{}

void QuadTree::qNode::build(QuadTree* qt, Iterator begin, Iterator end, node_id parent_id)
{
    this->begin_data = begin;
    this->end_data = end;

    if (this->end_data - this->begin_data < CAPACITY)
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

    qt->quadNodes.push_back(new_boxes[0]);
    this->children[0] = parent_id + 1;

    qt->quadNodes.push_back(new_boxes[1]);
    this->children[1] = parent_id + 2;

    qt->quadNodes.push_back(new_boxes[2]);
    this->children[2] = parent_id + 3;

    qt->quadNodes.push_back(new_boxes[3]);
    this->children[3] = parent_id + 4;


    // now recursively add the data
    qt->quadNodes[this->children[0]].build(qt, this->begin_data, Top_LR_split, this->children[0]);
    qt->quadNodes[this->children[1]].build(qt, this->begin_data, Top_LR_split, this->children[1]);
    qt->quadNodes[this->children[2]].build(qt, this->begin_data, Top_LR_split, this->children[2]);
    qt->quadNodes[this->children[3]].build(qt, this->begin_data, Top_LR_split, this->children[3]);
}

