//  Includes
#include "TextureBuilder.h"
#include <math.h>
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <glut.h>
#include <cstdlib> 
int rep = 1;

#define _USE_MATH_DEFINES
# define M_PI  
GLuint texID;
GLuint texID2;
GLuint texID3;
//-----------------

//  Methods Signatures
void drawRect(int x, int y, int w, int h);
void drawCircle(int x, int y, float r);
void Key(unsigned char key, int x, int y);
void KeyUp(unsigned char key, int x, int y);
void Timer(int value);
void Display();
void Anim();
void drawObstacles();
//-----------------

//  Global Variables
int selectedBar = 0; // used to determine which bar has the mouse currently over it
int bar1Y = 0;       // holds the Y translation value of bar 1
int bar2Y = 0;       // holds the Y translation value of bar 2
int bar3Y = 0;       // holds the Y translation value of bar 3
int ballY = 0;       // holds the value of the Y coordinate of the ball
					 //-----------------


int p0[2];
int p1[2];
int p2[2];
int p3[2];
int tar = 4;


int back1x = 0;
int back1x2 = 1280;
int back2x = 1280;
int back2x2 = 2560;
int back3x = 2560;
int back3x2 = 3840;
int currentLevel = 3;


int transx = 0;
int transy = 0;

double obsx1 = 1280;
float obsx2 = 1350;
double obsx3 = 1280;
double obsy1 = 650;
float obsy2 = 500;
double obsy3 = 200;

int tmp = 0;
int tmp1 = 0;
int tmp2 = 0;

int powerx = 1280;
int powery = 200;

int transobsx2 = 0;
int transobsy2 = 0;

int c = 0;
int degree = 0;
int counter = 0;
int k = 1;
bool up = true;
double x = 0.0;
double x1 = 0.0;
double x2 = 0.0;

int eshara = 1300;
int red = 700;
int yellow = 680;
int green = 660;

int busleft = -75;
int busright = 75;
int busup = 25;
int busdown = -25;

void main(int argc, char** argr) {
	glutInit(&argc, argr);

	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(0, 0);

	glutCreateWindow("Book Catcher");
	glutDisplayFunc(Display);
	glutKeyboardFunc(Key);      // sets the Keyboard handler function; called when a key is pressed
	glutKeyboardUpFunc(KeyUp);  // sets the KeyboardUp handler function; called when a key is released
	glutTimerFunc(0, Timer, 0); // sets the Timer handler function; which runs every `Threshold` milliseconds (1st argument)

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glEnable(GL_TEXTURE_2D);
	glutIdleFunc(Anim);
	glClearColor(1, 1, 1, 0);
	gluOrtho2D(0, 1280, 0, 720);
	loadBMP(&texID, "textures/1.bmp", true);
	loadBMP(&texID2, "textures/2.bmp", true);
	loadBMP(&texID3, "textures/5.bmp", true);
	glutMainLoop();
}


void print(int x, int y, char *string)
{
	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, y);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}

// draws rectangles using the (x,y) of the bottom left vertex, width (w) and height (h)
void drawRect(int x, int y, int w, int h) {
	glBegin(GL_POLYGON);
	glVertex2f(x, y);
	glVertex2f(x + w, y);
	glVertex2f(x + w, y + h);
	glVertex2f(x, y + h);
	glEnd();
}

// draws a circle using OpenGL's gluDisk, given (x,y) of its center and tis radius
void drawCircle(int x, int y, float r) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	GLUquadric *quadObj = gluNewQuadric();
	gluDisk(quadObj, 0, r, 50, 50);
	glPopMatrix();
}

// Keyboard handler function
//   key: the key pressed on the keyboard
//   x  : the X coordinate of the mouse cursor at the moment of pressing the key
//   y  : the Y coordinate of the mouse cursor at the moment of pressing the key
void Key(unsigned char key, int x, int y) {


	if (key == 'w' && transy + 20 < 340) {
		transy += 40;
	}
	if (key == 's' && transy - 20 > -340) {
		transy -= 40;
	}



	// ask OpenGL to recall the display function to reflect the changes on the window
	glutPostRedisplay();
}

// KeyboardUp handler function
//   similar to the Keyboard handler function except that it is called only when the key is released
void KeyUp(unsigned char key, int x, int y) {


}




void Timer(int value) {


	// ask OpenGL to recall the display function to reflect the changes on the window
	glutPostRedisplay();

	glutTimerFunc(10, Timer, 0);
}

void drawBackground1() {
	glBindTexture(GL_TEXTURE_2D, texID);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(back1x, 0, 0);
	glTexCoord2f(rep, 0.0f);
	glVertex3f(back1x, 720, 0);
	glTexCoord2f(rep, rep);
	glVertex3f(back1x2, 720, 0);
	glTexCoord2f(0.0f, rep);
	glVertex3f(back1x2, 0, 0);
	glEnd();

}
void drawBackground2() {
	glBindTexture(GL_TEXTURE_2D, texID2);
	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(back2x, 0, 0);
	glTexCoord2f(rep, 0.0f);
	glVertex3f(back2x, 720, 0);
	glTexCoord2f(rep, rep);
	glVertex3f(back2x2, 720, 0);
	glTexCoord2f(0.0f, rep);
	glVertex3f(back2x2, 0, 0);
	glEnd();
	//loadBMP(&texID2, "textures/2.bmp", true);
}

