#include <iostream>
#include <sstream>
#include <string>

#include <gl/glew.h>
#include <gl/glut.h>

#include "ObjModel.h"
#include "TrafficLight.h"
#include "utility.h"
#include "BillBoard.h"
using namespace std;

// Counter for the traffic signal.  Only one is needed.
int counter = 0; 


// 20 miliseconds
int updateInterval = 20;

// 3D models
ObjModel car, surveillanceCamera, tower;
TrafficLight trafficLight, trafficLight2, trafficLight3, trafficLight4;
Billboard board, board2;


// The display list IDs for car, surveillanceCamera, and terrain.
int carID, surveillanceCameraID, terrainID, towerID;

// Signals for North-South traffic and West-East traffic
Signal NS_Signal = Green, WE_Signal = Red;

// Car position and speed
Vector3 carPosition = { 3, 0, 45 }, boardPos = { 20, 0, -15 }, boardPos2 = { -20, 0, -15 },
localCarSpeed = { 0, 0, 0 }, worldCarSpeed;

// Car direction and heading
float carDirection = 180;
float boarddirection = -45;
float boarddirection2 = 45;
string carHeading = "N";
float speed = 0;

// Third person camera offset in the car's local frame and world frame.
Vector3	localCameraOffset = {0, 3, -10}, worldCameraOffset;

// Width and height for the window and three small viewports.
int winWidth, winHeight, sWidth, sHeight;

// Update the small viewports' size automatically.
void reshape(int w, int h)
{
	winWidth = w, winHeight = h;
	// Update sWidth and sHeight here.
	sHeight = h / 5; sWidth = w / 5;
	
	
}

void specialKey(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT)
	{
		// Update car direction



		// Compute the car heading.
		if (carDirection == 360)
			carDirection = 0;
		carDirection += 22.5;
		worldCarSpeed = computeRotatedVector(localCarSpeed, carDirection);
	
		
		

	}
	if (key == GLUT_KEY_RIGHT)
	{
		if (carDirection == 0)
			carDirection = 360;
		carDirection -= 22.5;
		worldCarSpeed = computeRotatedVector(localCarSpeed, carDirection);
		// Handle the right turns.
	}
	if (key == GLUT_KEY_UP)
	{
		localCarSpeed.z = localCarSpeed.z + .2;
		worldCarSpeed = computeRotatedVector(localCarSpeed ,carDirection);
		// acceleration
	}

	if (key == GLUT_KEY_DOWN)
	{
		// deceleration

		localCarSpeed.z = localCarSpeed.z - .2;
		worldCarSpeed = computeRotatedVector(localCarSpeed, carDirection);
		carPosition.x -= worldCarSpeed.x; carPosition.y -= worldCarSpeed.y; carPosition.z -= worldCarSpeed.z;
		
	}
}

void drawScene()
{// covert speed to string
	string speedword = to_string(speed);

	string SpeedPhrase = "Speed: ";
	SpeedPhrase.append(speedword);
	string Direction = "Direction: ";
	Direction.append(carHeading);
	glColor3f(1, 0, 0);
	//print words
	glWindowPos2i((winWidth / 2) -125, winHeight - (sHeight*1.2));
	printString(SpeedPhrase);
	glWindowPos2i((winWidth / 2) + 75, winHeight - (sHeight*1.2));
	printString(Direction);


	// Draw terrain
	glCallList(terrainID);

	glEnable(GL_LIGHTING);
	trafficLight.setMaterials();
	
	/*vector<string> test;
	test=trafficLight.GetMaterialNames();
	for (int i = 0; i < test.size(); i++)
		cout << "name: " << test[i] << endl;*/
	// North-East (NS_Signal)
	glPushMatrix();
	glTranslatef(10, 0, -10.5);
	glScalef(1/3.28/12, 1/3.28/12, 1/3.28/12);
	trafficLight.setSignal(NS_Signal);
	trafficLight.Draw();
	glPopMatrix();
	
	
	glPushMatrix();
	glTranslatef(10, 0, -10);
	glRotatef(-45, 0, 1, 0);
	glCallList(surveillanceCameraID);
	glPopMatrix();

	// North-West (WE_Signal)
///board 1
	//glColor3f(1, 1, 0);
	glPushMatrix();
	board.Draw();
	glPopMatrix();
// board 2
	glPushMatrix();
	board2.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-10, 0, -10.5);
	glScalef(1 / 3.28 / 12, 1 / 3.28 / 12, 1 / 3.28 / 12);
	glRotatef(90, 0, 1, 0);
	trafficLight2.setSignal(WE_Signal);
	trafficLight2.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-10, 0, -10);
	glRotatef(45, 0, 1, 0);
	glCallList(surveillanceCameraID);
	glPopMatrix();
	
	// South-East (WE_Signal)
	glPushMatrix();
	glTranslatef(10, 0, 10);
	glScalef(1 / 3.28 / 12, 1 / 3.28 / 12, 1 / 3.28 / 12);
	glRotatef(-90, 0, 1, 0);
	trafficLight3.setSignal(WE_Signal);
	trafficLight3.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, 0, 10.5);
	glRotatef(-135, 0, 1, 0);
	glCallList(surveillanceCameraID);
	glPopMatrix();

	// South West(SN_Signal)
	glPushMatrix();
	glTranslatef(-10, 0, 10);
	glScalef(1 / 3.28 / 12, 1 / 3.28 / 12, 1 / 3.28 / 12);
	glRotatef(180, 0, 1, 0);
	trafficLight4.setSignal(NS_Signal);
	trafficLight4.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-10, 0, 10.5);
	glRotatef(135, 0, 1, 0);
	glCallList(surveillanceCameraID);
	glPopMatrix();

	// Draw the car.
	glPushMatrix();
	glTranslatef(carPosition.x, carPosition.y, carPosition.z);
	glRotatef(carDirection, 0, 1, 0);
	glCallList(carID);
	glPopMatrix();



	
}
void ScreenShot(int height, int width)
{
	PPMImage imageshot;
	//	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE,imageshot.image);
	imageshot.AllocateMemory(width, height);
	//	glReadBuffer(GL_BACK);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, imageshot.image);
	imageshot.VerticalFlip();
	imageshot.WriteFile("image.ppm", "P3");



}

