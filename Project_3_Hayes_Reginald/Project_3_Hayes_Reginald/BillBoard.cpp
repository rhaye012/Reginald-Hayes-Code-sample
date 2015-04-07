#include "Billboard.h"


Billboard::Billboard(void)
{
}

Billboard::~Billboard(void)
{
	glDeleteTextures(1, &textureNumber);
}

void Billboard::ReadFile(string fileName)
{
	// Read the texture file, generate the texture object, and configure
	// the texturing parameters.
	PPMImage texture;
	texture.ReadFile(fileName);
	glGenTextures(1, &textureNumber);
	glBindTexture(GL_TEXTURE_2D, textureNumber);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, texture.image);




}

void Billboard::SetSize(float width, float height)
{
	widthboard = width; heightboard = height;
}

void Billboard::SetLocation(Vector3 location)
{
	locationboard.x = location.x; locationboard.y = location.y; locationboard.z = location.z;
}

void Billboard::SetOrientation(float orientation)
{
	orientationboard = orientation;
	// Your code
}

void Billboard::Draw()
{	
	// Draw the board and pillar.  Use texture mapping for the board only.
	// The pillar is drawn just using simple geometry, i.e., a rectangle.
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureNumber);
	glTranslatef(locationboard.x, locationboard.y, locationboard.z);
	glRotatef(orientationboard, 0, 1, 0);
	float kd[4];
	kd[0] = .33;
	kd[1] = .33;
	kd[2] = .33;
	kd[3] = .33;
	glDisable(GL_BLEND);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, kd);

	glBegin(GL_QUADS);
	glTexCoord3f(-1, 1, 0);; glVertex3f(-1*widthboard, 2*heightboard, 0);
	glTexCoord3f(-1, 2, 0); glVertex3f(-1 * widthboard, 2 * heightboard+heightboard, 0);
	glTexCoord3f(0, 2, 0); glVertex3f(widthboard, 2 * heightboard + heightboard, 0);
	glTexCoord3f(0, 1, 0); glVertex3f(widthboard, 2 * heightboard, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f(-1, 0, 0);
	glVertex3f(-1, 2*heightboard, 0);
	glVertex3f(1, 2 * heightboard, 0);
	glVertex3f(1, 0, 0);
	glEnd();
	
	
	

	
	// Use glEnable() and glDisable() to change OpenGL states.
}
