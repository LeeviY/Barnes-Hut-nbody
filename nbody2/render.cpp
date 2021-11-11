#include "render.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <vector>
#include <random>

using std::cout;
using std::endl;

Renderer::Renderer() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "nbody")
{

}

void Renderer::run() {

    #pragma region Generating matrices
    //std::vector<Point> points = generatePoints();
    //points = generatePointsInCircle(WINDOW_WIDTH/2, N, WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
    generatePointsInCircle(100, N/2, 200, 200);
    generatePointsInCircle(100, N/2, 600, 600);
    generateVelocities(200, 200, N/2, Vec2D(10, 0));
    generateVelocities(600, 600, N/2, Vec2D(-10, 0));
    accelerations = std::vector<Vec2D>(N);

    for (Vec2D& a : accelerations)
    {
        a = Vec2D(0, 0);
    }

    points[0] = Point(200, 200);
    points[0].mass = 100;
    velocities[0] = Vec2D(5, 0);

    points[1] = Point(600, 600);
    points[1].mass = 100;
    velocities[1] = Vec2D(-5, 0);

    cout << "Done" << endl;

    //int a;
    //std::cin >> a;

    #pragma endregion

    while (window.isOpen())
    {
        # pragma region Input Handling
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        #pragma endregion

        window.clear();

        //Building a quadtree for points.
        QuadTree root(Point(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_WIDTH, 0);

        for (Point &point : points)
        {
            root.insert(point);
        }

        root.calculateMassDistribution();      

        for (int i = 0;i < N;i++)
        {
            velocities[i] += accelerations[i] / 2;
            //points[i] += velocities[i];
        }
        
        for (int i = 0;i < N;i++)
        {
            accelerations[i] = root.calculateTreeForce(points[i]);    
            //drawLine(points[i].x, points[i].y, points[i].x + velocities[i].x, points[i].y + velocities[i].y);
        }

        drawPoint(points[0].x, points[0].y, 5, sf::Color::Red);
        drawPoint(points[1].x, points[1].y, 5, sf::Color::Blue);

        for (int i = 0;i < N;i++)
        {
            drawPoint(points[i].x, points[i].y, 2, sf::Color::White);

            velocities[i] += accelerations[i] / 2;
            points[i] += velocities[i];
        }

        /*for (int i = 0;i < N;i++)
        {
            Vec2D forceVector = root.calculateTreeForce(points[i]);

            drawPoint(points[i].x, points[i].y, 2, sf::Color::White);
            drawLine(points[i].x, points[i].y, points[i].x + velocities[i].x, points[i].y + velocities[i].y);
            
            velocities[i] += forceVector;
            points[i] += velocities[i];
        }*/

        //drawTree(&root);
        //drawPoint(root.centerOfMass.x, root.centerOfMass.y, 2, sf::Color::Magenta);

        window.display();

        Sleep(1000/fps);
    }
}

void Renderer::generatePoints()
{
    srand(seed);

    for (int i = 0;i < N;i++)
    {
        points.push_back(Point(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT));
    }
}

void Renderer::generatePointsInCircle(float rad, int num, float offX, float offY)
{
    srand(seed);

    for (int i = 0;i < num;i++)
    {
        float r = rad * sqrt((float) rand() / RAND_MAX);
        float t = ((float) rand() / RAND_MAX) * 2 * M_PI;

        Point p(r * cos(t), r * sin(t));

        p.x += offX;
        p.y += offY;

        points.push_back(p);
    }
}

void Renderer::generateVelocities(int centerX, int centerY, int num, Vec2D vOff)
{
    srand(seed);

    /*for (int i = 0;i < N;i++)
    {
        Vec2D v = Vec2D(rand() % WINDOW_WIDTH - WINDOW_WIDTH, rand() % WINDOW_HEIGHT - WINDOW_HEIGHT);
        v.toUnit();
        v.x /= 10;
        v.y /= 10;
        velocities.push_back(v);
    }*/

    

    int s = 3;

    for (int i = 0;i < num ;i++)
    {
        Point p = points[i];

        float dCenter = sqrt((centerX - p.x) * (centerX - p.x) + (centerY - p.y) * (centerY - p.y));

        if (p.x < centerX && p.y < centerY)
        {
            velocities.push_back(Vec2D(0, s));
        }
        if (p.x > centerX && p.y < centerY)
        {
            velocities.push_back(Vec2D(-s, 0));
        }
        if (p.x > centerX && p.y > centerY)
        {
            velocities.push_back(Vec2D(0, -s));
        }
        if (p.x < centerX && p.y > centerY)
        {
            velocities.push_back(Vec2D(s, 0));
        }
        //cout << dCenter << endl;
        //velocities[i] *= (dCenter/200);
        velocities[i] += vOff;
    }
}

void Renderer::drawTree(QuadTree* t)
{
    if (!t->divided)
    {
        if (t->particleCount != 0)
        {
            //drawPoint(t->points[0].x, t->points[0].y, 2, sf::Color::White);
        }
        return;
    }

    std::vector<QuadTree*> trees =
    { t->topLeftTree, t->topRightTree, t->botLeftTree, t->botRightTree };

    for (QuadTree* tree : trees)
    {
        drawRectangle(tree->center.x, tree->center.y, tree->r);
        //drawPoint(tree->centerOfMass.x, tree->centerOfMass.y, tree->mass, sf::Color::Red);

        if (tree->particleCount > 1)
        {
            //drawPoint(tree->centerOfMass.x, tree->centerOfMass.y, tree->mass, sf::Color::Red);
            //drawLine(tree->center.x, tree->center.y, tree->centerOfMass.x, tree->centerOfMass.y);
        }

        drawTree(tree);
    }
}

void Renderer::drawPoint(float x, float y, float r, sf::Color c)
{
    sf::CircleShape circle(r);

    circle.setOrigin(r, r);
    circle.setPosition(x, y);
    circle.setFillColor(c);

    window.draw(circle);
}

void Renderer::drawRectangle(float x, float y, float r)
{
    sf::RectangleShape rect;

    rect.setPosition(x-r, y-r);
    rect.setSize(sf::Vector2f(r*2, r*2));
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(0.5);
    rect.setFillColor(sf::Color::Transparent);

    window.draw(rect);
}

void Renderer::drawLine(float x1, float y1, float x2, float y2)
{
    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(x1, y1)),
        sf::Vertex(sf::Vector2f(x2, y2))
    };

    window.draw(line, 2, sf::Lines);
}
