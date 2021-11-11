#pragma once

#include "point.h"
#include "vec2D.h"

#include <math.h>
#include <vector>

const float G = 0.1;//0.0000000000667430;
const float theta = 0.8;
const float soft = 0.1;

class QuadTree
{
public:
    Point center;
    Point centerOfMass;
    float r;
    float mass;   
    int depth;
    int particleCount;  
    bool divided;

    std::vector<Point> points;

    QuadTree* topLeftTree;
    QuadTree* topRightTree;
    QuadTree* botLeftTree;
    QuadTree* botRightTree;

    QuadTree(Point _center, float _r, int _depth);
    ~QuadTree();

    void insert(Point& p);
    void createQuad(Point& p);
    void subdivide();

    void calculateMassDistribution();
    void calculateWeightedMass(QuadTree* t);

    Vec2D calculateAcceleration(Point& p1, Point& p2);
    Vec2D calculateForce(Point& p);
    Vec2D calculateTreeForce(Point& p);
};
    