void init()
{
	glClearColor(0.5, 0.5, 1.0, 1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Note that the light is defined in the eye or camera frame.
	GLfloat light_position[] = {0.0, 0.0, 0.0, 1.0};

	GLfloat ambient[] = {0.3, 0.3, 0.3, 1};
	GLfloat diffuse[] = {1.0, 1.0, 1.0, 1};
	GLfloat specular[] = {1.0, 1.0, 1.0, 1};

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glEnable(GL_LIGHTING);	
	glEnable(GL_LIGHT0);

	// Generate display list for the surveillance camera.
	surveillanceCameraID = glGenLists(1);
	
	glNewList(surveillanceCameraID, GL_COMPILE);
	surveillanceCamera.Draw();
	glEndList();

	// Generate display list for the car.
	carID = glGenLists(1);
	glNewList(carID, GL_COMPILE);
	car.Draw();
	glEndList();



	board.SetLocation(boardPos);
	board.SetOrientation(boarddirection);
	board.SetSize(3, 2.5);
	board.ReadFile("Square.ppm");

	board2.SetLocation(boardPos2);
	board2.SetOrientation(boarddirection2);
	board2.SetSize(5, 3.5);
	board2.ReadFile("wendys.ppm");

	// Generate the display list for terrain, including road and grass.
	terrainID = glGenLists(1);
	glNewList(terrainID, GL_COMPILE);
	glDisable(GL_LIGHTING);

	// Grass
	glColor3f(0, 0.7, 0);
	glBegin(GL_QUADS);
		glVertex3f(-1000, 0, 1000);
		glVertex3f(-10, 0, 1000);
		glVertex3f(-10, 0, 10);
		glVertex3f(-1000, 0, 10);

		glVertex3f(10, 0, 1000);
		glVertex3f(1000, 0, 1000);
		glVertex3f(1000, 0, 10);
		glVertex3f(10, 0, 10);

		glVertex3f(10, 0, -10);
		glVertex3f(1000, 0, -10);
		glVertex3f(1000, 0, -1000);
		glVertex3f(10, 0, -1000);

		glVertex3f(-1000, 0, -10);
		glVertex3f(-10, 0, -10);
		glVertex3f(-10, 0, -1000);
		glVertex3f(-1000, 0, -1000);
	glEnd();

	// Roads
	glBegin(GL_QUADS);
		glColor3f(0.2, 0.2, 0.2);

		glVertex3f(-10, 0, 1000);
		glVertex3f(10, 0, 1000);
		glVertex3f(10, 0, -1000);
		glVertex3f(-10, 0, -1000);

		glVertex3f(-1000, 0, 10);
		glVertex3f(1000, 0, 10);
		glVertex3f(1000, 0, -10);
		glVertex3f(-1000, 0, -10);
	glEnd();

	// Yellow line South
	glBegin(GL_POLYGON);
		glColor3f(1, 1, 0);
		glVertex3f(-0.1, 0.05, 1000);
		glVertex3f(0.1, 0.05, 1000);
		glVertex3f(0.1, 0.05, 10);
		glVertex3f(-0.1, 0.05, 10);
	glEnd();
	// Yellow line North
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 0);
	glVertex3f(-0.1, 0.05, -1000);
	glVertex3f(0.1, 0.05, -1000);
	glVertex3f(0.1, 0.05, -10);
	glVertex3f(-0.1, 0.05, -10);
	glEnd();
	// Yellow line west
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 0);
	glVertex3f(-1000, 0.05, -0.1);
	glVertex3f(-1000, 0.05, 0.1);
	glVertex3f(-10, 0.05, 0.1);
	glVertex3f(-10, 0.05, -0.1);
	glEnd();
	// Yellow line east
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 0);
	glVertex3f(1000, 0.05, -0.1);
	glVertex3f(1000, 0.05, 0.1);
	glVertex3f(10, 0.05, 0.1);
	glVertex3f(10, 0.05, -0.1);
	glEnd();
	// white dashed line South
	for (int i = 10; i <= 1000; i)
	{
		glBegin(GL_POLYGON);
		glColor3f(1, 1, 1);
		glVertex3f(4.9, 0.05, i);
		glVertex3f(5.1, 0.05, i);
		glVertex3f(5.1, 0.05, i + 3);
		glVertex3f(4.9, 0.05, i + 3);
		glEnd();
		i += 13;
	}
	
	for (int i = 10; i <= 1000; i)
	{
		glBegin(GL_POLYGON);
		glColor3f(1, 1, 1);
		glVertex3f(-4.9, 0.05, i);
		glVertex3f(-5.1, 0.05, i);
		glVertex3f(-5.1, 0.05, i + 3);
		glVertex3f(-4.9, 0.05, i + 3);
		glEnd();
		i += 13;
	}
	// white dashed line north
	for (int i = -10; i >= -1000; i)
	{
		glBegin(GL_POLYGON);
		glColor3f(1, 1, 1);
		glVertex3f(-4.9, 0.05, i);
		glVertex3f(-5.1, 0.05, i);
		glVertex3f(-5.1, 0.05, i - 3);
		glVertex3f(-4.9, 0.05, i - 3);
		glEnd();
		i -= 13;
	}

	for (int i = -10; i >= -1000; i)
	{
		glBegin(GL_POLYGON);
		glColor3f(1, 1, 1);
		glVertex3f(4.9, 0.05, i);
		glVertex3f(5.1, 0.05, i);
		glVertex3f(5.1, 0.05, i - 3);
		glVertex3f(4.9, 0.05, i - 3);
		glEnd();
		i -= 13;
	}
	// white dashed line west
	for (int i = -10; i >= -1000; i)
	{
		glBegin(GL_POLYGON);
		glColor3f(1, 1, 1);
		glVertex3f(i, 0.05, 4.9);
		glVertex3f(i, 0.05, 5.1);
		glVertex3f(i - 3, 0.05, 5.1);
		glVertex3f(i - 3, 0.05, 4.9);
		glEnd();
		i -= 13;
	}

	for (int i = -10; i >= -1000; i)
	{
		glBegin(GL_POLYGON);
		glColor3f(1, 1, 1);
		glVertex3f(i, 0.05, -4.9);
		glVertex3f(i, 0.05, -5.1);
		glVertex3f(i - 3, 0.05,-5.1);
		glVertex3f(i - 3, 0.05, -4.9);
		glEnd();
		i -= 13;
	}
	// white dashed line east
	for (int i = 10; i <= 1000; i)
	{
		glBegin(GL_POLYGON);
		glColor3f(1, 1, 1);
		glVertex3f(i, 0.05, -4.9);
		glVertex3f(i, 0.05, -5.1);
		glVertex3f(i + 3, 0.05, -5.1);
		glVertex3f(i + 3, 0.05, -4.9);
		glEnd();
		i += 13;
	}
	
	for (int i = 10; i <= 1000; i)
	{
		glBegin(GL_POLYGON);
		glColor3f(1, 1, 1);
		glVertex3f(i, 0.05, 4.9);
		glVertex3f(i, 0.05, 5.1);
		glVertex3f(i + 3, 0.05, 5.1);
		glVertex3f(i + 3, 0.05, 4.9);
		glEnd();
		i += 13;
	}




	glEndList();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Generate head-up display (HUD)
	stringstream ss;

	// Setup viewport, projection, and camera for the main view.
	glViewport(0, 0, winWidth, winHeight - sHeight - 50);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (float) winWidth / (winHeight - sHeight - 50), 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();



	// Update the code here for the correct third person camera that moves with the car.
