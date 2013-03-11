/*********************************
Barışcan AKIN
040080224

Solar System
	Sun
	Earth -> Moon
	Mars -> Phobos -> Deimos

Keyboard functions
	PAGE UP: Zoom in
	PAGE DOWN: Zoom out
	LEFT: Rotates camera to left
	RIGHT: Rotates camera to right
	UP: Rotates camera to up
	DOWN: Rotates camera to down

	Z: Orbit speed up
	X: Orbit speed down
*********************************/
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <SOIL/SOIL.h>

const int SPHERE_COUNT = 6;

const int WIDTH = 800;
const int HEIGHT = 600;

GLdouble angle = 0;

//Texture arrays
GLint textures[SPHERE_COUNT];
char *texturePaths[SPHERE_COUNT] = {"Textures/sunmap.bmp", 
									"Textures/earthmap.bmp", 
									"Textures/moonmap.bmp", 
									"Textures/marsmap.bmp", 
									"Textures/phobosmap.bmp", 
									"Textures/deimosmap.bmp"};

//Light specifications
GLfloat ambient[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat position[] = {0.0f, 0.0f, 0.0f, 1.0f};

//Glowing sun
GLfloat glow_on[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat glow_off[] = {0.0f, 0.0f, 0.0f, 1.0f};

//Camera and perspective values
GLfloat zoom_factor = 50.0;
GLfloat zNear = 0.1;
GLfloat zFar = 200;
GLint orbitDegreesX = 0;
GLint orbitDegreesY = 0;

//Orbit speed
GLdouble speed = 1.0;

//Takes image path as parameter
//Returns id of created texture
GLint LoadGLTexture(const char *filename)
{
    GLuint _texture;
    
    _texture = SOIL_load_OGL_texture // load an image file directly as a new OpenGL texture 
	(
     filename,
     SOIL_LOAD_AUTO,
     SOIL_CREATE_NEW_ID,
     SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_MIPMAPS
     );
    
    // check for an error during the load process 
    if(_texture == 0)
    {
	    printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
    }
	
    
    glBindTexture(GL_TEXTURE_2D, _texture); // select our current texture
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // texture should tile
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return _texture;
}

static void Draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(zoom_factor, (float) WIDTH / (float) HEIGHT, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, -80, 0, 0, 0, 0, 1, 0);
	glRotated(orbitDegreesX, 0, 1, 0);
	glRotated(orbitDegreesY, 1, 0, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
	
	glEnable(GL_TEXTURE_2D);

	GLUquadric* quadric = gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluQuadricTexture(quadric, GL_TRUE);
	
	//Sun
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glMaterialfv(GL_FRONT, GL_EMISSION, glow_on);
    gluSphere(quadric, 3, 24, 24);
	glMaterialfv(GL_FRONT, GL_EMISSION, glow_off);
    glPushMatrix();

	//Earth
	glBindTexture(GL_TEXTURE_2D, textures[1]);
    glRotated(2*angle, 0, 1, 0);
    glTranslatef(7.f, 0.f, 0.f);
    glRotated(angle, 0, 1, 0);
    gluSphere(quadric, 1, 24, 24);
    glPushMatrix();

	//Moon
	glBindTexture(GL_TEXTURE_2D, textures[2]);
    glRotated(4*angle, 0, 1, 0);
    glTranslatef(2.f, 0.f, 0.f);
    glRotated(angle, 0, 1, 0);
    gluSphere(quadric, 0.5, 24, 24);
    
    glPopMatrix();
    glPopMatrix();
	
	//Mars
    glBindTexture(GL_TEXTURE_2D, textures[3]);
    glRotated(angle + 130, 0, 1, 0);
    glTranslatef(20.0f, 0.f, 0.f);
    glRotated(angle, 0, 1, 0);
    gluSphere(quadric, 1.5, 24, 24);
    glPushMatrix();
    
	//Phobos (1st moon of Mars)
    glBindTexture(GL_TEXTURE_2D, textures[4]);
    glRotated(4*angle, 0, 1, 0);
    glTranslatef(4.f, 0.f, 0.f);
    glRotated(angle, 0, 1, 0);
    gluSphere(quadric, 0.5, 24, 24);
    glPopMatrix();
    
	//Deimos (2nd moon of Mars)
    glBindTexture(GL_TEXTURE_2D, textures[5]);
    glRotated(4*angle + 150 , 0, 1, 0);
    glTranslatef(7.f, 0.f, 0.f);
    glRotated(angle, 0, 1, 0);
    gluSphere(quadric, 1, 24, 24);

	glPopMatrix();
	glPopMatrix();

	glLoadIdentity();

	//Setting camera location and angle
	gluLookAt(0, 0, -100, 0, 0, 0, 0, 1,  0);

    glutSwapBuffers();

}

static void timerCallback (int value)
{
	angle += speed;
	glutTimerFunc(10, timerCallback, value);
	glutPostRedisplay();
}

void LoadGLTextures()
{
	for (int i = 0; i < SPHERE_COUNT; i++) {
		textures[i] = LoadGLTexture(texturePaths[i]);
    }
}

void InitGL(GLvoid)
{
	GLenum type;
	glutInitWindowSize(WIDTH, HEIGHT);
	type = GLUT_RGB;
	type |= GLUT_DOUBLE;
	type |= GLUT_DEPTH;
	type |= GLUT_MULTISAMPLE;
	glutInitDisplayMode(type);
	glutCreateWindow("Solar System - Bariscan Akin");

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	LoadGLTextures();
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void Key(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // ESC
			exit(0);
            break;
		case 'z':
		case 'Z':
			speed += 0.05;
			break;
		case 'x':
		case 'X':
			speed -= 0.05;
			break;
    }
}

void SpecialKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
			orbitDegreesX++;
            break;
        case GLUT_KEY_RIGHT:
			orbitDegreesX--;
            break;
        case GLUT_KEY_UP:
			orbitDegreesY--;
            break;
        case GLUT_KEY_DOWN:
			orbitDegreesY++;
            break;
        case GLUT_KEY_PAGE_UP:
			if(zoom_factor >= 5)
				zoom_factor -= 0.5;
            break;
        case GLUT_KEY_PAGE_DOWN:
			if(zoom_factor <= zFar/2)
				zoom_factor += 0.5;
            break;
    }
}

int main (int argc, char ** argv)
{
	glutInit(&argc, argv);
	InitGL();

	timerCallback(0);
	glutKeyboardFunc(Key);
	glutSpecialFunc(SpecialKey);
	glutDisplayFunc(Draw);
	glutMainLoop();
	return 0;
}

