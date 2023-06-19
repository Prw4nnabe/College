#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>

#pragma comment(lib, "legacy_stdio_definitions.lib")


HDC g_HDC;
bool fullScreen = false;
DWORD	tickCount, lasttickCount;
float Rot = 0;
float Move = 1;
float changMove = 0.001f;
int l = 0, tryb = 1, m = 0;
float Angle = 0;
float Red = 0.0, Green = 0.0, changRed = 0.001f;


float ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
float diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightPosition[] = { 1.0f, 0.0f, 1.0f, 0.0f };



void Initialize()       // Inicjacja OpenGL
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glEnable(GL_LIGHT0);
}

void DrawCube(float xPos, float yPos, float zPos, float xRot, float yRot, float zRot, float Red, float Green)
{
	glPushMatrix();
	glTranslatef(xPos, yPos, zPos);
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 1.0f, 0.0f);	// góra XZ
	glColor3f(Red, Green, 1.0f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);	// przód XY
	glColor3f(Red, Green, 1.0f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);

	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(1.0f, 0.0f, 0.0f);	// prawo YZ
	glColor3f(Red, Green, 1.0f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);

	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(-1.0f, 0.0f, 0.0f);	// lewo YZ
	glColor3f(Red, Green, 1.0f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);

	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, -1.0f, 0.0f);	// dół XZ
	glColor3f(Red, Green, 1.0f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, -1.0f);	// tył XY
	glColor3f(Red, Green, 1.0f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glEnd();
	glPopMatrix();
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();


	glTranslatef(0.0f, 0.0f, -7.0f);
	glRotatef(30, 1.0f, 0.0f, 0.0f);
	glRotatef(30, 0.0f, 1.0f, 0.0f);
	glRotatef(0, 0.0f, 0.0f, 1.0f);

	if (tryb == 1)
	{
		if (Rot > 360)
			Rot -= 360.0f;
		else
			Rot -= 0.057f;
		Angle += 0.001f;
	}

	if (tryb == 2)
	{
		if (Rot > 360)
			Rot -= 360.0f;
		else
			Rot -= 0.065f;
	}

	if (tryb == 3)
	{
		Move = Move + changMove;
		l += 1;
		if (l % 1000 == 0)
			changMove = changMove * (-1);
	}

	if (tryb == 4)
	{
		Red = Red + changRed;

		m += 1;
		if (m % 500 == 0)
		{
		
		changRed = changRed * (-1);
		}
	}

	for (float i = -1.5f; i <= 1.5f; i += 1.5f)//k jaka sciana //j jak wysoko //i jak daleko
	{
		for (float j = -1.5f; j <= 1.5f; j += 1.5f)
		{
			for (float k = -1.5f; k <= 1.5f; k += 1.5f)
			{
				if (k == -1.5 && i == -1.5 && j == 1.5f or k==0 && i==0 && j==1.5 or k==1.5 && i==1.5 && j==1.5)
					DrawCube(i * Move * cos(Angle) - k * sin(Angle), j * Move, k * Move * cos(Angle) + i * sin(Angle), 0.0f, Rot, 0.0f, Red, Green);
				else
					DrawCube(i * Move * cos(Angle) - k * sin(Angle), j * Move, k * Move * cos(Angle) + i * sin(Angle), 0.0f, Rot, 0.0f, 0, 0);

			}
		}
	}

	glFlush();					
	SwapBuffers(g_HDC);		
}


void SetupPixelFormat(HDC hDC) // format pikseli
{
	int nPixelFormat;		

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,								
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,								
		0, 0, 0, 0, 0, 0,				
		0,							
		0,							
		0,							
		0, 0, 0, 0,				
		16,							
		0,							
		0,							
		PFD_MAIN_PLANE,				
		0,							
		0, 0, 0 };					

	nPixelFormat = ChoosePixelFormat(hDC, &pfd);	// wybiera najbardziej zgodny format pikseli 
	SetPixelFormat(hDC, nPixelFormat, &pfd);		// określa format pikseli dla danego kontekstu urządzenia
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)  // Procedura okienkowa
{
	static HGLRC hRC;					// kontekst tworzenia grafiki
	static HDC hDC;						// kontekst urządzenia
	int width, height;					// szerokość i wysokość okna

	switch (message)						// obsługa komunikatów
	{
	case WM_CREATE:					// Utworzenie okna
		hDC = GetDC(hwnd);			// pobiera kontekst urządzenia dla okna
		g_HDC = hDC;
		SetupPixelFormat(hDC);		// wywołuje funkcję określającą format pikseli
		// tworzy kontekst grafiki i czyni go bieżącym
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);
		return 0;
		break;

	case WM_CLOSE:					// Zamknięcie okna
		wglMakeCurrent(hDC, NULL);  // usuwa kontekst renderowania okna
		wglDeleteContext(hRC);
		PostQuitMessage(0);			// wysyła WM_QUIT do kolejki komunikatów
		return 0;
		break;

	case WM_SIZE:					// Zmiana wymiarów okna
		height = HIWORD(lParam);
		width = LOWORD(lParam);
		if (height == 0)				// zabezpieczenie przed dzieleniem przez 0
			height = 1;
		glViewport(0, 0, width, height);		// nadanie nowych wymiarów okna OpenGL
		glMatrixMode(GL_PROJECTION);			// przełączenie macierzy rzutowania
		glLoadIdentity();						// zresetowanie macierzy rzutowania
		gluPerspective(66.0f, (GLfloat)width / (GLfloat)height, 1.0f, 1000.0f);  // wyznaczenie proporcji obrazu i ustawienie rzutowania perspektywicznego
		glMatrixMode(GL_MODELVIEW);				// przełączenie macierzy modelowania
		glLoadIdentity();						// zresetowanie macierzy modelowania
		return 0;
		break;

	default:
		break;
	}

	return (DefWindowProc(hwnd, message, wParam, lParam));
}

