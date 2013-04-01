#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <Windows.h>

using namespace std;

bool alive = true;

namespace gl
{
    HMODULE opengl = NULL;

    HGLRC (_stdcall *wglCreateContext)(HDC);
    BOOL (_stdcall *wglMakeCurrent)(HDC, HGLRC);
    BOOL (_stdcall *wglDeleteContext)(HGLRC);
    PROC (_stdcall *wglGetProcAddress)(LPCSTR) = NULL;
    HGLRC (_stdcall *wglCreateContextAttribs)(HDC, HGLRC, const int *);
    BOOL (_stdcall *wglChoosePixelFormat)(HDC, const int *, const FLOAT *, UINT, int *, UINT *);

#define WGL_CONTEXT_DEBUG_BIT                   0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT      0x0002
#define WGL_CONTEXT_MAJOR_VERSION               0x2091
#define WGL_CONTEXT_MINOR_VERSION               0x2092
#define WGL_CONTEXT_LAYER_PLANE                 0x2093
#define WGL_CONTEXT_FLAGS                       0x2094
#define WGL_CONTEXT_PROFILE_MASK                0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT            0x0001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT   0x0002

#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif
#ifndef GLAPI
#define GLAPI extern
#endif

    typedef unsigned int GLenum;
    typedef bool GLboolean;
    typedef unsigned int GLbitfield;
    typedef signed char GLbyte;
    typedef signed short GLshort;
    typedef signed int GLint;
    typedef signed int GLsize;
    typedef unsigned char GLubyte;
    typedef unsigned short GLushort;
    typedef unsigned int GLuint;
    typedef unsigned short GLhalf;
    typedef float GLfloat;
    typedef float GLclampf;
    typedef double GLdouble;
    typedef double GLclampd;
    typedef void GLvoid;
#if defined(_M_X64) || defined(__amd64__)
    typedef signed long long GLintptr;
    typedef signed long long GLsizeiptr;
#elif defined(_M_IX86) || defined(__i386__)
    typedef signed int GLintptr;
    typedef signed int GLsizeiptr;
#else
#error No define specifying architecture
#endif

#define GL_FALSE 0
#define GL_TRUE  1

    PROC bind(LPCSTR name, bool optional)
    {
        if (opengl == NULL)
        {
            opengl = LoadLibraryEx("OpenGL32.dll", NULL, 0);
        }

        if (opengl == NULL)
        {
            cerr << "Failed to open OpenGL32.dll while searching for entrypoint of " << name << endl;

            return NULL;
        }

        if (wglGetProcAddress == NULL)
        {
            wglGetProcAddress = reinterpret_cast<PROC (_stdcall *)(LPCSTR)>(GetProcAddress(opengl, "wglGetProcAddress"));
        }

        PROC proc = GetProcAddress(opengl, name);

        if (proc == NULL && wglGetProcAddress != NULL)
        {
            proc = wglGetProcAddress(name);
        }

        if (proc == NULL)
        {
            cerr << "Could not find entrypoint for " << name << endl;

            if (!optional)
            {
                throw runtime_error("OpenGL binding");
            }
        }

        return proc;
    }