//	cout << "WC: " << worldCameraOffset.x << "," << worldCameraOffset.y << "," << worldCameraOffset.z<< "||CC: " << carPosition.x << "," << carPosition.y << "," << carPosition.z << endl;
	gluLookAt(worldCameraOffset.x, worldCameraOffset.y , worldCameraOffset.z , carPosition.x, carPosition.y + 1.5,
		carPosition.z, 0, 1, 0);

	drawScene();

	// Setup viewport, projection, and camera for the South-East camera and draw the scene again.
	glViewport(0, winHeight - sHeight, sWidth, sHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, (float)sWidth / sHeight, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(10, 2, 10, 0,  0, 0, 0, 1, 0);
	drawScene();
	// Setup the viewport, projection, camera for the top view and draw the scene again.
	glViewport((winWidth-sWidth)/2, winHeight - sHeight, sWidth, sHeight);;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (float)sWidth / sHeight, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(-1, 40, 1, 0, 0, 0, 0, 1, 0);
	drawScene();
	// Setup viewport, projection, camera for the South-West camera and draw the scene again.
	glViewport(winWidth -sWidth, winHeight - sHeight, sWidth, sHeight);;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (float)sWidth / sHeight, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(-10, 2, 10, 0, 0, 0, 0, 1, 0);
	drawScene();
	glutSwapBuffers();
	glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 'r':
		// Add code for reset
		localCarSpeed.z = 0;
		carPosition.x = 3;
		carPosition.z = 45;
		carDirection = 180;
		localCarSpeed.z = 0;
		counter = 0;
		trafficLight.setSignal(NS_Signal);
		trafficLight2.setSignal(WE_Signal);
		trafficLight3.setSignal(WE_Signal);
		trafficLight4.setSignal(NS_Signal);
		worldCarSpeed = computeRotatedVector(localCarSpeed, carDirection);
		break;
	case 'b':
		localCarSpeed.z = 0;
		worldCarSpeed = computeRotatedVector(localCarSpeed, carDirection);
		
		break;
	case 's':
		ScreenShot(winHeight, winWidth);
		break;

	case 27:
		exit(0);
		break;
	}

	glutPostRedisplay();
}

