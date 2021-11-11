#pragma once

#include "quadtree.h"

#include <iostream>
#include <cmath>

QuadTree::QuadTree(Point _center, float _r, int _depth)
{
    center = _center;
    r = _r;
    mass = 0;
    depth = ++_depth;
    particleCount = 0;
    divided = false;

    topLeftTree = nullptr;
    topRightTree = nullptr;
    botLeftTree = nullptr;
    botRightTree = nullptr;
}

QuadTree::~QuadTree()
{
    delete topLeftTree, topRightTree, botLeftTree, botRightTree;
}

void QuadTree::insert(Point &p)
{
    if (particleCount > 1)
    {
        createQuad(p);
        points.push_back(p);
    }
    else if (particleCount == 1)
    {
        points.push_back(p);

        for (Point point : points)
        {
            createQuad(point);
        }
        //createQuad(p);
    }
    else if (particleCount == 0)
    {
        points.push_back(p);
    }

    particleCount++;
}

void QuadTree::createQuad(Point &p)
{
    if (!divided)
    {
        subdivide();
    }

    if (p.x > center.x - r && p.x < center.x && p.y > center.y - r && p.y < center.y)
    {
        topLeftTree->insert(p);
    }
    else if (p.x > center.x && p.x < center.x + r && p.y > center.y - r && p.y < center.y)
    {
        topRightTree->insert(p);
    }
    else if (p.x > center.x - r && p.x < center.x && p.y > center.y && p.y < center.y + r)
    {
        botLeftTree->insert(p);
    }
    else if (p.x > center.x && p.x < center.x + r && p.y > center.y && p.y < center.y + r)
    {
        botRightTree->insert(p);
    }
}

void QuadTree::subdivide()
{
    topLeftTree = new QuadTree(Point(center.x - r / 2, center.y - r / 2), r / 2, depth);
    topRightTree = new QuadTree(Point(center.x + r / 2, center.y - r / 2), r / 2, depth);
    botLeftTree = new QuadTree(Point(center.x - r / 2, center.y + r / 2), r / 2, depth);
    botRightTree = new QuadTree(Point(center.x + r / 2, center.y + r / 2), r / 2, depth);

    divided = true;
}

void QuadTree::calculateMassDistribution()
{
    if (particleCount == 1)
    {
        centerOfMass = points[0];
        mass += points[0].mass;
    }
    else
    {
        calculateWeightedMass(topLeftTree);
        calculateWeightedMass(topRightTree);
        calculateWeightedMass(botLeftTree);
        calculateWeightedMass(botRightTree);

        centerOfMass /= mass;
    }
}

void QuadTree::calculateWeightedMass(QuadTree* t)
{
    if (t->particleCount > 0)
    {
        t->calculateMassDistribution();
        mass += t->mass;
        centerOfMass += t->centerOfMass * t->mass;
    }
}

Vec2D QuadTree::calculateAcceleration(Point& p1, Point& p2)
{
    Vec2D acc;

    if (&p1 == &p2)
        return acc;

    const float& x1(p1.x), & y1(p1.y), & m1(p1.mass);
    const float& x2(p2.x), & y2(p2.y), & m2(p2.mass);

    float distance = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + soft);

    if (distance > 0)
    {
        float k = G * m2 / (distance * distance);

        acc.x += k * (x2 - x1);
        acc.y += k * (y2 - y1);
    }
    else
    {
        //check for singularity
        acc.x = 0;
        acc.y = 0;
    }

    return acc;
}

Vec2D QuadTree::calculateForce(Point &p)
{
    // calculate the force from the barnes hut tree to the particle p1
    Vec2D acc = calculateTreeForce(p);

    // calculate the force from particles not in the barnes hut tree on particle p
    if (points.size())
    {
        for (Point point : points)
        {
            Vec2D buf = calculateAcceleration(p, point);
            acc += buf;
        }
    }

    return acc;
}

Vec2D QuadTree::calculateTreeForce(Point &p)
{
    Vec2D acc;

    double distance(0), F(0), height(0);

    if (particleCount == 1)
    {
        acc = calculateAcceleration(p, centerOfMass);
    }
    else
    {
        distance = sqrt((p.x - centerOfMass.x) * (p.x - centerOfMass.x) + 
            (p.y - centerOfMass.y) * (p.y - centerOfMass.y));

        height = r*2;

        if (height / distance <= theta)
        {
            F = G * mass / (distance * distance);
            acc.x = F * (centerOfMass.x - p.x);
            acc.y = F * (centerOfMass.y - p.y);
        }
        else
        {
            Vec2D buf;

            if (divided)
            {
                buf = topLeftTree->calculateTreeForce(p);
                acc.x += buf.x;
                acc.y += buf.y;
                buf = topRightTree->calculateTreeForce(p);
                acc.x += buf.x;
                acc.y += buf.y;
                buf = botLeftTree->calculateTreeForce(p);
                acc.x += buf.x;
                acc.y += buf.y;
                buf = botRightTree->calculateTreeForce(p);
                acc.x += buf.x;
                acc.y += buf.y;
            }
        }
    }

    return acc;
}