    PROC bindarb(LPCSTR name, LPCSTR namearb)
    {
        PROC proc = NULL;

        try
        {
            proc = bind(name, false);
        }
        catch (runtime_error &e)
        {
            if (strncmp(e.what(), "OpenGL binding", 14) != 0)
            {
                throw e;
            }
        }

        if (proc == NULL)
        {
            proc = bind(namearb, false);
        }

        if (proc != NULL)
        {
            cerr << "Falling back to entrypoint for " << namearb << endl;
        }

        return proc;
    }

#define BIND(result, parameters, name) \
    typedef result (APIENTRYP BIND_GL_##name)(parameters); \
    GLAPI BIND_GL_##name name = reinterpret_cast<BIND_GL_##name>(bind(#name, false));

#define BINDOPT(result, parameters, name) \
    typedef result (APIENTRYP BIND_GL_##name)(parameters); \
    GLAPI BIND_GL_##name name = reinterpret_cast<BIND_GL_##name>(bind(#name, true));

#define BINDARB(result, parameters, name, namearb) \
    typedef result (APIENTRYP BIND_GL_##name)(parameters); \
    GLAPI BIND_GL_##name name = reinterpret_cast<BIND_GL_##name>(bindarb(#name, #namearb));

BIND(GLvoid, GLenum, glEnable);
BIND(GLvoid, GLenum, glDisable);
#define GL_TEXTURE_2D 0x0DE1
#define GL_DEPTH_TEST 0x0B71

BIND(GLubyte *, GLenum, glGetString);
#define GL_RENDERER                 0x1F01
#define GL_VENDOR                   0x1F00
#define GL_VERSION                  0x1F02
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C

    bool init()
    {
        if (opengl == NULL)
        {
            return false;
        }

        wglCreateContext = reinterpret_cast<HGLRC (_stdcall *)(HDC)>(GetProcAddress(opengl, "wglCreateContext"));
        wglMakeCurrent = reinterpret_cast<BOOL (_stdcall *)(HDC, HGLRC)>(GetProcAddress(opengl, "wglMakeCurrent"));
        wglDeleteContext = reinterpret_cast<BOOL (_stdcall *)(HGLRC)>(GetProcAddress(opengl, "wglDeleteContext"));
        wglCreateContextAttribs = reinterpret_cast<HGLRC (_stdcall *)(HDC, HGLRC, const int *)>(wglGetProcAddress("wglCreateContextAttribsARB"));
        wglChoosePixelFormat = reinterpret_cast<BOOL (_stdcall *)(HDC, const int *, const FLOAT *, UINT, int *, UINT *)>(wglGetProcAddress("wglChoosePixelFormatARB"));

        if (!wglCreateContext || !wglMakeCurrent || !wglDeleteContext)
        {
            FreeLibrary(opengl);

            return false;
        }

        return true;
    }

    void cleanup()
    {
        FreeLibrary(opengl);
    }
}

using namespace gl;

LRESULT CALLBACK application_loop(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message)
    {
    case WM_DESTROY:
        alive = false;
        break;

    case WM_PAINT:
        PAINTSTRUCT ps;

        BeginPaint(window, &ps);
        EndPaint(window, &ps);
        break;

    case WM_SIZE:
        break;

    default:
        return DefWindowProc(window, message, wparam, lparam);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int exit = EXIT_SUCCESS;

    /* Window */
    HINSTANCE instance;
    ATOM atom;
    HWND window;
    unsigned long int style = WS_POPUP;
    unsigned long int exstyle = WS_EX_APPWINDOW;

    /* Bindings */
    if (gl::init() == false)
    {
        cerr << "Could not bind OpenGL entry points" << endl;

        return EXIT_FAILURE;
    }

    /* Create window */
    instance = GetModuleHandle(NULL);

    if (instance == NULL)
    {
        cerr << "Could not retrieve module handle" << endl;

        return EXIT_FAILURE;
    }

    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_CLASSDC | CS_HREDRAW | CS_VREDRAW | CS_GLOBALCLASS;
    wc.lpfnWndProc = application_loop;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = DLGWINDOWEXTRA;
    wc.hInstance = instance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    wc.lpszMenuName = 0;
    wc.lpszClassName = "planets 3d window";

    atom = RegisterClassEx(&wc);

    RECT view;

    view.left = 0;
    view.top = 0;
    view.right = GetSystemMetrics(SM_CXSCREEN);
    view.bottom = GetSystemMetrics(SM_CYSCREEN);

    if (AdjustWindowRectEx(&view, style, FALSE, exstyle) == 0)
    {
        cerr << "Could not adjust window rectangle" << endl;
    }

    window = CreateWindowEx(exstyle, "planets 3d window", "OpenGL planets", style, view.left, view.top, view.right - view.left, view.bottom - view.top, NULL, NULL, instance, NULL);

    if (window == NULL)
    {
        cerr << "Could not create window" << endl;

        exit = EXIT_FAILURE;

        goto cleanup_window;
    }

    ShowWindow(window, SW_SHOW);
    UpdateWindow(window);

    /* Do stuff */
    MSG msg;

    while (alive)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

cleanup_window:
    /* Destroy window */
    ShowWindow(window, SW_HIDE);
    DestroyWindow(window);
    UnregisterClass(reinterpret_cast<LPCSTR>(atom), instance);

    gl::cleanup();

    cout << "Shutting down" << endl;

    return exit;
}