// Funkcja główna, od której rozpoczyna się wykonywanie aplikacji
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASSEX windowClass;		// klasa okna
	HWND	   hwnd;			// uchwyt okna
	MSG		   msg;				// komunikat
	bool	   done;			// znacznik zakończenia aplikacji
	DWORD	   dwExStyle;		// rozszerzony styl okna
	DWORD	   dwStyle;			// styl okna
	RECT	   windowRect;		// rozmiar okna

	// parametry okna
	int width = 800;
	int height = 600;
	int bits = 32;

	// fullScreen = TRUE/FALSE;		// Przełącznik aplikacja okienkowa/aplikacja pełnoekranowa

	windowRect.left = (long)0;						// struktura określająca rozmiary okna
	windowRect.right = (long)width;
	windowRect.top = (long)0;
	windowRect.bottom = (long)height;

	// definicja klasy okna
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// domyślna ikona
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);		// domyślny kursor
	windowClass.hbrBackground = NULL;						// bez tła
	windowClass.lpszMenuName = NULL;						// bez menu
	windowClass.lpszClassName = "OpenGL";
	windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);		// logo Windows


	if (!RegisterClassEx(&windowClass))
		return 0;


	if (fullScreen)
	{
		dwExStyle = WS_EX_APPWINDOW;			// rozszerzony styl okna
		dwStyle = WS_POPUP;						// styl okna
		ShowCursor(FALSE);						// ukrycie kursora myszy
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	// definicja klasy okna
		dwStyle = WS_OVERLAPPEDWINDOW;					// styl okna
	}
	MessageBox(
		NULL,
		"sterowanie: 1- obrót całą bryłą\n2-obrót pojedynczych sześcianów\n3-rozsuwanie\n4-zmiana koloru",
		"sterowanie sześcianem",
		MB_OK
	);
	
	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);		// skorygowanie rozmiaru okna
	// utworzenie okna
	hwnd = CreateWindowEx(NULL,									// styl rozszerzony
		"OpenGL",							// nazwa klasy
		"sześciany",						// nazwa aplikacji
		dwStyle | WS_CLIPCHILDREN |
		WS_CLIPSIBLINGS,
		350, 200,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,	// szerokość, wysokość
		NULL,									// uchwyt okna nadrzędnego
		NULL,									// uchwyt menu
		hInstance,
		NULL);

	if (!hwnd)	// sprawdzenie, czy utworzenie okna nie powiodło się (wtedy wartość hwnd równa NULL)
		return 0;

	ShowWindow(hwnd, SW_SHOW);			// wyświetlenie okna
	UpdateWindow(hwnd);					// aktualizacja okna

	done = false;						// inicjacja zmiennej warunku pętli przetwarzania komunikatów
	Initialize();						// inicjacja OpenGL

	while (!done)	// pętla przetwarzania komunikatów
	{
		PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		if (msg.message == WM_QUIT)		// aplikacja otrzymała komunikat WM_QUIT?
			done = true;				// jeśli tak, to kończy działanie
		else
		{
			Render();					// jeśli nie to renderuje scenę,
			TranslateMessage(&msg);		// tłumaczy komunikat i wysyła do systemu
			DispatchMessage(&msg);
		}
		if (msg.message == WM_CHAR)
		{
			switch (msg.wParam)
			{
			case 0x31:
			{
				tryb = 1;
				break;
			}
			case 0x32:
			{
				tryb = 2;
				break;
			}
			case 0x33:
			{
				tryb = 3;
				break;
			}
			case 0x34:
			{
				tryb = 4;
				break;
			}
			}
		}
	}

	if (fullScreen)						// jeżeli był tryb pełnoekranowy
	{
		ChangeDisplaySettings(NULL, 0);	// to przywrócenie pulpitu
		ShowCursor(TRUE);				// i wskaźnika myszy
	}

	return msg.wParam;
}
