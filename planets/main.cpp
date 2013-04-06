#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <Windows.h>
#include <map>

using namespace std;

bool alive = true;

namespace gl
{
    HMODULE opengl = NULL;
    multimap<void *, const char *> *imports = NULL;

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
    typedef signed int GLsizei;
    typedef unsigned char GLubyte;
    typedef unsigned short GLushort;
    typedef unsigned int GLuint;
    typedef unsigned short GLhalf;
    typedef float GLfloat;
    typedef float GLclampf;
    typedef double GLdouble;
    typedef double GLclampd;
    typedef void GLvoid;
    typedef char GLchar;
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
#define GL_NONE  0

    void *register_import(void *pointer, const char *name)
    {
        if (imports == NULL)
        {
            imports = new multimap<void *, const char *>;
        }

        if (imports == NULL)
        {
            throw runtime_error("OpenGL binding");
        }

        imports->insert(multimap<void *, const char *>::value_type(pointer, name));

        return NULL;
    }

#define BIND(result, name, ...) \
    typedef result (APIENTRYP BIND_GL_##name)(__VA_ARGS__); \
    GLAPI BIND_GL_##name name = reinterpret_cast<BIND_GL_##name>(register_import(&name, #name));

BIND(GLvoid, glEnable, GLenum);
BIND(GLvoid, glDisable, GLenum);
#define GL_TEXTURE_2D 0x0DE1
#define GL_DEPTH_TEST 0x0B71

BIND(GLubyte *, glGetString, GLenum);
#define GL_RENDERER                 0x1F01
#define GL_VENDOR                   0x1F00
#define GL_VERSION                  0x1F02
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C

BIND(GLvoid, glClearColor, GLclampf, GLclampf, GLclampf, GLclampf);
BIND(GLvoid, glClear, GLbitfield);
#define GL_COLOR_BUFFER_BIT 0x4000
#define GL_DEPTH_BUFFER_BIT 0x0100

BIND(GLvoid, glGenBuffers, GLsizei, GLuint *);
BIND(GLvoid, glDeleteBuffers, GLsizei, const GLuint *);
BIND(GLvoid, glBindBuffer, GLenum, GLuint);
#define GL_ARRAY_BUFFER         0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
BIND(GLvoid, glBufferData, GLenum, GLsizeiptr, const GLvoid *, GLenum);
#define GL_STATIC_DRAW 0x88E4

BIND(GLvoid, glGenVertexArrays, GLsizei, GLuint *);
BIND(GLvoid, glDeleteVertexArrays, GLsizei, const GLuint *);
BIND(GLvoid, glBindVertexArray, GLuint);
BIND(GLvoid, glEnableVertexAttribArray, GLuint);
BIND(GLvoid, glDisableVertexAttribArray, GLuint);
BIND(GLvoid, glVertexAttribPointer, GLuint, GLsizei, GLenum, GLboolean, GLsizei, const GLvoid *);
#define GL_UNSIGNED_INT 0x1405
#define GL_FLOAT        0x1406

BIND(GLuint, glCreateShader, GLuint);
#define GL_VERTEX_SHADER   0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
BIND(GLvoid, glShaderSource, GLuint, GLsizei, const GLchar **, const GLint *);
BIND(GLvoid, glCompileShader, GLuint);
BIND(GLvoid, glDeleteShader, GLuint);

BIND(GLuint, glCreateProgram, void);
BIND(GLvoid, glAttachShader, GLuint, GLuint);
BIND(GLvoid, glDetachShader, GLuint, GLuint);
BIND(GLvoid, glLinkProgram, GLuint);
BIND(GLvoid, glUseProgram, GLuint);
BIND(GLvoid, glDeleteProgram, GLuint);

BIND(GLint, glGetAttribLocation, GLuint, const GLchar *);
BIND(GLint, glGetUniformLocation, GLuint, const GLchar *);
BIND(GLvoid, glBindFragDataLocation, GLuint, GLuint, const GLchar *);

BIND(GLvoid, glDrawElements, GLenum, GLsizei, GLenum, GLvoid *);
#define GL_TRIANGLES 0x0004

    bool init()
    {
        if (opengl == NULL)
        {
            opengl = LoadLibraryEx("OpenGL32.dll", NULL, 0);
        }

        if (opengl == NULL)
        {
            return false;
        }

        if (wglGetProcAddress == NULL)
        {
            wglGetProcAddress = reinterpret_cast<PROC (_stdcall *)(LPCSTR)>(GetProcAddress(opengl, "wglGetProcAddress"));
        }

        if (wglGetProcAddress == NULL)
        {
            FreeLibrary(opengl);

            return false;
        }

        wglCreateContext = reinterpret_cast<HGLRC (_stdcall *)(HDC)>(GetProcAddress(opengl, "wglCreateContext"));
        wglMakeCurrent = reinterpret_cast<BOOL (_stdcall *)(HDC, HGLRC)>(GetProcAddress(opengl, "wglMakeCurrent"));
        wglDeleteContext = reinterpret_cast<BOOL (_stdcall *)(HGLRC)>(GetProcAddress(opengl, "wglDeleteContext"));

        if (!wglCreateContext || !wglMakeCurrent || !wglDeleteContext)
        {
            FreeLibrary(opengl);

            return false;
        }

        return true;
    }

    bool import()
    {
        if (imports != NULL)
        {
            for (multimap<void *, const char *>::iterator i = imports->begin(); i != imports->end(); ++i)
            {
                PROC *entrypoint = reinterpret_cast<PROC *>(i->first);

                if (*entrypoint == NULL)
                {
                    *entrypoint = GetProcAddress(opengl, i->second);

                    if (*entrypoint == NULL)
                    {
                        *entrypoint = wglGetProcAddress(i->second);
                    }

                    if (*entrypoint == NULL)
                    {
                        cerr << "Could not find entrypoint for " << i->second << endl;

                    }
                }
            }
        }

        return true;
    }

    bool init_context()
    {
        wglCreateContextAttribs = reinterpret_cast<HGLRC (_stdcall *)(HDC, HGLRC, const int *)>(wglGetProcAddress("wglCreateContextAttribsARB"));
        wglChoosePixelFormat = reinterpret_cast<BOOL (_stdcall *)(HDC, const int *, const FLOAT *, UINT, int *, UINT *)>(wglGetProcAddress("wglChoosePixelFormatARB"));

        if (!wglCreateContextAttribs || !wglChoosePixelFormat)
        {
            return false;
        }
        else
        {
            return true;
        }
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

    /* Context */
    HDC device;
    HGLRC context;

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

    device = GetDC(window);

    if (device == 0)
    {
        cerr << "Could not get device context" << endl;

        exit = EXIT_FAILURE;

        goto cleanup_window;
    }

    /* Create context */
    PIXELFORMATDESCRIPTOR pixel;

    ZeroMemory(&pixel, sizeof(PIXELFORMATDESCRIPTOR));

    pixel.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pixel.nVersion = 1;
    pixel.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pixel.iPixelType = PFD_TYPE_RGBA;
    pixel.cColorBits = 24;
    pixel.cDepthBits = 8;
    pixel.iLayerType = PFD_MAIN_PLANE;

    int format = ChoosePixelFormat(device, &pixel);

    if (format <= 0)
    {
        cerr << "Could not choose pixel format" << endl;

        exit = EXIT_FAILURE;

        goto cleanup_window;
    }

    if (SetPixelFormat(device, format, &pixel) == FALSE)
    {
        cerr << "Could not set pixel format" << endl;

        exit = EXIT_FAILURE;

        goto cleanup_window;
    }

    context = gl::wglCreateContext(device);

    if (context == 0)
    {
        cerr << "Could not create OpenGL context" << endl;

        exit = EXIT_FAILURE;

        goto cleanup_window;
    }

    if (gl::wglMakeCurrent(device, context) == 0)
    {
        cerr << "Could not set current rendering context" << endl;

        exit = EXIT_FAILURE;

        goto cleanup_context;
    }

    if (gl::init_context() == false)
    {
        cerr << "Could not bind entrypoint for advanced context creation" << endl;

        goto skip_context;
    }

    const int attributes[] = {
        WGL_CONTEXT_MAJOR_VERSION, 2,
        WGL_CONTEXT_MINOR_VERSION, 1,
        0
    };

    gl::wglMakeCurrent(device, NULL);
    gl::wglDeleteContext(context);

    context = wglCreateContextAttribs(device, 0, attributes);

    if (context == 0)
    {
        cerr << "Could not create OpenGL context" << endl;

        exit = EXIT_FAILURE;

        goto cleanup_window;
    }

    if (gl::wglMakeCurrent(device, context) == 0)
    {
        cerr << "Could not set current rendering context" << endl;

        exit = EXIT_FAILURE;

        goto cleanup_context;
    }

skip_context:

    ShowWindow(window, SW_SHOW);
    UpdateWindow(window);

    gl::import();

    /* Do stuff */
    MSG msg;

    cout << "Rendering using " << glGetString(GL_RENDERER) << " by " << glGetString(GL_VENDOR) << endl;
    cout << "OpenGL version is " << glGetString(GL_VERSION) << ", GLSL version is " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    GLfloat vertices[] = {
        -0.5f, -0.5f,  0.0f,
         0.0f,  0.5f,  0.0f,
         0.5f, -0.5f,  0.0f
    };

    GLuint indices[] = {
        0, 1, 2
    };

    GLuint buffers[2];
    GLuint array;

    glGenVertexArrays(1, &array);
    glBindVertexArray(array);
    glGenBuffers(2, buffers);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(GLuint), indices, GL_STATIC_DRAW);

    const GLchar *vertex_shader_src =
        "in vec3 att_vertex;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(att_vertex, 1.0);\n"
        "}"
    ;

    const GLchar *fragment_shader_src =
        "out vec4 fragcolor;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    fragcolor = vec4(1.0, 1.0, 1.0, 1.0);\n"
        "}"
    ;

    GLint vertex_shader_len = strlen(vertex_shader_src);
    GLint fragment_shader_len = strlen(fragment_shader_src);

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint program = glCreateProgram();

    glShaderSource(vertex_shader, 1, &vertex_shader_src, &vertex_shader_len);
    glShaderSource(fragment_shader, 1, &fragment_shader_src, &fragment_shader_len);
    glCompileShader(vertex_shader);
    glCompileShader(fragment_shader);
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glBindFragDataLocation(program, 0, "fragcolor");
    glLinkProgram(program);
    glUseProgram(program);

    GLint att_vertex = glGetAttribLocation(program, "att_vertex");
    
    glVertexAttribPointer(att_vertex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
    glEnableVertexAttribArray(att_vertex);

    glClearColor(0.33f, 0.33f, 0.33f, 1.0f);

    while (alive)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

        SwapBuffers(device);
    }

    cout << "Shutting down" << endl;

    glDisableVertexAttribArray(att_vertex);
    glUseProgram(GL_NONE);
    glDetachShader(program, vertex_shader);
    glDetachShader(program, fragment_shader);
    glDeleteProgram(program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
    glDeleteBuffers(2, buffers);
    glBindVertexArray(GL_NONE);
    glDeleteVertexArrays(1, &array);

cleanup_context:
    gl::wglMakeCurrent(device, NULL);
    gl::wglDeleteContext(context);

cleanup_window:
    /* Destroy window */
    ShowWindow(window, SW_HIDE);
    DestroyWindow(window);
    UnregisterClass(reinterpret_cast<LPCSTR>(atom), instance);

    gl::cleanup();

    return exit;
}