void drawBackground3() {
	glBindTexture(GL_TEXTURE_2D, texID3);
	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(back3x, 0, 0);
	glTexCoord2f(rep, 0.0f);
	glVertex3f(back3x, 720, 0);
	glTexCoord2f(rep, rep);
	glVertex3f(back3x2, 720, 0);
	glTexCoord2f(0.0f, rep);
	glVertex3f(back3x2, 0, 0);
	glEnd();
	glColor3f(1, 1, 1);
	print(3200, 360, "GAME OVER");
	//loadBMP(&texID3, "textures/4.bmp", true);
}

void drawModel() {
	int midx = 640;
	int midy = 360;

	glPushMatrix();
	glTranslated(midx, midy, 0);

	glColor3f(1, 0, 0);
	glTranslated(transx, transy, 0);
	drawRect(-75, -25, 150, 50);

	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(50, -20, 0);
	glVertex3f(50, 20, 0);
	glVertex3f(60, 20, 0);
	glVertex3f(60, -20, 0);
	glEnd();


	drawCircle(-35, -20, 10);
	drawCircle(35, -20, 10);
	glColor3f(1, 1, 1);
	drawCircle(-35, -20, 2);
	drawCircle(35, -20, 2);

	glBegin(GL_TRIANGLES);
	glColor3f(1, 1, 0);
	glVertex3f(75, -20, 0);
	glVertex3f(75, 0, 0);
	glVertex3f(85, -10, 0);
	glEnd();

	glColor3f(1, 1, 1);
	drawRect(-50, 5, 15, 15);
	drawRect(-20, 5, 15, 15);
	drawRect(10, 5, 15, 15);

	glTranslated(-midx, -midy, 0);
	glPopMatrix();

}

void drawObstacles() {
	glPushMatrix();
	glColor3f(0, 0, 0);
	drawRect(obsx1, obsy1, 40, 80);
	glColor3f(1, 0, 0);
	drawCircle(eshara, red, 10);
	glColor3f(1, 1, 0);
	drawCircle(eshara, yellow, 10);
	glColor3f(0, 1, 0);
	drawCircle(eshara, green, 10);
	glPopMatrix();



	glColor3f(1, 1, 0);
	drawCircle(obsx2, obsy2, 30);


	glPushMatrix();
	glLineWidth(5);
	glBegin(GL_LINE_STRIP);
	glColor3f(0, 0, 0);
	glVertex3f(obsx3, obsy3, 0);
	glVertex3f(obsx3 + 20, obsy3 + 10, 0);
	glVertex3f(obsx3, obsy3 + 20, 0);
	glVertex3f(obsx3 + 20, obsy3 + 30, 0);
	glVertex3f(obsx3, obsy3 + 40, 0);
	glVertex3f(obsx3 + 20, obsy3 + 50, 0);
	glVertex3f(obsx3, obsy3 + 60, 0);
	glEnd();
	glPopMatrix();

}

void drawPower() {
	glColor3f(0.5, 0.5, 0.5);
	drawRect(powerx, powery, 65, 70);
	glColor3f(1, 1, 1);
	print(powerx + 10, powery + 50, "Book");
}

void Display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	glColor3f(1, 1, 1);
	drawBackground1();
	drawBackground2();
	drawBackground3();
	glPopMatrix();
	glColor3f(1, 1, 1);
	drawModel();
	drawObstacles();
	drawPower();
	glColor3f(1, 1, 1);
	print(640, 650, "Catch the Books");

	glFlush();
}

void animateBack() {
	back1x -= 1;
	back1x2 -= 1;
	back2x -= 1;
	back2x2 -= 1;

	if (back2x == 0) {
		back1x += 2560;
		back1x2 += 2560;
	}
	if (back1x == 0) {
		back2x += 2560;
		back2x2 += 2560;
	}
	for (size_t i = 0; i < 999999; i++);
}

