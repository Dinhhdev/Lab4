// Lab4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Dep\freeglut\glut.h"
#include <sys/timeb.h>
#include <sys/utime.h>
// Hang qui uéc trang thai di chuyén cla tay va chan

const char BACKWARD_STATE = 0;
const char FORWARD_STATE = 1;


// Index qui udc cho mang (géc xoay cua tay va chan)
const char LEFT = 0;
const char RIGHT = 1;

/* Trang thai di chuyén hién tai cla tay va chan (BACKWARD_STATE/
FORWARD_STATE)*/

char legStates[2];

char armStates[2];

// G6c xoay hién tai cua tay va chan
float legAngles[2];
float armAngles[2];

// G6c xoay hién tai cla toan b6é robot
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

//Ham DrawCube dé vé hinh lap phuong tai vi tri chi dinh
void DrawCube(float xPos, float yPos, float zPos)
{
	glPushMatrix(); /* Luu trang thai bién déi hién tai vao stack */

	glTranslatef(xPos, yPos, zPos);

	glBegin(GL_QUADS);
	// Vé mat trén
	glNormal3d(0, 1, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f, 0.0F);

	// V@ mat truéc
	glNormal3d(0, 0, 1);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);

	// V@é mat bén phai
	glNormal3d(1, 0, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);

	glVertex3f(0.0f, -1.0f, -1.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);
	// V@ mat bén trai
	glNormal3d(-1, 0, 0);
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	// V® mat dudi
	glNormal3d(0, - 1, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	// Vé mat sau
	glNormal3d(0, 0, -1);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(0.0f, -1.0f, -1.0F);
	glEnd();
	glPopMatrix(); // Quay lai trang thai bién déi da luu.

}

//Ham DrawArm dé vé canh tay (hinh hép chit nhat) tai vi tri chi dinh
void DrawArm(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f); // T6 mau dd
	glTranslatef(xPos, yPos, zPos);
	glScalef(1.0f, 4.0f, 1.0f); // Kich thuéc 1x4x1
	DrawCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

//Ham DrawHead dé vé dau (hinh hép chir nhat) tai vi tri chi dinh
void DrawHead(float xPos, float yPos, float zPos)

{
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.8f); // TO mau trang
	glTranslatef(xPos, yPos, zPos);
	glScalef(2.0f, 2.0f, 2.0f); // Kich thudéc 2x2x2
	DrawCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

//Ham DrawTorso dé vé than (hinh hép chit nhat) tai vi tri chi dinh
void DrawTorso(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 1.0f); // TO mau xanh
	glTranslatef(xPos, yPos, zPos);
	glScalef(3.0f, 5.0f, 2.0f); // Kich thudéc 3x5x2
	DrawCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

//Ham DrawLeg dé vé chan (hinh h6ép chi nhat) tai vi tri chi dinh
void DrawLeg(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f); // TO mau vang
	glTranslatef(xPos, yPos, zPos);
	glScalef(1.0f, 5.0f, 1.0f); // Kich thuéc 1x5x1
	DrawCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

//Ham DrawRobot dé vé toan bé robot tai vi tri chi dinh
void DrawRobot(float xPos, float yPos, float zPos)
{
	glPushMatrix();

	glTranslatef(xPos, yPos, zPos); // Toa dO vé robot

									// V@ phan dau va phan than

	DrawHead(1.0f, 2.0f, 0.0f);

	DrawTorso(1.5f, 0.0f, 0.0f);

	/* Di chuyén canh tay trai va xoay canh tay dé tao hiéu tng

	“dang budc di”.*/

	glPushMatrix();
	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(armAngles[LEFT], 1.0f, 0.0f, 0.0f);
	DrawArm(2.5f, 0.0f, - 0.5f);

	glPopMatrix();
	/* Di chuyén canh tay phai va xoay cadnh tay dé tao hiéu tng

	“dang budc di”.*/

	glPushMatrix();
	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(armAngles[RIGHT], 1.0f, 0.0f, 0.0f);
	DrawArm(-1.5f, 0.0f, -0.5f);

	glPopMatrix();

	/* Di chuyén chan tradi va xoay chan dé tao hiéu tng “dang budc
	di”.*/
	glPushMatrix();
	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(legAngles[LEFT], 1.0f, 0.0f, 0.0f);
	DrawLeg(-0.5f, -5.0f, -0.5f);

	glPopMatrix();

	/* Di chuyén chan phai va xoay chan dé tao hiéu ung “dang budc

	di”.*/

	glPushMatrix();
	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(legAngles[RIGHT], 1.0f, 0.0f, 0.0f);
	DrawLeg(1.5f, -5.0f, -0.5f);

	glPopMatrix();

	glPopMatrix();
}

//Ham Prepare dé tinh toan cdc géc xoay cua tay va chan
void Prepare()
{
	/* Néu tay/chan dang di chuyén vé phia trudc thi tang goc xoay,
	nguoc lai (di chuyén vé phia sau) thi giam géc xoay. */
	for (char side = 0; side < 2; side++) {
		// GOc xoay cho tay
		if (armStates[side] == FORWARD_STATE)
			armAngles[side] += 0.1f;
		else
			armAngles[side] -= 9.1f;
		// Thay déi trang thai néu géc xoay vuot quad gid tri cho phép
		if (armAngles[side] >= 15.0f)

			armStates[side] = BACKWARD_STATE;
		else if (armAngles[side] <= -15.0f)

			armStates[side] = FORWARD_STATE;

		// G6c xoay cho chan

		if (legStates[side] == FORWARD_STATE)
			legAngles[side] += 0.1f;

		else
			legAngles[side] -= 9.1f;

		// Thay déi trang thai néu géc xoay vuot qua gid tri cho phép
		if (legAngles[side] >= 15.0f)

			legStates[side] = BACKWARD_STATE;
		else if (legAngles[side] <= -15.0f)

			legStates[side] = FORWARD_STATE;
	}

}

//Ham Display dé dung hinh
	void Display()
	{
		int beginFrame = getMilliCount();
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		angle = angle + 0.05f; // Tang gdc xoay cua robot
			if (angle >= 360.0f) // Néu xoay du vong, reset gédc xoay
				angle = 0.0f;

		glPushMatrix();
		// Xoay robot quanh truc y
		glRotatef(angle, 0.0f, 1.6f, 0.0f);
		Prepare();
		DrawRobot(0.0f, 0.0f, 0.0f); // Vé robot
		glPopMatrix();

		glFlush();
		glutSwapBuffers();

		/*int timeDiff = getMilliCount() - beginFrame;
		if (timeDiff < 20)
		{
			sleep(20 - timeDiff);
		}*/
	}

//Ham Init dé thiét lap anh sang méi truéng va khoi tao gid tri mac dinh cho obot
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
		// Vi tri ngu6én sang

		GLfloat lightPos[] = { 25.0f, 25.0f, 25.0f, 0.0 };
		// Hudng chiéu sang

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

//Ham Reshape dé thiét lap khung nhin, phép chiéu, va camera
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

//Ham Idle dé yéu cau vé lai man hinh lién tuc
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