void update()
{  //Calculate direction
	speed = localCarSpeed.z / .02;
	if (carDirection==180)
	{
		carHeading = "N";
	}
	else if (carDirection > 180 && carDirection < 270)
	{
		carHeading = "NW";

	}
	else if (carDirection == 270 )
	{
		carHeading = "W";

	}
	else if (carDirection > 270 && carDirection < 360)
	{
		carHeading = "SW";

	}
	else if (carDirection == 0)
	{
		carHeading = "S";

	}
	else if (carDirection > 0 && carDirection < 90)
	{
		carHeading = "SE";

	}
	else if (carDirection == 90 )
	{
		carHeading = "E";

	}
	else if (carDirection > 90 && carDirection < 180)
	{
		carHeading = "NE";

	}
	// Light controller
	if (counter == 5000)
	{
		if (NS_Signal == Green)
			NS_Signal = Yellow;
		if ( WE_Signal == Green)
		{
			WE_Signal = Yellow;
		}
		trafficLight.setSignal(NS_Signal);
		trafficLight4.setSignal(NS_Signal);
		trafficLight2.setSignal(WE_Signal);
		trafficLight3.setSignal(WE_Signal);

	}
	// Light controller
	if (counter == 6000)
	{
		if (NS_Signal == Red)
			NS_Signal = Green;
		if (WE_Signal == Red)
		{
			WE_Signal = Green;
		}
		if (NS_Signal == Yellow)
			NS_Signal = Red;
		if (WE_Signal == Yellow)
		{
			WE_Signal = Red;
		}
	
		trafficLight.setSignal(NS_Signal);
		trafficLight4.setSignal(NS_Signal);
		trafficLight2.setSignal(WE_Signal);
		trafficLight3.setSignal(WE_Signal);
		counter = 0;

	}

	
	//update carposition and camera
	carPosition.x += worldCarSpeed.x; carPosition.y += worldCarSpeed.y; carPosition.z += worldCarSpeed.z;
	worldCameraOffset = computeRotatedVector(localCameraOffset, carDirection);
	worldCameraOffset.x += carPosition.x; worldCameraOffset.y += carPosition.y; worldCameraOffset.z += carPosition.z;
	
}

void timer(int miliseconds)
{
	//cout << counter<<endl;
	update();
	glutTimerFunc(updateInterval, timer, updateInterval);

	counter += updateInterval;
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glutInitWindowPosition(100, 100);
	winWidth = 1300, winHeight = 800;
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Traffic Simulation");
	glewInit();

	// Load the 3D models.
	trafficLight.ReadFile("Models/TrafficLight.obj");
	trafficLight2.ReadFile("Models/TrafficLight.obj");
	trafficLight3.ReadFile("Models/TrafficLight.obj");
	trafficLight4.ReadFile("Models/TrafficLight.obj");
	//cout << "\n|||||||||||\n";
	car.ReadFile("Models/taxi.obj");
	//cout << "\n|||||||||||\n";
	surveillanceCamera.ReadFile("Models/camera.obj");
	//cout << "\n|||||||||||\n";
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKey);
	glutTimerFunc(0, timer, updateInterval);
	glutMainLoop();
	

	system("pause");
}