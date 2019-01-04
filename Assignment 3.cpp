#include <iostream>					// for string, cin, cout and endl
#include <glut.h>					// the library for glut (OpenGL)
#include "Mesh.h"					//including the mesh library


#include "ICS_Game.h"				// for the simple game framework

// constants
const int WINDOW_WIDTH = 800;		// the width of the game window in pixels
const int WINDOW_HEIGHT = 600;		// the height of the game window in pixels

const int NUMS_OF_MESH = 4;			//number of meshes

const int CHANGE_MESH_INDEX = 3;	//the last mesh index, which is where it has to return to 0 if the rightclick is true

const float WHEEL_ROTATE = 560.0;	//the number which divides the zoom value to make smoother zooms
const int MIN_ZOOM = -15;			//the minimum zoom
const int MAX_ZOOM = 0;				//the maximum zoom

const int RIGHT_ANGLE_UP = -90;		//the angle in degrees to cap the rotation up
const int RIGHT_ANGLE_DOWN = 90;	//the angle in degrees to cap the rotation down

const int X_ANGLE_DEFAULT = 0;		//the default rotation of angle x
const int Y_ANGLE_DEFAULT = 0;		//the default rotation of angle y

// global variables
float zoom = -2.5;						// the number of units to position the camera away from the origin along the z-axis
float rotateXAngle = X_ANGLE_DEFAULT;	// the number of degrees to rotate the model around the x-axis
float rotateYAngle = Y_ANGLE_DEFAULT;	// the number of degrees to rotate the model around the y-axis

Mesh wireMesh[4];						//creating an array of mesh

//array of file names
string dataFile[4] =			

{ 
	"data/data1.txt",
	"data/data2.txt",
	"data/data3.txt",
	"data/data4.txt"
};

//the current mesh being printed
int meshCount = 0;

//checking if the left mouse has been clicked
bool leftMouseHeldDown = false;

//the current rotation value of x and y
float currentRotateX = 0;
float currentRotateY = 0;

//the position of the mouse when it was clicked
float clickedX = 0;
float clickedY = 0;


/**
 * This initializes the program.
 */
void initialize()
{
	//using a for loop to go through each mesh
	for (int i = 0; i < NUMS_OF_MESH; i++)
	{
		//setting each mesh to the coresponding data file
		wireMesh[i].dataFile = dataFile[i];
		//initializing each mesh
		initializeMesh(wireMesh[i]);
	}

	glColor3f(1.0f, 1.0f, 1.0f);				// set the drawing color to white
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// set the render mode to wireframe
}



void render3D()
{
	
	glTranslatef(0.0f, 0.0f, zoom);				// position the camera on the z-axis
	glRotatef(rotateXAngle, 1.0f, 0.0f, 0.0f);	// rotate the model around the x-axis
	glRotatef(rotateYAngle, 0.0f, 1.0f, 0.0f);	// rotate the model around the y-axis

	
	glBegin(GL_TRIANGLES);				// this starts drawing triangles.  every 3 vertices between
										// here and glEnd() will be rendered on screen as a triangle

	//using a for loop to go through each triangle of the current mesh
	for (int i = 0; i < wireMesh[meshCount].currentNumOfTriangles; i++)
	{
		//using a for loop to go through each vertice of the triangle
		for (int j = 0; j < NUM_OF_POINTS; j++)
		{
			//drawing a single vertice, x, y, z coordinates
			glVertex3f(wireMesh[meshCount].arrayOfTriangles[i].vertice[j][X_COORDINATE_INDEX], wireMesh[meshCount].arrayOfTriangles[i].vertice[j][Y_COORDINATE_INDEX], wireMesh[meshCount].arrayOfTriangles[i].vertice[j][Z_COORDINATE_INDEX]);
		}
	}

	glEnd();							// the stops drawing triangles
	
}

/**
 * This handles mouse move events.
 *
 * @param x		The x position of the mouse cursor.
 * @param y		The y position of the mouse cursor.
 */
void handleMouseMove(int x, int y)
{
	//checking if the left mouse click is true 
	if(leftMouseHeldDown == true)
	{
		//changing the rotate y angle by adding the difference in x position and the current rotate y value
		rotateYAngle = (x - clickedX) + currentRotateY;

		//changing the rotate x angle by adding the difference in y position and the current rotate x value
		rotateXAngle = (y - clickedY) + currentRotateX;

		//checking if the x rotation if above the cap 
		if (rotateXAngle < RIGHT_ANGLE_UP)
		{
			//setting x rotation to the cap
			rotateXAngle = RIGHT_ANGLE_UP;
		}

		//checking if the x rotation if below the cap
		if (rotateXAngle > RIGHT_ANGLE_DOWN)
		{
			//setting x rotation to the cap
			rotateXAngle = RIGHT_ANGLE_DOWN;
		}
	}

	

}

