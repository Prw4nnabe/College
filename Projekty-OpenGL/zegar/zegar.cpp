#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <WinUser.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <time.h>
#include <sys/timeb.h>
#include <string.h>

int PI = 3.14159;

GLUquadricObj* Hand; //definiowanie wsk do objektu
GLUquadricObj* Disk;

struct tm* newtime;
time_t ltime;

GLfloat rx, ry, rz, angle; //opengl float

static int view_state = 1; //tryb widoku


static void TimeEvent(int te)
{

	rx = 30 * cos(angle);
	ry = 30 * sin(angle);
	rz = 30 * cos(angle);
	angle += 0.01;
	if (angle > 2 * PI) angle = 0;

	glutPostRedisplay();
	glutTimerFunc(100, TimeEvent, 1);
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);//kolor

	glEnable(GL_DEPTH_TEST);//porownaj i aktualnij bufor glebokowsi

	Hand = gluNewQuadric();// nowy obiekt
	gluQuadricDrawStyle(Hand, GLU_FILL);//styl powierzchni
	gluQuadricNormals(Hand, GLU_SMOOTH);//normalne wektory
	gluQuadricOrientation(Hand, GLU_OUTSIDE);//orientacja na zewnatrz
	gluQuadricTexture(Hand, GL_TRUE);//sa tworzone wsp tekstury

	Disk = gluNewQuadric();
	gluQuadricDrawStyle(Disk, GLU_FILL);
	gluQuadricNormals(Disk, GLU_SMOOTH);
	gluQuadricOrientation(Disk, GLU_OUTSIDE);
	gluQuadricTexture(Disk, GL_TRUE);
}

void Draw_clock(GLfloat cx, GLfloat cy, GLfloat cz)
{

	int hour_ticks;
	glPushMatrix();

	glTranslatef(cx, cy, cz);
	glRotatef(180, 1.0, 0.0, 0.0);
	glPushMatrix(); //rysuj tarcze
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(0, 0, 0.3);
	gluDisk(Disk, 0, 7, 12, 16);
	glPopMatrix();

	glPushMatrix();//rysuj ramie godzin
	glColor3f(0, 0, 0);
	glTranslatef(0, 0, 0.0);
	glRotatef((360 / 12) * newtime->tm_hour + (360 / 60) * (60 / (newtime->tm_min + 1)), 0.0, 0.0, 1.0);
	glPushMatrix();
	glTranslatef(0.0, 0.0, 2.0);
	glPopMatrix();
	glRotatef(90, 1.0, 0.0, 0.0);
	gluCylinder(Hand, 0.15, 0, 4, 16, 16);
	glPopMatrix();

	glPushMatrix();//rysuj ramie minut
	glColor3f(0, 0, 0);
	glTranslatef(0, 0, 0.0);
	glRotatef((360 / 60) * newtime->tm_min, 0.0, 0.0, 1.0);
	glPushMatrix();
	glTranslatef(0.0, 0.0, 3.0);
	glScalef(0.5, 0.5, 1.0);
	glPopMatrix();
	glRotatef(90, 1.0, 0.0, 0.0);
	gluCylinder(Hand, 0.1, 0, 6, 16, 16);
	glPopMatrix();

	glPushMatrix();//rysuj ramie sekund
	glColor3f(1.0, 0.0, 0.5);
	glTranslatef(0, 0, -0.0);
	glRotatef((360 / 60) * newtime->tm_sec, 0.0, 0.0, 1.0);
	glPushMatrix();
	glTranslatef(0.0, 0.0, 4.0);
	glScalef(0.25, 0.25, 1.0);
	glPopMatrix();
	glRotatef(90, 1.0, 0.0, 0.0);
	gluCylinder(Hand, 0.05, 0, 6, 16, 16);
	glPopMatrix();


	for (hour_ticks = 0; hour_ticks < 12; hour_ticks++)
	{
		glPushMatrix();// rysuj nast ramie
		glColor3f(0.2, 0.2, 0.4); 
		glTranslatef(0.0, 0.0, 0.0);
		glRotatef((360 / 12) * hour_ticks, 0.0, 0.0, 1.0);
		glTranslatef(6.0, 0.0, 0.0);
		glutSolidCube(0.5);

		glPopMatrix();
	}
	glPopMatrix();

}


void display(void)
{

	time(&ltime); // pobierz czas
	newtime = localtime(&ltime); // przekonwertuj na lokalny

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-8.0, 8.0, -8.0, 8.0, 1.0, 60.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();



	if (view_state == 1)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0, 1, 1.0, 60.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(rx, 0.0, rz, 0.0, 0.0, -14.0, 0, 1, 0);
	}
	Draw_clock(0.0, 0.0, -14.0);
	glutSwapBuffers();
}


void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		view_state = abs(view_state - 1);
		break;
	case 27:
		exit(0); //[ESC] 
		break;
	default:
		break;
	}
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	int DisplayResourceNAMessageBox();
	{
		int msgboxID = MessageBox
		(
			NULL,
			(LPCWSTR)L"sterowanie:\na-wyłączenie/włączenie trybu perspektywy\n esc-wyjście",
			(LPCWSTR)L"Zegar-wiadomość",
			MB_OK
		);

		switch (msgboxID)
		{
		case IDOK:
			glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
			glutInitWindowSize(500, 500);
			glutInitWindowPosition(500, 250);
			glutCreateWindow(argv[0]);
			glutSetWindowTitle("Zegar");
			init();
			glutDisplayFunc(display);
			glutReshapeFunc(reshape);
			glutKeyboardFunc(keyboard);
			glutTimerFunc(10, TimeEvent, 1);
			glutMainLoop();
			break;
		}

		return msgboxID;
	}

	return 0;
}