void moveObstacles() {


	switch (c) {
	case 0: if (obsx1 > -100) {
		obsx1 -= 1;
		eshara -= 1;
	}
			else {
				obsx2 = 1350;
				obsy2 = rand() % 600;
				tmp = obsy2;
				c++;
			}break;
	case 1: if (obsx2 > -100) {
		obsx2 -= 1;
		obsy2 = tmp + 100 * sin(x / 180.0 * 3.14);
		x++;
	}
			else {
				obsx3 = 1280;
				obsy3 = rand() % 600;
				tmp1 = obsy3;
				c++;
			}break;

	case 2: if (obsx3 > -100) {
		obsx3 -= 1;
		obsy3 = tmp1 + 100 * cos(x1 / 180.0 * 3.14);
		x1++;
	}
			else {
				obsx1 = 1280;
				eshara = 1300;
				obsy1 = rand() % 600;
				red = obsy1 + 50;
				yellow = red - 20;
				green = yellow - 20;
				c++;
			}break;

	case 3: if (obsx1 > -100) {
		obsx1 -= 1;
		eshara -= 1;
	}
			else {
				obsx2 = 1350;
				obsy2 = rand() % 600;
				tmp = obsy2;
				c++;
			}break;
	case 4: if (obsx2 > -100) {
		obsx2 -= 1;
		obsy2 = tmp + 100 * sin(x / 180.0 * 3.14);
		x++;
	}
			else {
				obsx3 = 1280;
				obsy3 = rand() % 600;
				tmp1 = obsy3;
				c++;
			}break;

	case 5: if (obsx3 > -100) {
		obsx3 -= 1;
		obsy3 = tmp1 + 100 * cos(x1 / 180.0 * 3.14);
		x1++;
	}
			else {
				powerx = 1280;
				powery = rand() % 600;
				tmp2 = powery;
				obsx1 = 1280;
				eshara = 1300;
				obsy1 = rand() % 600;
				red = obsy1 + 50;
				yellow = red - 20;
				green = yellow - 20;
				c++;
			}break;

	case 6: if (powerx > -100) {
		powerx--;
		powery = tmp2 + 200 * cos(cos(x2 / 180.0 * 3.14));
		x2++;
	}
			else {
				obsx1 = 1280;
				eshara = 1300;
				obsy1 = rand() % 600;
				red = obsy1 + 50;
				yellow = red - 20;
				green = yellow - 20;
				c = 0;
			}break;
	}

}

void detectCollision1() {
	int busleftreal = 640 + busleft + transx;
	int busrightreal = 640 + busright + transx;
	int busupreal = 360 + busup + transy;
	int busdowntreal = 360 + busdown + transy;

	int maxobsx = obsx1 + 40;
	int maxobsy = obsy1 + 80;


	if (obsx1 > 0) {
		if (obsx1 <= busrightreal && maxobsy >= busdowntreal && obsy1 <= busupreal && obsx1 && busleftreal <= maxobsx) {
			transx -= 200;
			obsx1 -= 20000;
			eshara -= 20000;
			red -= 20000;
			yellow -= 20000;
			green -= 20000;
			currentLevel--;
		}
	}

}

void detectCollision2() {
	int busleftreal = 640 + busleft + transx;
	int busrightreal = 640 + busright + transx;
	int busupreal = 360 + busup + transy;
	int busdowntreal = 360 + busdown + transy;

	int minobsx = obsx2 - 30;
	int maxobsx = obsx2 + 30;
	int maxobsy = obsy2 + 30;
	int minobsy = obsy2 - 30;

	if (minobsx > 0) {
		if (minobsx <= busrightreal && maxobsy >= busdowntreal && minobsy <= busupreal && busleftreal <= maxobsx) {
			transx -= 200;
			obsx2 -= 20000;
			currentLevel--;
		}
	}


}

void detectCollision3() {

	int busleftreal = 640 + busleft + transx;
	int busrightreal = 640 + busright + transx;
	int busupreal = 360 + busup + transy;
	int busdowntreal = 360 + busdown + transy;

	int minobsx = obsx3;
	int maxobsx = obsx3 + 20;
	int maxobsy = obsy3 + 60;
	int minobsy = obsy3;

	if (minobsx > 0) {
		if (minobsx <= busrightreal && maxobsy >= busdowntreal && minobsy <= busupreal && busleftreal <= maxobsx) {
			transx -= 200;
			obsx3 -= 20000;
			currentLevel--;
		}
	}


}

void detectCollision4() {
	int busleftreal = 640 + busleft + transx;
	int busrightreal = 640 + busright + transx;
	int busupreal = 360 + busup + transy;
	int busdowntreal = 360 + busdown + transy;

	int minpowerx = powerx;
	int maxpowerx = powerx + 65;
	int maxpowery = powery + 70;
	int minpowery = powery;

	if (minpowerx > 0) {
		if (minpowerx <= busrightreal && maxpowery >= busdowntreal && minpowery <= busupreal && busleftreal <= maxpowerx) {
			transx += 200;
			powerx -= 20000;
			currentLevel++;
		}
	}


}

void detectGameOver() {
	if (currentLevel == 0 || currentLevel == 6) {
		back3x = 0;
		back3x2 = 1280;
		obsx1 = -200000;
		obsx2 = -200000;
		obsx3 = -2000000;
		powerx = -200000;
		transx = -2000000;
	}
}

void Anim() {
	animateBack();
	moveObstacles();
	detectCollision1();
	detectCollision2();
	detectCollision3();
	detectCollision4();
	detectGameOver();
}