/**
 * This handles mouse left click events.
 *
 * @param x		The x position of the mouse cursor.
 * @param y		The y position of the mouse cursor.
 * @param down	true if the mouse button went down, false it it was released.
 */
void handleLeftClickEvent(int x, int y, bool down)
{
	//checking if the left button was clicked
	if (down)
	{
		//setting left clicked to true
		leftMouseHeldDown = true;

		//setting the current x and y position to use in other calculations(delta x, delta y), rotation
		clickedX = x;
		clickedY = y;

		//setting the current rotation value to use in other calculations, rotation
		currentRotateX = rotateXAngle;
		currentRotateY = rotateYAngle;
	}
	//in the event the button is released
	else
	{
		//setting left mouse click to false
		leftMouseHeldDown = false;
	}
}

/**
 * This handles mouse left click events.
 *
 * @param x		The x position of the mouse cursor.
 * @param y		The y position of the mouse cursor.
 * @param down	true if the mouse button went down, false it it was released.
 */
void handleRightClickEvent(int x, int y, bool down)
{
	//checking if the right button was pressed
	if (down)
	{
		//checking if the mesh count is at the point where it needs to loop back to 0
		if (meshCount == CHANGE_MESH_INDEX)
		{
			//setting mesh count to 0
			meshCount %= CHANGE_MESH_INDEX;
		}
		//if mesh count still isnt at the limit
		else
		{
			//adding one to the mesh count
			meshCount++;
		}

		//setting the rotation to the default after switching the mesh
		rotateXAngle = X_ANGLE_DEFAULT;
		rotateYAngle = Y_ANGLE_DEFAULT;
	}
}

/**
 * This handles mouse wheel events.
 *
 * @param x			The x position of the mouse cursor.
 * @param y			The y position of the mouse cursor.
 * @param rotation	The wheel rotation value.  A positive value indicates that the wheel was rotated forward, away
 *					from the user; a negative value indicates that the wheel was rotated backward, toward the user. 
 */
void handleMouseWheel(int x, int y, int rotation)
{
	//declaring a variable of type float that takes the rotation of the wheel and divides it by the wheel rotate to make it smoother
	float zoomAmount = rotation / WHEEL_ROTATE;

	//adding zoomAmount to the zoom value
	zoom += zoomAmount;

	//checking if the zoom value is going over the minimum zoom
	if (zoom < MIN_ZOOM)
	{
		//setting zoom to the minimum
		zoom = MIN_ZOOM;
	}

	//checking if the zoom value is going over the maximum value
	if (zoom > MAX_ZOOM)
	{
		//setting the zoom to the maximum
		zoom = MAX_ZOOM;
	}
}

/**
 * The callback for handling exiting the program.
 */
void handleExit()
{
	//using a for loop to go through each mesh
	for (int i = 0; i < NUMS_OF_MESH; i++)
	{
		//calling the delete function to delete the mesh at the specific index
		deleteMesh(wireMesh[i]);
	}
}

/**
 * This is the entry point to the program.
 *
 * @param hInstance			A handle to the current instance of the application.
 * @param hPrevInstance		A handle to the previous instance of the application.
 * @param lpCmdLine			The command line for the application, excluding the program name.
 * @param nCmdShow			Controls how the window will be shown.
 *
 *@returns					If the function succeeds, terminating when it receives a WM_QUIT message, it should return the exit value contained
 *							in that message's wParam parameter.  If the function terminates before entering the message loop, it should return zero.
 */
#ifdef _DEBUG
int main()
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{

	// create the game and set the callbacks
	ICS_Game::getInstance().setInitializeCallback(initialize);				// set the callback for initializing the program
	ICS_Game::getInstance().setRender3DCallback(render3D);					// set the callback for rendering 3D game elements
	ICS_Game::getInstance().setMouseMoveCallback(handleMouseMove);			// set the callback for handling mouse move events
	ICS_Game::getInstance().setMouseMoveCallback(handleMouseWheel);			// set the callback for handling mouse wheel events
	ICS_Game::getInstance().setLeftClickCallback(handleLeftClickEvent);		// set the callback for handling mouse left click events
	ICS_Game::getInstance().setRightClickCallback(handleRightClickEvent);	// set the callback for handling mouse right click events
	ICS_Game::getInstance().setExitCallback(handleExit);					// set the callback for exiting the program

	// start the game... the program ends when this function returns (when the game loop ends)
	return ICS_Game::getInstance().go("OpenGL", WINDOW_WIDTH, WINDOW_HEIGHT);
}