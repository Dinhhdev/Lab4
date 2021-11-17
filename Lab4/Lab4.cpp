// Lab4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Dep\freeglut\glut.h"
#include <sys/timeb.h>
#include <sys/utime.h>
// Hang qui u�c trang thai di chuy�n cla tay va chan

const char BACKWARD_STATE = 0;
const char FORWARD_STATE = 1;


// Index qui udc cho mang (g�c xoay cua tay va chan)
const char LEFT = 0;
const char RIGHT = 1;

/* Trang thai di chuy�n hi�n tai cla tay va chan (BACKWARD_STATE/
FORWARD_STATE)*/

char legStates[2];

char armStates[2];

// G6c xoay hi�n tai cua tay va chan
float legAngles[2];
float armAngles[2];

// G6c xoay hi�n tai cla toan b6� robot
float angle;

int getMilliCount() {
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
}

void sleep(int sleeptime)
{
	int count = 0;
	int beginsleep = getMilliCount();
	while (getMilliCount() - beginsleep < sleeptime)
	{
		count++;
	}
}

//Ham DrawCube d� v� hinh lap phuong tai vi tri chi dinh
void DrawCube(float xPos, float yPos, float zPos)
{
	glPushMatrix(); /* Luu trang thai bi�n d�i hi�n tai vao stack */

	glTranslatef(xPos, yPos, zPos);

	glBegin(GL_QUADS);
	// V� mat tr�n
	glNormal3d(0, 1, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f, 0.0F);

	// V@ mat tru�c
	glNormal3d(0, 0, 1);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);

	// V@� mat b�n phai
	glNormal3d(1, 0, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);

	glVertex3f(0.0f, -1.0f, -1.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);
	// V@ mat b�n trai
	glNormal3d(-1, 0, 0);
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	// V� mat dudi
	glNormal3d(0, - 1, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	// V� mat sau
	glNormal3d(0, 0, -1);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(0.0f, -1.0f, -1.0F);
	glEnd();
	glPopMatrix(); // Quay lai trang thai bi�n d�i da luu.

}

//Ham DrawArm d� v� canh tay (hinh h�p chit nhat) tai vi tri chi dinh
void DrawArm(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f); // T6 mau dd
	glTranslatef(xPos, yPos, zPos);
	glScalef(1.0f, 4.0f, 1.0f); // Kich thu�c 1x4x1
	DrawCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

//Ham DrawHead d� v� dau (hinh h�p chir nhat) tai vi tri chi dinh
void DrawHead(float xPos, float yPos, float zPos)

{
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.8f); // TO mau trang
	glTranslatef(xPos, yPos, zPos);
	glScalef(2.0f, 2.0f, 2.0f); // Kich thud�c 2x2x2
	DrawCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

//Ham DrawTorso d� v� than (hinh h�p chit nhat) tai vi tri chi dinh
void DrawTorso(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 1.0f); // TO mau xanh
	glTranslatef(xPos, yPos, zPos);
	glScalef(3.0f, 5.0f, 2.0f); // Kich thud�c 3x5x2
	DrawCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

//Ham DrawLeg d� v� chan (hinh h6�p chi nhat) tai vi tri chi dinh
void DrawLeg(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f); // TO mau vang
	glTranslatef(xPos, yPos, zPos);
	glScalef(1.0f, 5.0f, 1.0f); // Kich thu�c 1x5x1
	DrawCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

//Ham DrawRobot d� v� toan b� robot tai vi tri chi dinh
void DrawRobot(float xPos, float yPos, float zPos)
{
	glPushMatrix();

	glTranslatef(xPos, yPos, zPos); // Toa dO v� robot

									// V@ phan dau va phan than

	DrawHead(1.0f, 2.0f, 0.0f);

	DrawTorso(1.5f, 0.0f, 0.0f);

	/* Di chuy�n canh tay trai va xoay canh tay d� tao hi�u tng

	�dang budc di�.*/

	glPushMatrix();
	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(armAngles[LEFT], 1.0f, 0.0f, 0.0f);
	DrawArm(2.5f, 0.0f, - 0.5f);

	glPopMatrix();
	/* Di chuy�n canh tay phai va xoay cadnh tay d� tao hi�u tng

	�dang budc di�.*/

	glPushMatrix();
	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(armAngles[RIGHT], 1.0f, 0.0f, 0.0f);
	DrawArm(-1.5f, 0.0f, -0.5f);

	glPopMatrix();

	/* Di chuy�n chan tradi va xoay chan d� tao hi�u tng �dang budc
	di�.*/
	glPushMatrix();
	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(legAngles[LEFT], 1.0f, 0.0f, 0.0f);
	DrawLeg(-0.5f, -5.0f, -0.5f);

	glPopMatrix();

	/* Di chuy�n chan phai va xoay chan d� tao hi�u ung �dang budc

	di�.*/

	glPushMatrix();
	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(legAngles[RIGHT], 1.0f, 0.0f, 0.0f);
	DrawLeg(1.5f, -5.0f, -0.5f);

	glPopMatrix();

	glPopMatrix();
}

