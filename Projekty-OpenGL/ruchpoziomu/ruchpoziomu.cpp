#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>
#pragma comment(lib, "legacy_stdio_definitions.lib") 

HDC			hDC = NULL;
HGLRC		hRC = NULL;	
HWND		hWnd = NULL;
HINSTANCE	hInstance;

bool	keys[256];			// klawiatura 
bool	active = TRUE;
bool	fullscreen = FALSE;	// wylaczony fullscreen

bool przyspieszanie = true;
GLfloat x = -2.0f;				//lewo prawo kuli
GLfloat y = 6.0f;			//gora dol kuli

GLfloat vx = 0.0f;	//pred pozioma
GLfloat ax = -0.005f;//przysp


GLfloat vy = 0.0f;  //pred pionowa
GLfloat ay = -0.01f; //przysp


GLfloat odlegloscKam = -50.0f;//kamera
GLfloat angX = 25.0f;
GLfloat angY = -55.0f;
GLfloat predKamery = 0.3F; //pred kamery



LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)
{
	if (height == 0)
	{
		height = 1;
	}

	glViewport(0, 0, width, height);// Reset Viewport

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int InitGL(GLvoid)
{
	glShadeModel(GL_SMOOTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


	return TRUE;
}

int DrawGLScene(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, odlegloscKam - angX / 3);//im bardziej z gory tym dalej kamera
	glRotatef(angX, 1.0f, 0.0f, 0.0f);//obrot 20st w dol
	glRotatef(angY, 0.0f, 1.0f, 0.0f);//obrot 20st w dol


	glPushMatrix();//rysowanie podstawy
	glTranslatef(0.0f, -0.5f, 0.0f);
	glColor3f(0.5f, 0.3f, 0.5f);
	auxSolidBox(60, 1, 20);
	glPopMatrix();


	glTranslatef(-27.0f, 0.0f, 0.0f);//rysowanie box'a
	glPushMatrix();
	glTranslatef(0.0f, 3.0f, 0.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	auxSolidCube(6);
	glPopMatrix();


	glTranslatef(x, y + 1, 0);//rysowanie kuli
	glColor3f(0.7f, 0.0f, 0.0f);
	auxSolidSphere(1);

	return TRUE;
}

GLvoid KillGLWindow(GLvoid)
{
	
	if (hRC)
	{
		if (!wglMakeCurrent(NULL, NULL))
		{
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))
		{
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;
	}

	if (hDC && !ReleaseDC(hWnd, hDC))
	{
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;
	}

	if (hWnd && !DestroyWindow(hWnd))
	{
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;
	}

	if (!UnregisterClass("OpenGL", hInstance))
	{
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;
	}
}
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;
	WNDCLASS	wc;//struktura okna windows
	DWORD		dwExStyle;
	DWORD		dwStyle;//styl okna
	RECT		WindowRect;
	WindowRect.left = (long)0;			// Set Left Value To 0
	WindowRect.right = (long)width;		// Set Right Value To Requested Width
	WindowRect.top = (long)0;				// Set Top Value To 0
	WindowRect.bottom = (long)height;		// Set Bottom Value To Requested Height

	hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = (WNDPROC)WndProc;					// WndProc Handles Messages
	wc.cbClsExtra = 0;									// No Extra Window Data
	wc.cbWndExtra = 0;									// No Extra Window Data
	wc.hInstance = hInstance;							// Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "OpenGL";

	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}

	if (fullscreen)
	{
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;
		ShowCursor(FALSE);
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW;
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);

	// Create The Window
	if (!(hWnd = CreateWindowEx(dwExStyle,
		"OpenGL",
		title,								// Window Title
		dwStyle |							// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		400, 200,							// Window Position
		WindowRect.right - WindowRect.left,	// Calculate Window Width
		WindowRect.bottom - WindowRect.top,	// Calculate Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		hInstance,							// Instance
		NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),	// Size Of This Pixel Format Descriptor
		1,								// Version Number
		PFD_DRAW_TO_WINDOW |			// Format Must Support Window
		PFD_SUPPORT_OPENGL |			// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,				// Must Support Double Buffering
		PFD_TYPE_RGBA,					// Request An RGBA Format
		bits,							// Select Our Color Depth
		0, 0, 0, 0, 0, 0,				// Color Bits Ignored
		0,								// No Alpha Buffer
		0,								// Shift Bit Ignored
		0,								// No Accumulation Buffer
		0, 0, 0, 0,						// Accumulation Bits Ignored
		16,								// 16Bit Z-Buffer (Depth Buffer)  
		0,								// No Stencil Buffer
		0,								// No Auxiliary Buffer
		PFD_MAIN_PLANE,					// Main Drawing Layer
		0,								// Reserved
		0, 0, 0							// Layer Masks Ignored
	};

	if (!(hDC = GetDC(hWnd)))
	{
		KillGLWindow();
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))
	{
		KillGLWindow();
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))
	{
		KillGLWindow();
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!(hRC = wglCreateContext(hDC)))	// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;			
	}

	if (!wglMakeCurrent(hDC, hRC))		// Try To Activate The Rendering Context
	{
		KillGLWindow();
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	ShowWindow(hWnd, SW_SHOW);			// Show The Window
	SetForegroundWindow(hWnd);			// Slightly Higher Priority
	SetFocus(hWnd);						// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);		// Set Up Our Perspective GL Screen

	if (!InitGL())
	{
		KillGLWindow();
		MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND	hWnd,// Handle For This Window
	UINT	uMsg,			// Message For This Window
	WPARAM	wParam,			// Additional Message Information
	LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)			// Check For Windows Messages
	{
	case WM_ACTIVATE:		// Watch For Window Activate Message
	{
		if (!HIWORD(wParam))
		{
			active = TRUE;
		}
		else
		{
			active = FALSE;	
		}

		return 0;
	}

	case WM_SYSCOMMAND:
	{
		switch (wParam)
		{
		case SC_SCREENSAVE:
		case SC_MONITORPOWER:
			return 0;
		}
		break;
	}

	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	case WM_KEYDOWN:
	{
		keys[wParam] = TRUE;
		return 0;
	}

	case WM_KEYUP:
	{
		keys[wParam] = FALSE;
		return 0;
	}

	case WM_SIZE:
	{
		ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));
		return 0;
	}
	}


	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPSTR		lpCmdLine,
	int			nCmdShow)
{
	MSG		msg;
	BOOL	done = FALSE;

	MessageBox(
		NULL,
		"sterowanie: a-przyśpieszenie\nr-reset sceny\nsterowanie kamerą-strzałki",
		"sterowanie kulą",
		MB_OK
	);

	//Stworz okno
	if (!CreateGLWindow((char*)"rzut poziomy", 640, 480, 16, fullscreen))
	{
		return 0;
	}
	int lastTick = 0;
	while (!done)
	{

		bool wyrenderowano = false;
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				done = TRUE;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (active)
			{
				if (keys[VK_ESCAPE])
				{
					done = TRUE;
				}
				else
				{
					int tick = GetTickCount();
					if (tick - lastTick >= 20)
					{
						DrawGLScene();
						SwapBuffers(hDC);
						lastTick = tick;
						wyrenderowano = true;
					}

				}
			}

			if (wyrenderowano)
			{
				
				if (keys[VK_UP])
				{
					angX = angX + predKamery;
					if (angX > 40) angX = 40;
				}
				if (keys[VK_DOWN])
				{
					angX = angX - predKamery;
					if (angX < 0) angX = 0;
				}
				if (keys[VK_RIGHT])
				{
					angY = angY + predKamery;
					if (angY > 360) angY = angY - 360;
					if (angY < 0) angY = angY + 360;
				}
				if (keys[VK_LEFT])
				{
					angY = angY - predKamery;
					if (angY > 360) angY = angY - 360;
					if (angY < 0) angY = angY + 360;
				}
				if (keys['R'])//reset kamery
				{
					x = -2.0f;//reset pozycji
					y = 6.0f;

					vx = 0.0f;//reset predkosci
					vy = 0.0f;

					ay = -0.02f;//reset przyspieszenia
					ax = -0.005f;
					przyspieszanie = true;


				}
				if (keys['A'])// predkosc
				{
					if (przyspieszanie)//przyspieszenie
						vx = vx + 0.05f;

				}
				//"fizyka kuli"
				if (przyspieszanie)	// if kula na szczescianie
				{
					if (x > 4) //kula minela box'a
					{
						przyspieszanie = false;
					}
				}
				else //jezeli kula juz spada
				{
					vx = vx + ax; // a<0 spowalnia
					if (vx < 0.01) // kiedy predkosc mniejsza od 0.01
					{
						vx = 0; //predc=0
						ax = 0; //przys=0
					}

					if (y == 0) //dotkniecie ziemi
					{
						vy = -0.7 * vy;//70% predkosci i zmiana kierunku 
						vx = 0.7 * vx; //70% predkosci poziomej
						if (vy < 0.05) // kulka sie nie odbie
							vy = 0;		//predkosc pionu
					}


					vy = vy + ay;  //predkosc zwiekszona o przysp(wzrasta jak spada, maleje jak sie wznosi
				}

				x = x + vx;		//przesuniecie po X
				y = y + vy;		//przesuniecie po Y
				if (y < 0) y = 0; // jak spadnie ponizej Y to stop na Y=0

			}
		}
	}

	
	KillGLWindow();
	return (msg.wParam);//zakoncz program
}