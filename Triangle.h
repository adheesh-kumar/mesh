#pragma once						//including pragma once in order to include this header file once

const int NUM_OF_POINTS = 3;		//const number of points in a triangle
const int NUM_OF_COORDINATES = 3;	//const number of coordinates(xyz) in a point

const int X_COORDINATE_INDEX = 0;	//the index for the x coordinate
const int Y_COORDINATE_INDEX = 1;	//the index for the y coordinate
const int Z_COORDINATE_INDEX = 2;	//the index for the z coordinate

/******************************************
this struct represents a triangle with a
2D array representing the attributes of a
Triangle
******************************************/
struct Triangle
{
	//2D Array of floats that reprents all 3 points and within each of the points, the x, y, z, coordinates
	float vertice[NUM_OF_POINTS][NUM_OF_COORDINATES];
};