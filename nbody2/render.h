#pragma once

#include "vec2D.h"
#include "quadtree.h"
#include "point.h"


#include <SFML/Graphics.hpp>
#include <string>
#include <Windows.h>

using std::string;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int fps = 60;

const int seed = 2;
const int N = 1000;
const int s = 5;

class Renderer {
public:
	Renderer();
	void run();

private:
	sf::RenderWindow window;

	std::vector<Point> points;
	std::vector<Vec2D> velocities;
	std::vector<Vec2D> accelerations;

	void generatePoints();
	void generatePointsInCircle(float rad, int num, float offX, float offY);
	void generateVelocities(int centerX, int centerY, int num, Vec2D vOff);

	void drawTree(QuadTree* t);

	void drawPoint(float x, float y, float s, sf::Color c);
	void drawRectangle(float x, float y, float r);
	void drawLine(float x1, float y1, float x2, float y2);
};