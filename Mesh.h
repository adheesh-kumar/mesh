#pragma once							//including pragma once in order to include this header file once
#include "Triangle.h"					//including the triangle header file
#include <string>						//including string
#include <fstream>						//including file stream
#include <sstream>						//including string stream

using namespace std;					//using the standard namespace

const int DEFAULT_MAX_TRIANGLES = 10;	//the default number of triangles

const int RESIZE_AMOUNT = 2;			//the amount used to resize the array

const int TOKEN_DEFAULT_MAX = 3;		//default amount of tokens

const string TRIANGLE_START = "P 3";	//the string which starts a new triangle

/***************************************
this struct represents a mesh of triangles
which create an image: teapot, rocket,
sphere, spaceship
***************************************/
struct Mesh
{
	//the data file corresponding to where the mesh is stored
	string dataFile;

	//setting the default number of triangles to 10;
	int maxNumOfTriangles = DEFAULT_MAX_TRIANGLES;

	//setting the current number of triangles to 0;
	int currentNumOfTriangles = 0;

	//dynamically allocating on array of triangles
	Triangle* arrayOfTriangles = NULL;
};
 

/*****************************************
using a template function to resize an
array of any type, in this case, the 
number of triangles
@param1 the array being resized
@param2 the current array size
@param3 the maxSize that will be changed
*****************************************/

//template 
template <class T>
T* reSize(T* inputArray, int currentsize, int& maxsize)
{
	//doubling the maximum size
	maxsize *= RESIZE_AMOUNT;

	//creating a new temp array of type T and allocating double the memory of the original
	T* tempArray = new T[maxsize];

	//using a for loop to set each element of the original array into the temp
	for (int i = 0; i < currentsize; i++)
	{
		//assigning the values the original to the temp
		tempArray[i] = inputArray[i];
	}

	//freeing the memory in the original
	delete[] inputArray;

	//setting the pointer to NULL so it doesnt point to invalid memory
	inputArray = NULL;
	
	//returning the new array with twice the allocated memory
	return tempArray;
}


/********************************************
this function of type string takes a character
delimited string and returns an array of tokens
@param1 the text being tokenized
@param2 the character which seperates the text
@param3 the number of tokens created
********************************************/
string* tokenize(string text, char delimiter, int& tokenCount)
{
	//creating a string stream
	stringstream ss;

	//passing the text into the string stream
	ss << text;

	//declaring a variable to store each token
	string token;

	//declaring the default max token per line
	int maxtoken = TOKEN_DEFAULT_MAX;

	//creating an array of strings with the default maxtoken
	string* strings = new string[maxtoken];

	//using a while loop to go through each token of the text
	while (getline(ss, token, delimiter))
	{
		//checking if the tokenCount is equal to the max
		if (tokenCount == maxtoken)
		{
			//resizing the array of strings
			strings = reSize(strings, tokenCount, maxtoken);
		}

		//setting the token equal to the array of strings at index token count
		strings[tokenCount] = token;

		//adding 1 to the token count
		tokenCount++;
	}

	//returning the array of strings
	return strings;
}


/****************************************************************
the function assigns a point to a mesh of triangles
with 3 values (x, y, and z coordinates)
@param1 the mesh that is going to be affected
@param2 an array of strings containin each xyz coordinate
@param3 the number of tokens in the count
@param4 the vertice the function is going to assign values too
****************************************************************/
void getPoint(Mesh& mesh, string* xyzCoordinates, int tokenCount, int verticeCount)
{
	//using a for loop to go through each coordinate
	for (int i = 0; i < tokenCount; i++)
	{
		//setting the coordinate to the proper triangle ---> proper vertice ---> proper coordinate
		mesh.arrayOfTriangles[mesh.currentNumOfTriangles].vertice[verticeCount][i] = atof(xyzCoordinates[i].c_str());
	}
}


/*************************************************
function to initalize a mesh
@param1 the mesh to initialize
*************************************************/
void initializeMesh(Mesh& createMesh)
{ 
	//opening the file for file reading
	ifstream inFile(createMesh.dataFile);

	//checking if the file is open
	if (inFile.is_open())
	{
		//creating a variable of string to read from the file
		string line;

		//setting the max num of triangles to the default;
		createMesh.maxNumOfTriangles = DEFAULT_MAX_TRIANGLES;
		//setting the number of triangles to 0;
		createMesh.currentNumOfTriangles = 0;

		//allocating memory for the number of triangles 
		createMesh.arrayOfTriangles = new Triangle[createMesh.maxNumOfTriangles];
		
		//using a while loop to read a line from the file
		while (getline(inFile, line))
		{
			//setting the number of tokens to 0
			int tokenCount = 0;

			//checking if the line is equal to the start of a new triangle
			if (line == TRIANGLE_START)
			{
				//checking if the current number of triangles equals the max, than allocate more memory
				if (createMesh.currentNumOfTriangles == createMesh.maxNumOfTriangles)
				{
					//calling the resize function to double the size of the array of triangles
					createMesh.arrayOfTriangles = reSize(createMesh.arrayOfTriangles, createMesh.currentNumOfTriangles, createMesh.maxNumOfTriangles);
				}

				//setting the vertice count to 0;
				int verticeCount = 0;

				//using a for loop to go through each line containing the xyz coordinates of 3 vertices
				for (int i = 0; i < NUM_OF_POINTS; i++)
				{
					//setting the token count to 0
					tokenCount = 0;

					//reading one vertice from the file
					getline(inFile, line);

					//tokenizing the xyz coordinates and putting them into an array
					string* vertice = tokenize(line, ',', tokenCount);

					//assigning the xyz coordinates to the vertice
					getPoint(createMesh, vertice, tokenCount, verticeCount);

					//increasing the vertice count
					verticeCount++;

				}

				//increasing the size of the current triangle after all vertices and coordinates have been placed in the current one
				createMesh.currentNumOfTriangles++;
			}
		}

	}
	//incase of the file not opening
	else
	{
		//printing an error message
		cout << "Error Opening Mesh File" << endl;
	}
}

/***************************************************
this function deletes and frees up memory from a 
mesh
@param1 th emesh being deleted
***************************************************/
void deleteMesh(Mesh& deletedMesh)
{
	//deleting the array of triangles
	delete[] deletedMesh.arrayOfTriangles;

	//setting the array of triangles to NULL
	deletedMesh.arrayOfTriangles = NULL;

	//deleteing the current number of triangles
	deletedMesh.currentNumOfTriangles = 0;

}