/*
#include "ssGUI/Backend/Win32_OpenGL3_3/BackendMainWindowWin32_OpenGL3_3.hpp"
#include "ssGUI/Backend/Win32_OpenGL3_3/BackendSystemInputWin32_OpenGL3_3.hpp"
#include "ssGUI/Backend/Win32_OpenGL3_3/BackendDrawingWin32_OpenGL3_3.hpp"

#include <iostream>
#include <vector>


int main()
{
    ssGUI::Backend::BackendDrawingWin32_OpenGL3_3 drawing;
    ssGUI::Backend::BackendMainWindowWin32_OpenGL3_3 window;
    ssGUI::Backend::BackendSystemInputWin32_OpenGL3_3 inputs;

    //std::vector<glm::vec2> pos {glm::ivec2(50, 50), glm::ivec2(100, 50), glm::ivec2(100, 100), glm::ivec2(50, 100)};
    std::vector<glm::vec2> pos {glm::ivec2(100, 100), glm::ivec2(100, 50), glm::ivec2(50, 50) };
    std::vector<glm::vec2> uv;
    std::vector<glm::u8vec4> color;
    std::vector<int> counts { 3};
    std::vector<ssGUI::DrawingProperty> props {ssGUI::DrawingProperty()};

    //drawing.ClearBackBuffer(glm::u8vec4());
    //drawing.DrawEntities(pos, uv, color, counts, props);

    while(!window.IsClosed())
    {
        drawing.ClearBackBuffer(glm::u8vec4());
        //drawing.Render(glm::u8vec3(255, 255, 255));
        inputs.UpdateInput();

        //drawing.DrawEntities(pos, uv, color, counts, props);

        //if(!window.IsClosed())
        //    drawing.Render(glm::u8vec3());
    }

    //std::cout << "Test\n";


    return 0;
}

*/







//OpenGL GLClear memory leak test



/* An example of the minimal Win32 & OpenGL program.  It only works in
   16 bit color modes or higher (since it doesn't create a
   palette). */

#ifndef UNICODE
#define UNICODE
#define _UNICODE
#endif

#include <windows.h>            /* must include this before GL/gl.h */
#include "glad/glad.h"
#include "glad/glad_wgl.h"
#include "wglext.h"
#include <string>

static bool CloseFlag = false;

void display(HDC hDC)
{
    /* rotate a triangle around */
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2i(0,  1);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2i(-1, -1);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2i(1, -1);
    glEnd();
    glFlush();

    SwapBuffers(hDC);
}

void JustClear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}


LONG WINAPI WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{ 
    static PAINTSTRUCT ps;

    switch(uMsg) 
    {
        case WM_SIZE:
	        glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
	        PostMessage(hWnd, WM_PAINT, 0, 0);
	        return 0;

        case WM_CHAR:
	        switch (wParam) 
            {
	            case 27:			/* ESC key */
	                //PostQuitMessage(0);
	                CloseFlag = true;
                    break;
	        }
            return 0;

        case WM_CLOSE:
	        CloseFlag = true;
	        //PostQuitMessage(0);
	        return 0;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam); 
} 

HWND CreateOpenGLWindow(char* title, int x, int y, int width, int height, 
		   BYTE type, DWORD flags)
{
    int         pf;
    HDC         hDC;
    HWND        hWnd;
    WNDCLASS    wc;
    PIXELFORMATDESCRIPTOR pfd;
    static HINSTANCE hInstance = 0;

    /* only register the window class once - use hInstance as a flag. */
    if (!hInstance) {
	    hInstance = GetModuleHandle(NULL);
	    wc.style         = CS_OWNDC;
	    wc.lpfnWndProc   = (WNDPROC)WindowProc;
	    wc.cbClsExtra    = 0;
	    wc.cbWndExtra    = 0;
	    wc.hInstance     = hInstance;
	    wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
	    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	    wc.hbrBackground = NULL;
	    wc.lpszMenuName  = NULL;
	    wc.lpszClassName = L"OpenGL";

	    if (!RegisterClass(&wc)) {

	        return NULL;
	    }
    }

    std::wstring className = L"OpenGL";

    hWnd = CreateWindow(className.c_str(), className.c_str(), WS_OVERLAPPEDWINDOW |
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			x, y, width, height, NULL, NULL, hInstance, NULL);

    if (hWnd == NULL) {
	    return NULL;
    }

    hDC = GetDC(hWnd);

    /* there is no guarantee that the contents of the stack that become
       the pfd are zeroed, therefore _make sure_ to clear these bits. */
    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize        = sizeof(pfd);
    pfd.nVersion     = 1;
    pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | flags;
    pfd.iPixelType   = type;
    pfd.cColorBits   = 32;

    pf = ChoosePixelFormat(hDC, &pfd);
    if (pf == 0) {
	    return 0;
    } 
 
    if (SetPixelFormat(hDC, pf, &pfd) == FALSE) {
	    return 0;
    } 

    DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    ReleaseDC(hWnd, hDC);

    return hWnd;
}    

int main ()
{
    HDC hDC;				/* device context */
    HGLRC hRC;				/* opengl context */
    HWND  hWnd;				/* window */
    MSG   msg;				/* message */



    hWnd = CreateOpenGLWindow("minimal", 0, 0, 256, 256, PFD_TYPE_RGBA, 0);
    if (hWnd == NULL)
	    exit(1);

    hDC = GetDC(hWnd);
    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);

    if(!gladLoadWGL(hDC))
    {
        return -1;
    }

    if (!gladLoadGL())                  //Load Glad
    {
        return -1;
    }

    ShowWindow(hWnd, SW_SHOW);

    while(!CloseFlag)
    {
        //Handles input
        while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
	        TranslateMessage(&msg);
	        DispatchMessage(&msg);
        }

        //Render
        //display(hDC);
        JustClear();
    }

    wglMakeCurrent(NULL, NULL);
    ReleaseDC(hWnd, hDC);
    wglDeleteContext(hRC);
    DestroyWindow(hWnd);

    return 0;
}