//Ham Prepare d� tinh toan cdc g�c xoay cua tay va chan
void Prepare()
{
	/* N�u tay/chan dang di chuy�n v� phia trudc thi tang goc xoay,
	nguoc lai (di chuy�n v� phia sau) thi giam g�c xoay. */
	for (char side = 0; side < 2; side++) {
		// GOc xoay cho tay
		if (armStates[side] == FORWARD_STATE)
			armAngles[side] += 0.1f;
		else
			armAngles[side] -= 9.1f;
		// Thay d�i trang thai n�u g�c xoay vuot quad gid tri cho ph�p
		if (armAngles[side] >= 15.0f)

			armStates[side] = BACKWARD_STATE;
		else if (armAngles[side] <= -15.0f)

			armStates[side] = FORWARD_STATE;

		// G6c xoay cho chan

		if (legStates[side] == FORWARD_STATE)
			legAngles[side] += 0.1f;

		else
			legAngles[side] -= 9.1f;

		// Thay d�i trang thai n�u g�c xoay vuot qua gid tri cho ph�p
		if (legAngles[side] >= 15.0f)

			legStates[side] = BACKWARD_STATE;
		else if (legAngles[side] <= -15.0f)

			legStates[side] = FORWARD_STATE;
	}

}

//Ham Display d� dung hinh
	void Display()
	{
		int beginFrame = getMilliCount();
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		angle = angle + 0.05f; // Tang gdc xoay cua robot
			if (angle >= 360.0f) // N�u xoay du vong, reset g�dc xoay
				angle = 0.0f;

		glPushMatrix();
		// Xoay robot quanh truc y
		glRotatef(angle, 0.0f, 1.6f, 0.0f);
		Prepare();
		DrawRobot(0.0f, 0.0f, 0.0f); // V� robot
		glPopMatrix();

		glFlush();
		glutSwapBuffers();

		/*int timeDiff = getMilliCount() - beginFrame;
		if (timeDiff < 20)
		{
			sleep(20 - timeDiff);
		}*/
	}

//Ham Init d� thi�t lap anh sang m�i tru�ng va khoi tao gid tri mac dinh cho obot
	void Init()

	{

		glEnable(GL_DEPTH_TEST);
		glShadeModel(GL_SMOOTH);

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		// Mau anh sang ambient

		GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		// Mau anh sang diffuse

		GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		// Vi tri ngu6�n sang

		GLfloat lightPos[] = { 25.0f, 25.0f, 25.0f, 0.0 };
		// Hudng chi�u sang

		GLfloat spotDir[] = { 0.0, 0.0, 0.0, 0.0 };

		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir);

		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		glClearColor(0.0, 0.0, 0.0, 0.0);

		// Gan gia tri mac dinh ban dau cho robot
		angle = 0.0f;

		armAngles[LEFT] = 0.0;

		armAngles[RIGHT] = 0.0;

		legAngles[LEFT] = 0.0;

		legAngles[RIGHT] = 0.0;

		armStates[LEFT] = FORWARD_STATE;
		armStates[RIGHT] = BACKWARD_STATE;
		legStates[LEFT] = FORWARD_STATE;
		legStates[RIGHT] = BACKWARD_STATE;

	}

//Ham Reshape d� thi�t lap khung nhin, ph�p chi�u, va camera
	void Reshape(int Width, int Height)

	{
		glViewport(0, 0, (GLsizei)Width, (GLsizei)Height);

		glMatrixMode(GL_PROJECTION);

		glLoadIdentity();

		gluPerspective(45.0, (GLfloat)Width / (GLfloat)Height, 1.0,
			200.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(5.0, 5.0, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	}

	void keyboard(unsigned char key, int x, int y) {
		switch (key) {
		case 'b':
			
			glutPostRedisplay();
			break;
		case '1':
			

			glutPostRedisplay();
			break;


		default:          
			break;
		}
	}

//Ham Idle d� y�u cau v� lai man hinh li�n tuc
	void Idle()
	{
		glutPostRedisplay();
	}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(800, 600);	
	glutCreateWindow("Robot_Buoc_Di_Lab01");

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(Idle);
	Init();
	glutMainLoop();
	return 0;


}

