#define _CRT_SECURE_NO_WARNINGS

#ifndef MAIN_H
#define MAIN_H

#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "CustomSceneViewer.h"
#include "Def.h"

struct triangle;

using namespace std;

struct ray {
	Point start;
	Point end;
	Vector dir;
};

struct Bounding_Box
{
	float min_x, min_y, min_z;
	float max_x, max_y, max_z;
};

struct Octree
{
	Bounding_Box bbox;
	bool is_leaf;
	int depth;
	vector<triangle> primitives;

	vector<Octree> children;	
};


#endif