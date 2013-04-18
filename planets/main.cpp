#define _USE_MATH_DEFINES
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <Windows.h>
#include <map>
#include <cmath>
#include <vector>

using namespace std;

/* Forward declarations */ /****************************************/

bool alive = true;

#define TEXTURE_DIFFUSE  0
#define TEXTURE_POSITION 1
#define TEXTURE_NORMAL   2

#define TEXTURE_MOON    2
#define TEXTURE_EARTH   3
#define TEXTURE_JUPITER 4
#define TEXTURE_PLUTO   5

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
BIND(GLvoid, glViewport, GLint, GLint, GLsizei, GLsizei);

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
#define GL_STREAM_DRAW 0x88E0
#define GL_STATIC_DRAW 0x88E4
BIND(GLvoid, glBufferSubData, GLenum, GLintptr, GLsizeiptr, const GLvoid *);

BIND(GLvoid, glGenVertexArrays, GLsizei, GLuint *);
BIND(GLvoid, glDeleteVertexArrays, GLsizei, const GLuint *);
BIND(GLvoid, glBindVertexArray, GLuint);
BIND(GLvoid, glEnableVertexAttribArray, GLuint);
BIND(GLvoid, glDisableVertexAttribArray, GLuint);

BIND(GLvoid, glVertexAttribPointer, GLuint, GLsizei, GLenum, GLboolean, GLsizei, const GLvoid *);
#define GL_UNSIGNED_BYTE        0x1401
#define GL_UNSIGNED_INT         0x1405
#define GL_FLOAT                0x1406
#define GL_UNSIGNED_SHORT_5_6_5 0x8363
BIND(GLvoid, glVertexAttribIPointer, GLuint, GLsizei, GLenum, GLsizei, const GLvoid *);
BIND(GLvoid, glVertexAttribDivisor, GLuint, GLuint);

BIND(GLuint, glCreateShader, GLuint);
#define GL_VERTEX_SHADER   0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_GEOMETRY_SHADER 0x8DD9
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

BIND(GLvoid, glUniformMatrix4fv, GLint, GLsizei, GLboolean, const GLfloat *);
BIND(GLvoid, glUniform1f, GLint, GLfloat);
BIND(GLvoid, glUniform1i, GLint, GLint);
BIND(GLvoid, glUniform3f, GLint, GLfloat, GLfloat, GLfloat);

BIND(GLvoid, glDrawElements, GLenum, GLsizei, GLenum, const GLvoid *);
#define GL_TRIANGLES 0x0004
BIND(GLvoid, glDrawElementsInstanced, GLenum, GLsizei, GLenum, const GLvoid *, GLsizei);

BIND(GLvoid, glGenFramebuffers, GLsizei, GLuint *);
BIND(GLvoid, glDeleteFramebuffers, GLsizei, GLuint *);
BIND(GLvoid, glBindFramebuffer, GLenum, GLuint);
#define GL_DRAW_FRAMEBUFFER 0x8CA9
#define GL_FRAMEBUFFER      0x8D40
BIND(GLvoid, glFramebufferRenderbuffer, GLenum, GLenum, GLenum, GLuint);
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_COLOR_ATTACHMENT1 0x8CE1
#define GL_COLOR_ATTACHMENT2 0x8CE2
#define GL_DEPTH_ATTACHMENT  0x8D00
BIND(GLvoid, glFramebufferTexture2D, GLenum, GLenum, GLenum, GLuint, GLint);
BIND(GLvoid, glDrawBuffers, GLsizei, const GLenum *);
BIND(GLvoid, glDrawBuffer, GLenum);
#define GL_BACK 0x0405

BIND(GLvoid, glGenRenderbuffers, GLsizei, GLuint *);
BIND(GLvoid, glDeleteRenderbuffers, GLsizei, GLuint *);
BIND(GLvoid, glBindRenderbuffer, GLenum, GLuint);
#define GL_RENDERBUFFER 0x8D41
BIND(GLvoid, glRenderbufferStorage, GLenum, GLenum, GLsizei, GLsizei);
#define GL_DEPTH_COMPONENT24 0x81A6

BIND(GLvoid, glGenTextures, GLsizei, GLuint *);
BIND(GLvoid, glDeleteTextures, GLsizei, GLuint *);
BIND(GLvoid, glBindTexture, GLenum, GLuint);
#define GL_TEXTURE_2D 0x0DE1
BIND(GLvoid, glTexImage2D, GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, GLvoid *data);
#define GL_RGB     0x1907
#define GL_RGBA    0x1908
#define GL_RGBA32F 0x8814
#define GL_RGBA16F 0x805B
#define GL_RGB32F  0x8815
#define GL_RGB16F  0x881B
#define GL_RGB565  0x8D62
BIND(GLvoid, glTexParameteri, GLenum, GLenum, GLint);
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S     0x2802
#define GL_TEXTURE_WRAP_T     0x2803
#define GL_NEAREST            0x2600
#define GL_LINEAR             0x2601
#define GL_REPEAT             0x2901
#define GL_CLAMP_TO_EDGE      0x812F
BIND(GLvoid, glActiveTexture, GLenum);
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE2 0x84C2

    bool init_context(bool has_context = true)
    {
        PROC GetProcAddressW = GetProcAddress(opengl, "wglGetProcAddress");
        PROC CreateContext = GetProcAddress(opengl, "wglCreateContext");
        PROC MakeCurrent = GetProcAddress(opengl, "wglMakeCurrent");
        PROC DeleteContext = GetProcAddress(opengl, "wglDeleteContext");

        if (!GetProcAddressW || !CreateContext || !MakeCurrent || !DeleteContext)
        {
            return false;
        }

        wglGetProcAddress = reinterpret_cast<PROC (_stdcall *)(LPCSTR)>(GetProcAddressW);
        wglCreateContext = reinterpret_cast<HGLRC (_stdcall *)(HDC)>(CreateContext);
        wglMakeCurrent = reinterpret_cast<BOOL (_stdcall *)(HDC, HGLRC)>(MakeCurrent);
        wglDeleteContext = reinterpret_cast<BOOL (_stdcall *)(HGLRC)>(DeleteContext);

        if (!has_context)
        {
            return true;
        }

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

        if(init_context(false) == false)
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

    void cleanup()
    {
        FreeLibrary(opengl);
    }
}

using namespace gl;

struct matrix
{
    GLfloat elements[16];

    matrix();

    matrix operator*(const matrix &other);
};

matrix translate(GLfloat x, GLfloat y, GLfloat z);
matrix rotate(GLfloat rx, GLfloat ry, GLfloat rz);
matrix scale(GLfloat s);
matrix perspective(GLfloat cnear, GLfloat cfar, GLfloat width, GLfloat height, GLfloat fov);
matrix orthogonal(GLfloat cnear, GLfloat cfar, GLfloat width, GLfloat height);

struct shader_stage1
{
    static const GLchar *source_vertex;
    static const GLchar *source_geometry;
    static const GLchar *source_fragment;

    GLuint shader_vertex;
    GLuint shader_geometry;
    GLuint shader_fragment;

    GLuint program;

    GLint att_vertex;
    GLint att_color;
    GLint att_normal;
    GLint att_matrix;
    GLint att_texture;

    GLint uni_perspective;
    GLint uni_counter;
    GLint uni_earth_moon;
    GLint uni_pluto_jupiter;

    shader_stage1();
    ~shader_stage1();
};

struct shader_stage2
{
    static const GLchar *source_vertex;
    static const GLchar *source_fragment;

    GLuint shader_vertex;
    GLuint shader_fragment;

    GLuint program;

    GLuint format;

    GLuint buffer_vertices;
    GLuint buffer_indices;

    GLint att_vertex;

    GLint uni_perspective;
    GLint uni_color;
    GLint uni_position;
    GLint uni_normal;
    GLint uni_light;

    shader_stage2();
    ~shader_stage2();
};

struct galaxy;
struct sun;
struct planet;

struct celestial
{
    matrix transform;
    galaxy *g;
    celestial *p;
    GLuint texture;
    GLfloat tx, ty, tz;
    GLfloat rx, ry, rz;
    GLfloat ox, oy, oz;
    GLfloat d, s;

    celestial(galaxy *g, GLuint texture = TEXTURE_JUPITER);

    celestial *add_child(GLfloat rx, GLfloat ry, GLfloat rz, GLfloat d, GLfloat s, GLfloat ox, GLfloat oy, GLfloat oz, GLuint texture = TEXTURE_MOON);

    void update(GLfloat time);
};

struct galaxy
{
    static unsigned short pixels_earth_moon[];
    static unsigned short pixels_pluto_jupiter[];

    GLuint texture_earth_moon;
    GLuint texture_pluto_jupiter;

    GLuint format;

    GLuint buffer_vertices;
    GLuint buffer_indices;
    GLuint buffer_matrices;

    shader_stage1 *shader;

    vector<celestial *> objects;

    galaxy(shader_stage1 *shader);
    ~galaxy();

    celestial *add_sun(GLfloat tx, GLfloat ty, GLfloat tz, GLfloat s, GLuint texture = TEXTURE_JUPITER);
    void render(GLfloat time);
};

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
        WGL_CONTEXT_MAJOR_VERSION, 3,
        WGL_CONTEXT_MINOR_VERSION, 3,
        0
    };

    HGLRC backup = context;

    context = wglCreateContextAttribs(device, 0, attributes);

    if (context == 0)
    {
        cerr << "Could not create OpenGL context" << endl;

        exit = EXIT_FAILURE;

        goto cleanup_window;
    }

    if (gl::wglMakeCurrent(device, context) == 0 || init_context() == false)
    {
        if (gl::wglMakeCurrent(device, backup) == 0)
        {
            cerr << "Could not set current rendering context" << endl;

            exit = EXIT_FAILURE;

            goto cleanup_context;
        }
        else
        {
            gl::wglDeleteContext(context);

            context = backup;
        }
    }
    else
    {
        gl::wglDeleteContext(backup);
    }

skip_context:

    ShowWindow(window, SW_SHOW);
    UpdateWindow(window);

    gl::import();

    /* Do stuff */
    MSG msg;

    cout << "Rendering using " << glGetString(GL_RENDERER) << " by " << glGetString(GL_VENDOR) << endl;
    cout << "OpenGL version is " << glGetString(GL_VERSION) << ", GLSL version is " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    shader_stage1 *stage1 = new shader_stage1;
    shader_stage2 *stage2 = new shader_stage2;

    galaxy *milky_way = new galaxy(stage1);

    celestial *sol = milky_way->add_sun(0.0f, 0.0f, -5.0f, 1.0f);
    celestial *terra = sol->add_child(0.0f, 1.0f, 0.0f, 4.0f, 0.2f, 0.0f, 0.0f, 0.0f, TEXTURE_EARTH);
    celestial *luna = terra->add_child(0.8f, 0.0f, 0.0f, 5.0f, 0.3f, 0.0f, 0.0f, 0.0f);
    celestial *luna2 = terra->add_child(0.0f, 3.0f, 0.0f, 7.0f, 0.4f, 0.2f, 0.0f, 0.0f);
    celestial *pluto = sol->add_child(0.0f, -0.3f, 0.0f, 9.0f, 0.45f, 0.0f, 0.0f, 0.0f, TEXTURE_PLUTO);
    celestial *pluto2 = sol->add_child(0.0f, 2.3f, 0.0f, 7.0f, 0.17f, 0.3f, 0.0f, 0.0f, TEXTURE_EARTH);

    GLuint frame;
    GLuint render;
    GLuint textures[3];

    glEnable(GL_TEXTURE_2D);
    glGenFramebuffers(1, &frame);
    glGenRenderbuffers(1, &render);
    glBindRenderbuffer(GL_RENDERBUFFER, render);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
    glGenTextures(3, textures);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_DIFFUSE]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_POSITION]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_NORMAL]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindFramebuffer(GL_FRAMEBUFFER, frame);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, render);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[TEXTURE_DIFFUSE], 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, textures[TEXTURE_POSITION], 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, textures[TEXTURE_NORMAL], 0);
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glViewport(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
    glEnable(GL_DEPTH_TEST);

    GLfloat counter = 0.0f;

    GLenum deferred_targets[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };

    while (alive)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        counter += 0.0033f;

        glBindFramebuffer(GL_FRAMEBUFFER, frame);
        glDrawBuffers(3, deferred_targets);

        milky_way->render(counter);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_DIFFUSE]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_POSITION]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_NORMAL]);

        glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
        glDrawBuffer(GL_BACK);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(stage2->format);
        glUseProgram(stage2->program);
        glUniform3f(stage2->uni_light, 2.5f, sin(counter * 0.025f) * 5.0f + 1.5f, 0.0f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

        SwapBuffers(device);
    }

    cout << "Shutting down" << endl;

    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
    glDeleteFramebuffers(1, &frame);
    glDeleteRenderbuffers(1, &render);
    glDeleteTextures(3, textures);
    glUseProgram(GL_NONE);
    glBindVertexArray(GL_NONE);

    delete milky_way;

    delete stage1;
    delete stage2;

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

/* Matrix methods */ /****************************************/

matrix::matrix()
{
    for (int x = 0; x < 4; ++x)
        for (int y = 0; y < 4; ++y)
            elements[x + y * 4] = (x == y) ? 1.0f : 0.0f;
}

matrix matrix::operator*(const matrix &other)
{
    matrix result;

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            result.elements[i + j * 4] = 0.0f;

            for (int k = 0; k < 4; ++k)
            {
                result.elements[i + j * 4] += elements[i + k * 4] * other.elements[k + j * 4];
            }
        }
    }

    return result;
}

matrix translate(GLfloat x, GLfloat y, GLfloat z)
{
    matrix result;

    result.elements[12] = x;
    result.elements[13] = y;
    result.elements[14] = z;

    return result;
}

matrix rotate(GLfloat rx, GLfloat ry, GLfloat rz)
{
    matrix rox, roy, roz;

    rox.elements[ 5] =  cos(rx);
    rox.elements[ 6] = -sin(rx);
    rox.elements[ 9] =  sin(rx);
    rox.elements[10] =  cos(rx);

    roy.elements[ 0] =  cos(ry);
    roy.elements[ 2] =  sin(ry);
    roy.elements[ 8] = -sin(ry);
    roy.elements[10] =  cos(ry);

    roz.elements[ 0] =  cos(rz);
    roz.elements[ 1] = -sin(rz);
    roz.elements[ 4] =  sin(rz);
    roz.elements[ 5] =  cos(rz);

    return rox * roy * roz;
}

matrix scale(GLfloat s)
{
    matrix result;

    result.elements[ 0] = s;
    result.elements[ 5] = s;
    result.elements[10] = s;

    return result;
}

matrix perspective(GLfloat cnear, GLfloat cfar, GLfloat width, GLfloat height, GLfloat fov)
{
    matrix result;

    result.elements[ 0] = (2.0f * cnear / (2.0f * cnear * tanf(fov * M_PI / 360.0f))) / (width / height);
    result.elements[ 5] = (2.0f * cnear / (2.0f * cnear * tanf(fov * M_PI / 360.0f)));
    result.elements[10] = -(cfar + cnear) / (cfar - cnear);
    result.elements[11] = -1.0f;
    result.elements[14] = -2.0f * (cfar * cnear) / (cfar - cnear);
    result.elements[15] = 0.0f;

    return result;
}

matrix orthogonal(GLfloat cnear, GLfloat cfar, GLfloat width, GLfloat height)
{
    matrix result;

    result.elements[ 0] = 2.0f / width;
    result.elements[ 5] = 2.0f / height;
    result.elements[10] = -2.0f / (cfar - cnear);
    result.elements[14] = -(cfar + cnear) / (cfar - cnear);

    return result;
}

/* Shader methods */ /****************************************/

shader_stage1::shader_stage1()
{
    shader_vertex = glCreateShader(GL_VERTEX_SHADER);
    shader_geometry = glCreateShader(GL_GEOMETRY_SHADER);
    shader_fragment = glCreateShader(GL_FRAGMENT_SHADER);
    program = glCreateProgram();

    GLint length_vertex = strlen(source_vertex);
    GLint length_geometry = strlen(source_geometry);
    GLint length_fragment = strlen(source_fragment);

    glShaderSource(shader_vertex, 1, &source_vertex, &length_vertex);
    glShaderSource(shader_geometry, 1, &source_geometry, &length_geometry);
    glShaderSource(shader_fragment, 1, &source_fragment, &length_fragment);

    glCompileShader(shader_vertex);
    glCompileShader(shader_geometry);
    glCompileShader(shader_fragment);

    glAttachShader(program, shader_vertex);
    glAttachShader(program, shader_geometry);
    glAttachShader(program, shader_fragment);

    glBindFragDataLocation(program, GL_COLOR_ATTACHMENT0 - GL_COLOR_ATTACHMENT0, "r_color");
    glBindFragDataLocation(program, GL_COLOR_ATTACHMENT1 - GL_COLOR_ATTACHMENT0, "r_position");
    glBindFragDataLocation(program, GL_COLOR_ATTACHMENT2 - GL_COLOR_ATTACHMENT0, "r_normal");

    glLinkProgram(program);

    att_vertex = glGetAttribLocation(program, "att_vertex");
    att_color = glGetAttribLocation(program, "att_color");
    att_normal = glGetAttribLocation(program, "att_normal");
    att_matrix = glGetAttribLocation(program, "att_matrix");
    att_texture = glGetAttribLocation(program, "att_texture");

    uni_perspective = glGetUniformLocation(program, "uni_perspective");
    uni_counter = glGetUniformLocation(program, "uni_counter");
    uni_earth_moon = glGetUniformLocation(program, "uni_earth_moon");
    uni_pluto_jupiter = glGetUniformLocation(program, "uni_pluto_jupiter");

    matrix matrix_perspective = perspective(0.1f, 1000.0f, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 75.0f) * translate(0.0f, 0.0f, -10.0f) * rotate(-0.4f, 0.0f, 0.0f);

    glUseProgram(program);

    glUniformMatrix4fv(uni_perspective, 1, GL_FALSE, matrix_perspective.elements);
    glUniform1f(uni_counter, 0.0f);
    glUniform1i(uni_earth_moon, GL_TEXTURE0 - GL_TEXTURE0);
    glUniform1i(uni_pluto_jupiter, GL_TEXTURE1 - GL_TEXTURE0);

    glUseProgram(GL_NONE);
}

shader_stage1::~shader_stage1()
{
    glUseProgram(GL_NONE);

    glDetachShader(program, shader_vertex);
    glDetachShader(program, shader_geometry);
    glDetachShader(program, shader_fragment);

    glDeleteShader(shader_vertex);
    glDeleteShader(shader_geometry);
    glDeleteShader(shader_fragment);

    glDeleteProgram(program);
}

shader_stage2::shader_stage2()
{
    shader_vertex = glCreateShader(GL_VERTEX_SHADER);
    shader_fragment = glCreateShader(GL_FRAGMENT_SHADER);
    program = glCreateProgram();

    GLint length_vertex = strlen(source_vertex);
    GLint length_fragment = strlen(source_fragment);

    glShaderSource(shader_vertex, 1, &source_vertex, &length_vertex);
    glShaderSource(shader_fragment, 1, &source_fragment, &length_fragment);

    glCompileShader(shader_vertex);
    glCompileShader(shader_fragment);

    glAttachShader(program, shader_vertex);
    glAttachShader(program, shader_fragment);

    glBindFragDataLocation(program, GL_COLOR_ATTACHMENT0 - GL_COLOR_ATTACHMENT0, "r_color");

    glLinkProgram(program);

    att_vertex = glGetAttribLocation(program, "att_vertex");

    uni_perspective = glGetUniformLocation(program, "uni_perspective");
    uni_color = glGetUniformLocation(program, "uni_color");
    uni_position = glGetUniformLocation(program, "uni_position");
    uni_normal = glGetUniformLocation(program, "uni_normal");
    uni_light = glGetUniformLocation(program, "uni_light");

    matrix matrix_orthogonal = orthogonal(0.1f, 1000.0f, 1.0f, 1.0f) * translate(-0.5f, -0.5f, 0.0f);

    glUseProgram(program);

    glUniformMatrix4fv(uni_perspective, 1, GL_FALSE, matrix_orthogonal.elements);
    glUniform1i(uni_color, GL_TEXTURE0 - GL_TEXTURE0);
    glUniform1i(uni_position, GL_TEXTURE1 - GL_TEXTURE0);
    glUniform1i(uni_normal, GL_TEXTURE2 - GL_TEXTURE0);

    glUseProgram(GL_NONE);

    GLfloat vertices[] = {
        0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 0.0f, -1.0f,
    };

    GLuint indices[] = {
        1, 0, 2,
        3, 2, 0,
    };

    glGenVertexArrays(1, &format);

    glBindVertexArray(format);

    glGenBuffers(1, &buffer_vertices);
    glGenBuffers(1, &buffer_indices);

    glBindBuffer(GL_ARRAY_BUFFER, buffer_vertices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_indices);

    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(att_vertex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);

    glEnableVertexAttribArray(att_vertex);

    glBindVertexArray(GL_NONE);
}

shader_stage2::~shader_stage2()
{
    glBindVertexArray(format);

    glDisableVertexAttribArray(att_vertex);

    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);

    glBindVertexArray(GL_NONE);

    glDeleteBuffers(1, &buffer_vertices);
    glDeleteBuffers(1, &buffer_indices);

    glDeleteVertexArrays(1, &format);

    glUseProgram(GL_NONE);

    glDetachShader(program, shader_vertex);
    glDetachShader(program, shader_fragment);

    glDeleteShader(shader_vertex);
    glDeleteShader(shader_fragment);

    glDeleteProgram(program);
}

/* Celestial methods */ /****************************************/

celestial::celestial(galaxy *g, GLuint texture)
{
    this->g = g;
    this->texture = texture;
    this->rx = 0.0f;
    this->ry = 0.0f;
    this->rz = 0.0f;
    this->ox = 0.0f;
    this->oy = 0.0f;
    this->oz = 0.0f;
    this->d = 0.0f;
    this->s = 1.0f;
    this->p = NULL;

    g->objects.push_back(this);

    glBindVertexArray(g->format);

    glBufferData(GL_ARRAY_BUFFER, g->objects.size() * (16 * sizeof(GLfloat) + sizeof(GLuint)), NULL, GL_STREAM_DRAW);

    glBindVertexArray(GL_NONE);
}

celestial *celestial::add_child(GLfloat rx, GLfloat ry, GLfloat rz, GLfloat d, GLfloat s, GLfloat ox, GLfloat oy, GLfloat oz, GLuint texture)
{
    celestial *object = new celestial(g, texture);

    object->rx = rx;
    object->ry = ry;
    object->rz = rz;
    object->ox = ox;
    object->oy = oy;
    object->oz = oz;
    object->d = d;
    object->s = s;
    object->p = this;

    return object;
}

void celestial::update(GLfloat time)
{
    if (p == NULL)
        transform = translate(tx, ty, tz);
    else
        transform = p->transform;

    transform = transform * rotate(ox, oy, oz) * rotate(time * rx, time * ry, time *rz) * translate(0.0f, 0.0f, d) * scale(s);
}

galaxy::galaxy(shader_stage1 *shader)
{
    this->shader = shader;

    GLfloat radius = (1.0f + sqrtf(5.0f)) / 2.0f;

    GLfloat vertices[] = {
          -1.0f,  radius,    0.0f,
           1.0f,    0.0f,    0.0f,
           1.0f,  radius,    0.0f,
           0.5f,    0.5f,    0.0f,
          -1.0f, -radius,    0.0f,
           0.0f,    1.0f,    0.0f,
           1.0f, -radius,    0.0f,
           0.0f,    0.5f,    0.5f,
           0.0f,   -1.0f,  radius,
           0.0f,    0.0f,    1.0f,
           0.0f,    1.0f,  radius,
           0.5f,    0.0f,    0.5f,
           0.0f,   -1.0f, -radius,
           1.0f,    0.0f,    1.0f,
           0.0f,    1.0f, -radius,
           1.0f,    1.0f,    0.0f,
         radius,    0.0f,   -1.0f,
           0.0f,    1.0f,    1.0f,
         radius,    0.0f,    1.0f,
           0.0f,    0.0f,    0.0f,
        -radius,    0.0f,   -1.0f,
           0.5f,    0.5f,    0.5f,
        -radius,    0.0f,    1.0f,
           1.0f,    1.0f,    1.0f,
    };

    GLuint indices[] = {
         0, 11,  5,
         0,  5,  1,
         0,  1,  7,
         0,  7, 10,
         0, 10, 11,
         1,  5,  9,
         5, 11,  4,
        11, 10,  2,
        10,  7,  6,
         7,  1,  8,
         3,  9,  4,
         3,  4,  2,
         3,  2,  6,
         3,  6,  8,
         3,  8,  9,
         4,  9,  5,
         2,  4, 11,
         6,  2, 10,
         8,  6,  7,
         9,  8,  1,
    };

    glGenVertexArrays(1, &format);

    glBindVertexArray(format);

    glGenBuffers(1, &buffer_vertices);
    glGenBuffers(1, &buffer_indices);
    glGenBuffers(1, &buffer_matrices);

    glBindBuffer(GL_ARRAY_BUFFER, buffer_vertices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_indices);

    glBufferData(GL_ARRAY_BUFFER, 72 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 60 * sizeof(GLuint), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(shader->att_vertex, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), NULL);
    glVertexAttribPointer(shader->att_normal, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), NULL);
    glVertexAttribPointer(shader->att_color, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLfloat *>(NULL) + 3);

    glEnableVertexAttribArray(shader->att_vertex);
    glEnableVertexAttribArray(shader->att_normal);
    glEnableVertexAttribArray(shader->att_color);

    glBindBuffer(GL_ARRAY_BUFFER, buffer_matrices);

    glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STREAM_DRAW);

    glVertexAttribPointer(shader->att_matrix, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(GLfloat) + sizeof(GLuint), NULL);
    glVertexAttribPointer(shader->att_matrix + 1, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(GLfloat) + sizeof(GLuint), reinterpret_cast<GLfloat *>(NULL) + 4);
    glVertexAttribPointer(shader->att_matrix + 2, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(GLfloat) + sizeof(GLuint), reinterpret_cast<GLfloat *>(NULL) + 8);
    glVertexAttribPointer(shader->att_matrix + 3, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(GLfloat) + sizeof(GLuint), reinterpret_cast<GLfloat *>(NULL) + 12);
    glVertexAttribIPointer(shader->att_texture, 1, GL_UNSIGNED_INT, 16 * sizeof(GLfloat) + sizeof(GLuint), reinterpret_cast<GLfloat *>(NULL) + 16);

    glVertexAttribDivisor(shader->att_matrix, 1);
    glVertexAttribDivisor(shader->att_matrix + 1, 1);
    glVertexAttribDivisor(shader->att_matrix + 2, 1);
    glVertexAttribDivisor(shader->att_matrix + 3, 1);
    glVertexAttribDivisor(shader->att_texture, 1);

    glEnableVertexAttribArray(shader->att_matrix);
    glEnableVertexAttribArray(shader->att_matrix + 1);
    glEnableVertexAttribArray(shader->att_matrix + 2);
    glEnableVertexAttribArray(shader->att_matrix + 3);
    glEnableVertexAttribArray(shader->att_texture);

    glBindVertexArray(GL_NONE);

    glGenTextures(1, &texture_earth_moon);
    glGenTextures(1, &texture_pluto_jupiter);

    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, texture_earth_moon);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 32, 32, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, pixels_earth_moon);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glActiveTexture(GL_TEXTURE1);

    glBindTexture(GL_TEXTURE_2D, texture_pluto_jupiter);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 32, 32, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, pixels_pluto_jupiter);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

galaxy::~galaxy()
{
    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    glDeleteTextures(1, &texture_earth_moon);
    glDeleteTextures(1, &texture_pluto_jupiter);

    glBindVertexArray(format);

    glDisableVertexAttribArray(shader->att_matrix);
    glDisableVertexAttribArray(shader->att_matrix + 1);
    glDisableVertexAttribArray(shader->att_matrix + 2);
    glDisableVertexAttribArray(shader->att_matrix + 3);
    glDisableVertexAttribArray(shader->att_texture);

    glDisableVertexAttribArray(shader->att_vertex);
    glDisableVertexAttribArray(shader->att_normal);
    glDisableVertexAttribArray(shader->att_color);

    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);

    glBindVertexArray(GL_NONE);

    glDeleteBuffers(1, &buffer_vertices);
    glDeleteBuffers(1, &buffer_indices);
    glDeleteBuffers(1, &buffer_matrices);

    glDeleteVertexArrays(1, &format);

    for (int i = 0; i < objects.size(); ++i)
    {
        delete objects[i];
    }
}

celestial *galaxy::add_sun(GLfloat tx, GLfloat ty, GLfloat tz, GLfloat s, GLuint texture)
{
    celestial *object = new celestial(this, texture);

    object->tx = tx;
    object->ty = ty;
    object->tz = tz;
    object->s = s;

    return object;
}

void galaxy::render(GLfloat time)
{
    glBindVertexArray(format);

    for (int i = 0; i < objects.size(); ++i)
    {
        objects[i]->update(time);

        glBufferSubData(GL_ARRAY_BUFFER, i * (16 * sizeof(GLfloat) + sizeof(GLuint)), 16 * sizeof(GLfloat), objects[i]->transform.elements);

        glBufferSubData(GL_ARRAY_BUFFER, i * (16 * sizeof(GLfloat) + sizeof(GLuint)) + 16 * sizeof(GLfloat), sizeof(GLuint), &objects[i]->texture);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shader->program);

    glUniform1f(shader->uni_counter, time);

    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, texture_earth_moon);

    glActiveTexture(GL_TEXTURE1);

    glBindTexture(GL_TEXTURE_2D, texture_pluto_jupiter);

    glDrawElementsInstanced(GL_TRIANGLES, 60, GL_UNSIGNED_INT, NULL, objects.size());
}

/* Shader source codes */ /****************************************/

/* Stage 1, vertex shader: Adds rotation */
const GLchar *shader_stage1::source_vertex = "\
                                             #version 330 core\n\
                                             \n\
                                             in vec3 att_vertex;\n\
                                             in vec3 att_color;\n\
                                             in mat4 att_matrix;\n\
                                             in vec3 att_normal;\n\
                                             in uint att_texture;\n\
                                             \n\
                                             out vec3 vg_color;\n\
                                             out vec3 vg_normal;\n\
                                             out vec3 vg_position;\n\
                                             out mat4 vg_matrix;\n\
                                             out mat4 vg_perspective;\n\
                                             out uint vg_texture;\n\
                                             \n\
                                             uniform mat4 uni_perspective;\n\
                                             uniform float uni_counter;\n\
                                             \n\
                                             void main()\n\
                                             {\n\
                                                 float spin = uni_counter * (-0.5 + 0.75 * gl_InstanceID);\n\
                                                 mat4 rotate = mat4(cos(spin), 0.0, -sin(spin), 0.0,\n\
                                                                          0.0, 1.0,        0.0, 0.0,\n\
                                                                    sin(spin), 0.0,  cos(spin), 0.0,\n\
                                                                          0.0, 0.0,        0.0, 1.0);\n\
                                                 vec4 position = uni_perspective * att_matrix * rotate * vec4(att_vertex, 1.0);\n\
                                                 \n\
                                                 gl_Position = position;\n\
                                                 vg_color = att_color;\n\
                                                 vg_normal = normalize(att_normal);\n\
                                                 vg_position = att_vertex;\n\
                                                 vg_matrix = att_matrix * rotate;\n\
                                                 vg_perspective = uni_perspective;\n\
                                                 vg_texture = att_texture;\n\
                                             }\
                                             ";

/* Stage 1, geometry shader: Subdivides the icosahedron to approximate an icosphere */
const GLchar *shader_stage1::source_geometry = "\
                                                #version 330 core\n\
                                                \n\
                                                layout(triangles) in;\n\
                                                layout(triangle_strip, max_vertices = 8) out;\n\
                                                \n\
                                                in vec3 vg_normal[];\n\
                                                in vec3 vg_position[];\n\
                                                in vec3 vg_color[];\n\
                                                in mat4 vg_matrix[];\n\
                                                in mat4 vg_perspective[];\n\
                                                in uint vg_texture[];\n\
                                                \n\
                                                out vec3 gf_normal;\n\
                                                out vec3 gf_position;\n\
                                                out vec3 gf_color;\n\
                                                out vec3 gf_texcoord;\n\
                                                flat out uint gf_texture;\n\
                                                \n\
                                                void main()\n\
                                                {\n\
                                                    vec3 position[3];\n\
                                                    vec3 normal[3];\n\
                                                    \n\
                                                    for (int i = 0; i < 3; i++)\n\
                                                    {\n\
                                                        position[i] = vec3((vg_position[i].x + vg_position[(i + 1) % 3].x) / 2.0,\n\
                                                                           (vg_position[i].y + vg_position[(i + 1) % 3].y) / 2.0,\n\
                                                                           (vg_position[i].z + vg_position[(i + 1) % 3].z) / 2.0);\n\
                                                        position[i] = normalize(position[i]) * (sqrt(10.0) + 2.0 * sqrt(5.0)) / 4.0;\n\
                                                        \n\
                                                        normal[i] = vec3((vg_normal[i].x + vg_normal[(i + 1) % 3].x) / 2.0,\n\
                                                                         (vg_normal[i].y + vg_normal[(i + 1) % 3].y) / 2.0,\n\
                                                                         (vg_normal[i].z + vg_normal[(i + 1) % 3].z) / 2.0);\n\
                                                    }\n\
                                                    \n\
                                                    gl_Position = vg_perspective[0] * vg_matrix[0] * vec4(vg_position[0], 1.0);\n\
                                                    gf_normal = (vg_matrix[0] * vec4(vg_normal[0], 0.0)).xyz;\n\
                                                    gf_position = (vg_matrix[0] * vec4(vg_position[0], 1.0)).xyz;\n\
                                                    gf_color = vec3(1.0, 0.0, 0.0);\n\
                                                    gf_texcoord = vg_normal[0];\n\
                                                    gf_texture = vg_texture[0];\n\
                                                    EmitVertex();\n\
                                                    \n\
                                                    gl_Position = vg_perspective[0] * vg_matrix[0] * vec4(position[0], 1.0);\n\
                                                    gf_normal = (vg_matrix[0] * vec4(normal[0], 0.0)).xyz;\n\
                                                    gf_position = (vg_matrix[0] * vec4(position[0], 1.0)).xyz;\n\
                                                    gf_color = vec3(1.0, 1.0, 0.0);\n\
                                                    gf_texcoord = normal[0];\n\
                                                    gf_texture = vg_texture[0];\n\
                                                    EmitVertex();\n\
                                                    \n\
                                                    gl_Position = vg_perspective[0] * vg_matrix[0] * vec4(position[2], 1.0);\n\
                                                    gf_normal = (vg_matrix[0] * vec4(normal[2], 0.0)).xyz;\n\
                                                    gf_position = (vg_matrix[0] * vec4(position[2], 1.0)).xyz;\n\
                                                    gf_color = vec3(1.0, 0.0, 1.0);\n\
                                                    gf_texcoord = normal[2];\n\
                                                    gf_texture = vg_texture[0];\n\
                                                    EmitVertex();\n\
                                                    \n\
                                                    gl_Position = vg_perspective[0] * vg_matrix[0] * vec4(position[1], 1.0);\n\
                                                    gf_normal = (vg_matrix[0] * vec4(normal[1], 0.0)).xyz;\n\
                                                    gf_position = (vg_matrix[0] * vec4(position[1], 1.0)).xyz;\n\
                                                    gf_color = vec3(0.0, 1.0, 1.0);\n\
                                                    gf_texcoord = normal[1];\n\
                                                    gf_texture = vg_texture[0];\n\
                                                    EmitVertex();\n\
                                                    \n\
                                                    gl_Position = vg_perspective[0] * vg_matrix[0] * vec4(vg_position[2], 1.0);\n\
                                                    gf_normal = (vg_matrix[0] * vec4(vg_normal[2], 0.0)).xyz;\n\
                                                    gf_position = (vg_matrix[0] * vec4(vg_position[2], 1.0)).xyz;\n\
                                                    gf_color = vec3(0.0, 0.0, 1.0);\n\
                                                    gf_texcoord = vg_normal[2];\n\
                                                    gf_texture = vg_texture[0];\n\
                                                    EmitVertex();\n\
                                                    \n\
                                                    EndPrimitive();\n\
                                                    \n\
                                                    gl_Position = vg_perspective[0] * vg_matrix[0] * vec4(position[1], 1.0);\n\
                                                    gf_normal = (vg_matrix[0] * vec4(normal[1], 0.0)).xyz;\n\
                                                    gf_position = (vg_matrix[0] * vec4(position[1], 1.0)).xyz;\n\
                                                    gf_color = vec3(0.0, 1.0, 1.0);\n\
                                                    gf_texcoord = normal[1];\n\
                                                    gf_texture = vg_texture[0];\n\
                                                    EmitVertex();\n\
                                                    \n\
                                                    gl_Position = vg_perspective[0] * vg_matrix[0] * vec4(position[0], 1.0);\n\
                                                    gf_normal = (vg_matrix[0] * vec4(normal[0], 0.0)).xyz;\n\
                                                    gf_position = (vg_matrix[0] * vec4(position[0], 1.0)).xyz;\n\
                                                    gf_color = vec3(1.0, 1.0, 0.0);\n\
                                                    gf_texcoord = normal[0];\n\
                                                    gf_texture = vg_texture[0];\n\
                                                    EmitVertex();\n\
                                                    \n\
                                                    gl_Position = vg_perspective[0] * vg_matrix[0] * vec4(vg_position[1], 1.0);\n\
                                                    gf_normal = (vg_matrix[0] * vec4(vg_normal[1], 0.0)).xyz;\n\
                                                    gf_position = (vg_matrix[0] * vec4(vg_position[1], 1.0)).xyz;\n\
                                                    gf_color = vec3(0.0, 1.0, 0.0);\n\
                                                    gf_texcoord = vg_normal[1];\n\
                                                    gf_texture = vg_texture[0];\n\
                                                    EmitVertex();\n\
                                                    \n\
                                                    EndPrimitive();\n\
                                                }\n\
                                            ";

/* Stage 1, fragment shader: Store results in framebuffers */
const GLchar *shader_stage1::source_fragment = "\
                                               #version 330 core\n\
                                               \n\
                                               in vec3 gf_color;\n\
                                               in vec3 gf_normal;\n\
                                               in vec3 gf_position;\n\
                                               in vec3 gf_texcoord;\n\
                                               flat in uint gf_texture;\n\
                                               \n\
                                               out vec3 r_color;\n\
                                               out vec3 r_normal;\n\
                                               out vec3 r_position;\n\
                                               \n\
                                               uniform sampler2D uni_earth_moon;\n\
                                               uniform sampler2D uni_pluto_jupiter;\n\
                                               \n\
                                               void main()\n\
                                               {\n\
                                                   vec2 texcoord = normalize(gf_texcoord).xy * 0.5 + vec2(0.5, 0.5);\n\
                                                   texcoord.y = texcoord.y * 0.5;\n\
                                                   if (gf_texture % 2u == 1u)\n\
                                                       texcoord.y = texcoord.y + 0.5;\n\
                                                   if ((gf_texture / 2u) == 1u)\n\
                                                       r_color = texture(uni_earth_moon, texcoord).rgb;\n\
                                                   else if ((gf_texture / 2u) == 2u)\n\
                                                       r_color = texture(uni_pluto_jupiter, texcoord).rgb;\n\
                                                   else\n\
                                                       r_color = vec3(1.0, 0.0, 1.0);\n\
                                                   r_normal = gf_normal;\n\
                                                   r_position = gf_position;\n\
                                               }\
                                               ";

/* Stage 2, vertex_shader: Aligns rectangle to screen */
const GLchar *shader_stage2::source_vertex = "\
                                             #version 330 core\n\
                                             \n\
                                             in vec3 att_vertex;\n\
                                             \n\
                                             out vec2 vf_texcoord;\n\
                                             \n\
                                             uniform mat4 uni_perspective;\n\
                                             \n\
                                             void main()\n\
                                             {\n\
                                                 gl_Position = uni_perspective * vec4(att_vertex, 1.0);\n\
                                                 vf_texcoord = att_vertex.xy;\n\
                                             }\n\
                                             ";

/* Stage 2, fragment shader: Displays framebuffers and composited result */
const GLchar *shader_stage2::source_fragment = "\
                                               #version 330 core\n\
                                               \n\
                                               in vec2 vf_texcoord;\n\
                                               \n\
                                               out vec4 r_color;\n\
                                               \n\
                                               uniform sampler2D uni_color;\n\
                                               uniform sampler2D uni_position;\n\
                                               uniform sampler2D uni_normal;\n\
                                               uniform vec3 uni_light;\n\
                                               \n\
                                               void main()\n\
                                               {\n\
                                                   vec3 position = texture(uni_position, vf_texcoord).rgb;\n\
                                                   vec3 normal = texture(uni_normal, vf_texcoord).rgb;\n\
                                                   vec4 diffuse = vec4(texture(uni_color, vf_texcoord).rgb, 1.0);\n\
                                                   \n\
                                                   vec3 light = normalize(uni_light - position);\n\
                                                   \n\
                                                   r_color = clamp(max(dot(light, normal), 0.0), 0.1, 1.0) * diffuse;\n\
                                               }\n\
                                               ";

/* Textures */ /****************************************/

unsigned short galaxy::pixels_earth_moon[] = {
    0xb5b6, 0xa534, 0xb596, 0xbdf7, 0xbdd7, 0xd69a, 0xce79, 0xd69a,
    0xd6ba, 0xc618, 0xc638, 0xce59, 0xce59, 0xc638, 0xc618, 0xbdf7,
    0xbdf7, 0xb5b6, 0xb5b6, 0xbdf7, 0xb5b6, 0xa514, 0xad75, 0xad55,
    0x9cf3, 0xad75, 0xb5b6, 0xb596, 0xa534, 0xad75, 0xad75, 0xad75,
    0xa514, 0x9cd3, 0x94b2, 0x9cf3, 0x94b2, 0xad55, 0xad75, 0x9cf3,
    0x9cd3, 0x9cd3, 0x9cf3, 0xad55, 0xad75, 0xad75, 0xa534, 0xa514,
    0x9cf3, 0xad55, 0xa534, 0xad55, 0xa514, 0xa514, 0x9cf3, 0xa514,
    0xa534, 0xa534, 0xad55, 0xad75, 0xa534, 0xa514, 0xa534, 0xa534,
    0x9cf3, 0x94b2, 0x9cf3, 0x94b2, 0x94b2, 0x94b2, 0xad55, 0xad55,
    0x9cf3, 0xa514, 0xa534, 0xad55, 0xad75, 0xad55, 0xb596, 0xb596,
    0xad75, 0xb5b6, 0xbdd7, 0xb5b6, 0xad55, 0xad55, 0xa534, 0xa514,
    0xa534, 0xad55, 0xb5b6, 0xb596, 0xb5b6, 0xb596, 0xa514, 0xa514,
    0xad55, 0xa514, 0x9cf3, 0x9cd3, 0x94b2, 0x9cd3, 0xa534, 0xa534,
    0xa534, 0xa534, 0xad75, 0xa534, 0xad55, 0xb5b6, 0xb5b6, 0xbdf7,
    0xbdd7, 0xbdd7, 0xb5b6, 0xb5b6, 0xb5b6, 0xb596, 0xb596, 0xad75,
    0xad55, 0xad75, 0xb596, 0xb596, 0xad55, 0xa534, 0x9cf3, 0x9cf3,
    0xad55, 0xad75, 0xa534, 0xa534, 0xa534, 0xa514, 0xad55, 0xad55,
    0xa534, 0xad55, 0xb596, 0xad55, 0xad55, 0xad55, 0xb5b6, 0xbdf7,
    0xb5b6, 0xbdd7, 0xc638, 0xbdf7, 0xb5b6, 0xb5b6, 0xbdd7, 0xa534,
    0xa534, 0xb596, 0xad75, 0xad75, 0xb596, 0xad75, 0x9cd3, 0xa534,
    0xb596, 0xb5b6, 0xad75, 0xad75, 0xad55, 0xa534, 0xa534, 0x9cf3,
    0xad55, 0xad75, 0xb5b6, 0xa534, 0x9492, 0x9cd3, 0xa534, 0xb596,
    0xb596, 0xb5b6, 0xc638, 0xbdd7, 0xb596, 0xad75, 0xb5b6, 0xad75,
    0xad75, 0xad75, 0xad75, 0xa514, 0xad55, 0xb596, 0xa514, 0xad55,
    0xb5b6, 0xb596, 0xad75, 0xad75, 0xad55, 0xa514, 0xad55, 0x9492,
    0x9cf3, 0xb596, 0xbdd7, 0xad75, 0x9cd3, 0x8c71, 0x94b2, 0xad55,
    0xad75, 0xb596, 0xbdf7, 0xad75, 0xad55, 0xad75, 0xbdf7, 0xb5b6,
    0xb5b6, 0xad55, 0xa534, 0xa514, 0xb596, 0xad55, 0xad55, 0xad75,
    0xb596, 0xb5b6, 0xad75, 0xad55, 0xa534, 0xa514, 0xa534, 0xa534,
    0xa534, 0xb596, 0xad55, 0x9492, 0x8430, 0x8c51, 0x9cd3, 0xad75,
    0xb596, 0xb5b6, 0xad75, 0xb596, 0x9cf3, 0xa534, 0xb5b6, 0xbdf7,
    0xb596, 0xad75, 0xa534, 0xad55, 0xad55, 0xad55, 0xa534, 0xad75,
    0xb5b6, 0xb5b6, 0xad75, 0xad55, 0xad55, 0xad55, 0xad55, 0xad55,
    0xad75, 0xb596, 0x9cd3, 0x94b2, 0x9cd3, 0x9cf3, 0xa514, 0xad55,
    0xb596, 0xb596, 0xa534, 0x9cd3, 0xa534, 0xa514, 0xad55, 0xb596,
    0xad55, 0xad55, 0xad55, 0xad55, 0xad55, 0xad55, 0xad75, 0xb5b6,
    0xbdd7, 0xbdd7, 0xb596, 0xad75, 0xad55, 0xa534, 0xad55, 0xad55,
    0xad55, 0xa514, 0x9492, 0x9cd3, 0x94b2, 0x94b2, 0x94b2, 0x94b2,
    0xa514, 0xa514, 0xa514, 0xa534, 0xa534, 0x9492, 0xad75, 0xb5b6,
    0xad55, 0xad75, 0xad55, 0xad55, 0xa534, 0xad75, 0xad75, 0xb5b6,
    0xb5b6, 0xb596, 0xad75, 0xad75, 0xad55, 0xad55, 0xa534, 0xad55,
    0xa534, 0x94b2, 0x8c71, 0x9492, 0x8430, 0x9492, 0x9492, 0x94b2,
    0xa534, 0x9cf3, 0x9cf3, 0xb596, 0xad75, 0xa514, 0xb596, 0xb5b6,
    0xa534, 0xa534, 0xad55, 0xad55, 0xad55, 0xa514, 0xad75, 0xb5b6,
    0xb5b6, 0xb5b6, 0xb5b6, 0xad75, 0xad75, 0xad75, 0xad55, 0xad55,
    0xad75, 0x94b2, 0x8c51, 0x9cd3, 0x9cd3, 0x94b2, 0x9cf3, 0xa534,
    0xb596, 0xb596, 0xb596, 0xb5b6, 0xb5b6, 0xad55, 0xb5b6, 0xad75,
    0xad75, 0xad75, 0xad55, 0xa534, 0xad55, 0xad75, 0xad55, 0xb596,
    0xad55, 0xa534, 0xad55, 0xad55, 0xa534, 0xad55, 0xad55, 0xa534,
    0xb596, 0xad55, 0xad55, 0xa534, 0xa534, 0x9cf3, 0x9492, 0x94b2,
    0xa514, 0x9cd3, 0x9cd3, 0xad55, 0xad55, 0x9cd3, 0xa534, 0xa534,
    0xad75, 0xa534, 0xad55, 0xad75, 0xad75, 0xad75, 0xad75, 0xb596,
    0xa534, 0x9cd3, 0xa514, 0x9cd3, 0x9cd3, 0x9cf3, 0x9cf3, 0x9cd3,
    0xa534, 0xb596, 0xb596, 0xb5b6, 0xad75, 0x9cf3, 0x9cf3, 0x9cf3,
    0xa514, 0xa514, 0x9cd3, 0xa534, 0xad75, 0x9cf3, 0xad55, 0xad75,
    0xad55, 0xa534, 0xad75, 0xa534, 0xa534, 0xa534, 0xb596, 0xb596,
    0x9cf3, 0x9cd3, 0x9cd3, 0x9cd3, 0x9cf3, 0xa514, 0xa534, 0xa514,
    0xa514, 0xa534, 0xad75, 0xbdd7, 0xbdf7, 0xbdd7, 0xc618, 0xc638,
    0xb5b6, 0xad55, 0xad55, 0xa534, 0xad75, 0xad55, 0x9cf3, 0xad75,
    0xad55, 0xa534, 0xad55, 0xa514, 0xad55, 0xad55, 0xb596, 0xad75,
    0xad55, 0xad55, 0xad55, 0xa514, 0x9492, 0xa534, 0xbdd7, 0xad75,
    0xad55, 0x9cd3, 0x9cf3, 0xa534, 0xad55, 0xb5b6, 0xb5b6, 0xbdf7,
    0xb5b6, 0xb5b6, 0xb596, 0xad75, 0xa514, 0xa534, 0xad55, 0xbdd7,
    0xbdf7, 0xc618, 0xbdf7, 0xb5b6, 0xbdd7, 0xb5b6, 0xb5b6, 0xbdd7,
    0xd6dc, 0xd6dc, 0xdf1d, 0xe75e, 0xef7e, 0xef7e, 0xef7e, 0xef7e,
    0xef7e, 0xe71d, 0xdedc, 0xd6fc, 0xd6dc, 0xdf1d, 0xef7e, 0xef7e,
    0xef7e, 0xef5e, 0xef7e, 0xe75e, 0xe75e, 0xe75e, 0xef5e, 0xe75e,
    0xe75e, 0xe75e, 0xe75e, 0xe73e, 0xe73e, 0xe75e, 0xe75e, 0xdf1d,
    0x2a4b, 0x220a, 0x3aac, 0x536f, 0x6bf1, 0x6bf1, 0x6c12, 0x84d4,
    0x7432, 0xa597, 0xa577, 0x2a2b, 0x19ea, 0x328c, 0x7432, 0xa597,
    0xadd8, 0xadd8, 0xadd8, 0xc65a, 0xdefc, 0xcebc, 0xbe3a, 0xd6fc,
    0xe73e, 0xe73d, 0xdf1d, 0xe73d, 0xdefc, 0xce9b, 0xa5b8, 0x4b4f,
    0x1a0a, 0x0989, 0x0988, 0x0168, 0x0148, 0x0148, 0x0148, 0x0148,
    0x0147, 0x11a9, 0x2a8c, 0x09a9, 0x0989, 0x0988, 0x0148, 0x0968,
    0x0968, 0x0988, 0x0988, 0x11c9, 0x2a4b, 0x222b, 0x11ca, 0x2a6c,
    0x3aed, 0x3acd, 0x2a6c, 0x3aad, 0x2a6c, 0x19ea, 0x09a9, 0x1a0a,
    0x224b, 0x11ca, 0x11ea, 0x11ea, 0x11ea, 0x11ea, 0x11ea, 0x11ca,
    0x11ca, 0x42ab, 0x1a0a, 0x09ca, 0x11ea, 0x11ea, 0x11ea, 0x11ea,
    0x11ea, 0x11ca, 0x11c9, 0x11ca, 0x09a9, 0x09c9, 0x11ea, 0x09c9,
    0x09a9, 0x09a9, 0x09a9, 0x09a9, 0x09a9, 0x09aa, 0x11ea, 0x2a6c,
    0x226c, 0x120a, 0x120a, 0x120a, 0x120b, 0x120b, 0x120b, 0x120b,
    0x09eb, 0x5b2c, 0x6b2b, 0x120b, 0x11ea, 0x120b, 0x120b, 0x120b,
    0x120a, 0x1a0b, 0x1a2b, 0x120b, 0x120b, 0x120b, 0x120b, 0x120b,
    0x120b, 0x120b, 0x1a2a, 0x120a, 0x3a8a, 0x2a6a, 0x120b, 0x32ed,
    0x22ad, 0x122b, 0x122b, 0x122c, 0x1a4c, 0x1a4c, 0x1a4c, 0x1a4c,
    0x0a0b, 0x4b2e, 0x9beb, 0x634b, 0x124c, 0x122b, 0x1a4c, 0x122c,
    0x0a0b, 0x7bce, 0x942d, 0x226c, 0x226c, 0x122b, 0x1a4c, 0x1a4c,
    0x122c, 0x122b, 0x840c, 0xac6c, 0xb42a, 0x636b, 0x122c, 0x2aad,
    0x2ace, 0x1a6c, 0x1a6d, 0x1a6d, 0x1a6d, 0x1a6d, 0x1a6d, 0x1a6d,
    0x126d, 0x63ce, 0x7bc9, 0x9c69, 0x6bcc, 0x124c, 0x1a6d, 0x1a6c,
    0x1a6c, 0x9c6e, 0xb4cd, 0x5b8c, 0x32ed, 0x126d, 0x1a6d, 0x1a6c,
    0x1a4c, 0x124c, 0x3b0d, 0x942d, 0x7bac, 0x2aed, 0x1a8d, 0x2aee,
    0x2aee, 0x1a8d, 0x1a8d, 0x1a8d, 0x1a8d, 0x1aad, 0x1aae, 0x1a8e,
    0x330e, 0x7388, 0x6325, 0x73a8, 0x6bed, 0x1a8d, 0x1a8d, 0x126e,
    0x1a8d, 0x83eb, 0xacec, 0x6bcd, 0x126d, 0x1aae, 0x1a8e, 0x1a8d,
    0x22ad, 0x432d, 0x3b2e, 0x22ee, 0x4b4e, 0x2aee, 0x1aae, 0x2aef,
    0x2aee, 0x1a8d, 0x1aad, 0x1aad, 0x1aae, 0x1aae, 0x1aae, 0x1aae,
    0x2b0e, 0x73ec, 0x7c0c, 0x330d, 0x128e, 0x1aae, 0x22cd, 0x636c,
    0x73ac, 0x9c2b, 0xac6c, 0xb4ae, 0x432e, 0x128e, 0x22ce, 0x1aae,
    0x2b0e, 0x4b6e, 0x434e, 0x2ace, 0x1aae, 0x1aae, 0x1aae, 0x2aef,
    0x2aee, 0x1a8d, 0x1a8d, 0x1a6d, 0x1a8d, 0x126d, 0x2aed, 0x4b6d,
    0x330d, 0x22ce, 0x1a8d, 0x128d, 0x1a8d, 0x124d, 0x63ae, 0xdd90,
    0xddd1, 0xddf2, 0xd590, 0xa48f, 0x94b0, 0x1a8e, 0x534c, 0x534c,
    0x536c, 0x740d, 0x22ee, 0x1a8d, 0x1a8d, 0x1a8d, 0x1a6d, 0x2aee,
    0x2aad, 0x1a4c, 0x1a4c, 0x1a4c, 0x124c, 0x2aad, 0x9caf, 0x63ab,
    0x32cb, 0x1a4c, 0x122c, 0x1a4c, 0x1a6c, 0x122c, 0x32cd, 0xb530,
    0xd5d2, 0xa4d0, 0x8c2e, 0xb4cf, 0xbd10, 0x9c8f, 0x9c6d, 0xbd10,
    0xbd70, 0xb4ee, 0x6b8c, 0x1a6d, 0x1a6c, 0x1a4c, 0x1a4c, 0x2aad,
    0x228c, 0x120b, 0x120b, 0x120b, 0x120b, 0x8c4e, 0xcdd2, 0x948a,
    0x6bc8, 0x430a, 0x122b, 0x122b, 0x122b, 0x1a2c, 0x0a0b, 0x5b6c,
    0x6bad, 0x4b0c, 0x532c, 0x946e, 0x840e, 0xa40b, 0xacef, 0xcd71,
    0xd5b2, 0xd550, 0x942c, 0x4b2b, 0x2a8b, 0x120b, 0x120b, 0x228c,
    0x224c, 0x120a, 0x11ea, 0x09eb, 0x42ec, 0x948d, 0x948c, 0x73c9,
    0x5349, 0x842a, 0x638b, 0x120b, 0x09aa, 0x11ea, 0x11eb, 0x328a,
    0x5329, 0x7389, 0x7368, 0x52e6, 0x52e7, 0x7368, 0x6b27, 0x7ba9,
    0xa48d, 0x944d, 0x9c6c, 0x8409, 0x42ea, 0x122b, 0x1a2a, 0x1a4c,
    0x32ac, 0x3249, 0x632a, 0x636c, 0x7b8b, 0x6b49, 0x7b89, 0x6b4a,
    0x3a6b, 0x734b, 0x42cb, 0x4b4f, 0x63f1, 0x1a0b, 0x222b, 0x11ea,
    0x328b, 0x530b, 0x5ae9, 0x52e7, 0x52e6, 0x5ae6, 0x4a65, 0x5285,
    0x6327, 0x6328, 0x6328, 0x73a9, 0x73cb, 0x5b2a, 0x5309, 0x4aeb,
    0xce59, 0xad55, 0xbdb5, 0xad34, 0x7c30, 0x9c71, 0xb513, 0xacf2,
    0x942e, 0x634d, 0x6bd0, 0xc659, 0xffbe, 0xbe18, 0x222b, 0x0148,
    0x0968, 0x220a, 0x21e9, 0x0989, 0x11a8, 0x21e8, 0x21e8, 0x2a07,
    0x4aa8, 0x4aa9, 0x52eb, 0x5b2c, 0x634d, 0x73ae, 0x8471, 0xad75,
    0xdefc, 0xe73d, 0xef3d, 0xef5d, 0xef5d, 0xef3d, 0xe71c, 0xde9a,
    0xd69a, 0xd67a, 0xdedb, 0xe6fc, 0xdebb, 0xd69b, 0x9d56, 0x9d36,
    0xad77, 0xad97, 0xadb8, 0xa577, 0x9d56, 0x9d56, 0xad97, 0xa597,
    0xad97, 0xb5b8, 0xc63a, 0xc63a, 0xce7a, 0xd69b, 0xdebc, 0xdedc
};

unsigned short galaxy::pixels_pluto_jupiter[] = {
    0x9d34, 0x94f3, 0x94f3, 0x94f3, 0x94f3, 0x94f3, 0x94f3, 0x94f3,
    0x9d13, 0x94f3, 0x94f3, 0x94f3, 0x9d13, 0x9d13, 0x9cf3, 0x9d13,
    0x94f3, 0x94f3, 0x9d13, 0x9d13, 0x9d13, 0x9d13, 0x9d14, 0x9d34,
    0x9d34, 0x9d34, 0x9d34, 0x9d34, 0x9d14, 0x9d14, 0x9d14, 0x9d34,
    0x8451, 0x8c91, 0x8c71, 0x8471, 0x8c91, 0x8c91, 0x8471, 0x8c92,
    0x8cb2, 0x8cb2, 0x8c92, 0x8c92, 0x94b2, 0x8cb2, 0x8c91, 0x8c71,
    0x8c91, 0x8c71, 0x8c71, 0x8c91, 0x8c91, 0x8c71, 0x8c91, 0x8c91,
    0x8c91, 0x8c91, 0x8c71, 0x8c71, 0x8c91, 0x8c91, 0x8c91, 0x8c71,
    0x738d, 0x7bce, 0x7bcd, 0x7bce, 0x7bce, 0x7bce, 0x7bce, 0x7bce,
    0x7bce, 0x83ee, 0x7bce, 0x7bce, 0x83ee, 0x83ee, 0x83ee, 0x7bcd,
    0x83ee, 0x83cd, 0x83ee, 0x83ee, 0x83ee, 0x83ce, 0x7bce, 0x83ee,
    0x7bce, 0x83ce, 0x83ce, 0x83ce, 0x7bce, 0x83cd, 0x83ee, 0x7bad,
    0x83ee, 0x8c0f, 0x8c2f, 0x9450, 0x9450, 0x944f, 0x9450, 0x9450,
    0x9450, 0x9450, 0x9450, 0x8c4f, 0x942f, 0x942f, 0x8c2f, 0x840e,
    0x83ee, 0x8c0f, 0x8c2f, 0x8c2f, 0x8c2f, 0x8c2f, 0x8c2f, 0x8c2f,
    0x8c0e, 0x8c0f, 0x8c0e, 0x8c0e, 0x8c0e, 0x8c0e, 0x8c2e, 0x83ee,
    0x94b2, 0x9cd2, 0x9cf2, 0x9cd2, 0x9cf2, 0x9cd2, 0x9cf2, 0x9cf3,
    0x9cd2, 0x9cf2, 0x9cf3, 0xa513, 0xa4f3, 0xa4f2, 0x9cd2, 0x9cd2,
    0xa513, 0xa513, 0xa534, 0xa4f3, 0xa513, 0xa513, 0xa534, 0xa534,
    0xa513, 0xa4f3, 0x9cf3, 0x9cf3, 0x9cd2, 0x9cd2, 0x9cd2, 0x94b2,
    0xadb7, 0xbdf7, 0xb5d7, 0xb5b7, 0xb5b7, 0xb595, 0xad75, 0xad96,
    0xb5b6, 0xb5d6, 0xb5b6, 0xb574, 0xc5f6, 0xc638, 0xbe18, 0xbe38,
    0xc659, 0xc659, 0xc659, 0xc659, 0xc659, 0xbe39, 0xbe39, 0xbe59,
    0xb5f7, 0xb5d7, 0xbe18, 0xbe39, 0xbe39, 0xbe39, 0xbe39, 0xb5f8,
    0xad54, 0xad54, 0xb575, 0xb595, 0xb595, 0xb574, 0xb574, 0xb595,
    0xb574, 0xb553, 0xad12, 0xbd11, 0xbd11, 0xacd1, 0xa490, 0xa4b0,
    0xacf1, 0xacf1, 0xa4b0, 0xacd0, 0xacf1, 0xa4b0, 0xa4b1, 0xacf1,
    0xacd1, 0xacf2, 0xad12, 0xad33, 0xad12, 0xad54, 0xb554, 0xa534,
    0xb5d7, 0xc618, 0xc618, 0xc638, 0xc638, 0xc617, 0xc638, 0xc659,
    0xbe17, 0xbdf7, 0xc617, 0xc617, 0xc617, 0xc5f7, 0xbdf7, 0xbdf7,
    0xc617, 0xc617, 0xbdf7, 0xbdf7, 0xc5f6, 0xbdf6, 0xbdd6, 0xbdd6,
    0xbdf7, 0xc617, 0xc617, 0xbdf7, 0xc5f7, 0xc617, 0xc617, 0xb5d7,
    0xacf2, 0xb532, 0xb532, 0xad12, 0xb532, 0xbd52, 0xbd73, 0xb533,
    0xb533, 0xad12, 0xbd53, 0xbd74, 0xc5b4, 0xc5b4, 0xad54, 0xad33,
    0xb533, 0xbd53, 0xb553, 0xa513, 0xb553, 0xb553, 0xa4f2, 0x9cd2,
    0xad12, 0xb532, 0xbd53, 0xad12, 0xacf2, 0xad12, 0xbd52, 0xb532,
    0x9c90, 0xac8f, 0xacd0, 0xacb0, 0xa4d1, 0xacd1, 0xb511, 0xad12,
    0xad33, 0xacf1, 0xb512, 0xacf1, 0xb4f1, 0xb532, 0xacf1, 0xa48f,
    0xac6e, 0xacb0, 0xacb0, 0xacb0, 0xa490, 0xa46f, 0xa48f, 0xa48f,
    0xacb0, 0xa4b0, 0xa48f, 0xa46e, 0xac8f, 0xacd0, 0xacd0, 0x9c6f,
    0xad75, 0xb5b5, 0xbdd6, 0xb5b6, 0xad95, 0xb595, 0xad74, 0xb595,
    0xb596, 0xb5b6, 0xb596, 0xb595, 0xb596, 0xbdd6, 0xb5b6, 0xad95,
    0xad95, 0xb596, 0xb5b6, 0xb5d7, 0xb5b6, 0xb596, 0xb5d7, 0xb5d7,
    0xb5d7, 0xb5f7, 0xb5d7, 0xb5f7, 0xb5d7, 0xb5d7, 0xbdd7, 0xad75,
    0x9cd2, 0xad13, 0xad13, 0xa4f2, 0xa4d1, 0x9cb0, 0xa4d1, 0x9cb1,
    0xa4b1, 0xa4d1, 0xa4b1, 0xa4d1, 0xad12, 0xacf2, 0x9cb1, 0x9cb1,
    0x9c70, 0x9c90, 0x9470, 0x9cb1, 0xa4d1, 0xa4b1, 0x9cd1, 0x9cd1,
    0x9cb1, 0x9c91, 0x9cb1, 0xa4d1, 0xa4d1, 0xa4b1, 0xa4d1, 0x9cd1,
    0x840f, 0x8c2f, 0x944f, 0x8c0e, 0x8c2f, 0x8c0e, 0x8c0e, 0x83ee,
    0x8c2e, 0x942f, 0x942f, 0x8c2f, 0x8c0e, 0x8c0e, 0x8bee, 0x8c0e,
    0x8bee, 0x8c0e, 0x942f, 0x8c2f, 0x8c0e, 0x8c2f, 0x840e, 0x8c0e,
    0x83ee, 0x83ce, 0x83ee, 0x83ee, 0x83ee, 0x840e, 0x8c2f, 0x840f,
    0x7bce, 0x83ee, 0x840e, 0x83ee, 0x83ee, 0x83ce, 0x83ce, 0x7bce,
    0x7bce, 0x7bce, 0x7bce, 0x73ae, 0x7bce, 0x7bce, 0x7bae, 0x73ae,
    0x7bce, 0x7bee, 0x7bee, 0x7bce, 0x83ee, 0x83ce, 0x83ee, 0x7bee,
    0x7bce, 0x7bce, 0x7bce, 0x83ee, 0x83ee, 0x7bce, 0x7bce, 0x7bce,
    0x8430, 0x8430, 0x842f, 0x7c0f, 0x7c0f, 0x7c0f, 0x8430, 0x8430,
    0x7c0f, 0x8430, 0x842f, 0x7c0f, 0x842f, 0x8430, 0x842f, 0x7c0f,
    0x7c0f, 0x7c0f, 0x7c0f, 0x840f, 0x840f, 0x7c0f, 0x7bef, 0x7bef,
    0x7c0f, 0x840f, 0x7c0f, 0x7c0f, 0x842f, 0x842f, 0x7c0f, 0x8430,
    0x8491, 0x8451, 0x8451, 0x8451, 0x8471, 0x8471, 0x8471, 0x8471,
    0x8471, 0x8471, 0x8471, 0x8471, 0x8471, 0x8471, 0x7c30, 0x8450,
    0x8450, 0x8450, 0x8450, 0x8450, 0x7c30, 0x7c30, 0x7c30, 0x7c30,
    0x7c30, 0x7c30, 0x7c30, 0x7c30, 0x7c50, 0x8450, 0x8450, 0x8471,
    0xc659, 0xc659, 0xc679, 0xbe39, 0xb5f8, 0xbe18, 0xbe39, 0xbe39,
    0xbe39, 0xb618, 0xadb7, 0xa556, 0xa576, 0xadb7, 0xadb7, 0xadb7,
    0xadb7, 0xb618, 0xbe39, 0xbe18, 0xb618, 0xbe18, 0xbe59, 0xc67a,
    0xc67a, 0xc69a, 0xc659, 0xc659, 0xbe59, 0xbe39, 0xbe38, 0xbe39,
    0xb5f8, 0xbe18, 0xbe18, 0xbe38, 0xadd7, 0xadb7, 0xa597, 0x9d35,
    0x9515, 0x94f5, 0x8473, 0x6bb0, 0x5b2e, 0x5b2e, 0x5b2e, 0x52cd,
    0x5b0e, 0x6bb0, 0x8472, 0x94f5, 0x9d35, 0xadb7, 0xbe59, 0xce9a,
    0xce9a, 0xc69a, 0xc659, 0xb618, 0xadb7, 0xa576, 0xa596, 0xa576,
    0xbe39, 0xb5f8, 0xb5f8, 0xb5f8, 0xb5d8, 0x9d56, 0x8cb3, 0x8493,
    0x8493, 0x7c52, 0x7412, 0x6bb0, 0x636f, 0x634e, 0x5b2e, 0x636f,
    0x73f1, 0x8493, 0x8493, 0x8cb3, 0x9d35, 0xadd7, 0xbe59, 0xc69a,
    0xc69a, 0xbe59, 0xb5f8, 0xa576, 0x9d36, 0x9d56, 0xa577, 0xadb7,
    0xcebb, 0xc67a, 0xadb7, 0xadb7, 0x9d35, 0x8493, 0x7c52, 0x8453,
    0x8473, 0x7c52, 0x7411, 0x636e, 0x5b2e, 0x5b2d, 0x4aac, 0x52ed,
    0x636e, 0x7c52, 0x9d56, 0x8cd4, 0x8452, 0x8493, 0x8cd4, 0xa556,
    0xadd7, 0x9d35, 0x8c93, 0x73f1, 0x6bb0, 0x7c52, 0x8cb4, 0xadb7,
    0xc659, 0xbe18, 0x8cb4, 0x8493, 0x8473, 0x6bb0, 0x5b2e, 0x6b8f,
    0x7c32, 0x7c52, 0x8cf4, 0x5b4e, 0x424a, 0x426a, 0x424a, 0x424a,
    0x3a2a, 0x52cc, 0x7412, 0x7c12, 0x636f, 0x6b90, 0x634f, 0x6bb0,
    0x7c52, 0x73f1, 0x52cc, 0x424a, 0x426b, 0x52cc, 0x636e, 0x8cd4,
    0x8c93, 0x7c31, 0x636e, 0x5b4e, 0x5b2d, 0x52ed, 0x638f, 0x7411,
    0x8473, 0x8cd4, 0xadb7, 0x94f5, 0x7c11, 0x4aac, 0x4a8b, 0x52cc,
    0x4aac, 0x4a8c, 0x5b0e, 0x634f, 0x424b, 0x4a8b, 0x424a, 0x3a2a,
    0x39e9, 0x3a2a, 0x422a, 0x3a2a, 0x3a2a, 0x426b, 0x5b2e, 0x7c31,
    0x6b8f, 0x4aac, 0x424a, 0x426b, 0x4a8b, 0x4aac, 0x636f, 0x8493,
    0xadb7, 0xb5f8, 0xb618, 0xadd7, 0x94f5, 0x6b90, 0x5b0e, 0x5b0e,
    0x6bb0, 0x6bb0, 0x52ed, 0x426b, 0x3a0a, 0x31c9, 0x39e9, 0x422a,
    0x3a0a, 0x31c9, 0x39e9, 0x3a2a, 0x426b, 0x4aac, 0x636f, 0x73f1,
    0x5b0d, 0x4a8c, 0x426b, 0x424a, 0x4a8c, 0x52ee, 0x6bd1, 0x8cd4,
    0xbe39, 0xceba, 0xcedb, 0xc67a, 0xadb7, 0x7c32, 0x634f, 0x636f,
    0x7412, 0x7c32, 0x6b90, 0x5b2f, 0x52cd, 0x426b, 0x4a6b, 0x52cd,
    0x426b, 0x39e9, 0x31c9, 0x424b, 0x5aed, 0x5b2e, 0x5b2e, 0x636f,
    0x5b0d, 0x4a8c, 0x39e9, 0x31c8, 0x3a2a, 0x52cd, 0x6b90, 0x94f5,
    0xadb7, 0xb618, 0xb5f8, 0xadb7, 0xa597, 0xa577, 0x73f1, 0x634f,
    0x6370, 0x6b90, 0x636f, 0x6370, 0x632f, 0x52cd, 0x52ed, 0x5b2e,
    0x426b, 0x39e9, 0x3a2a, 0x4aac, 0x5b2e, 0x636f, 0x5b0e, 0x5b0e,
    0x6b90, 0x424b, 0x31c8, 0x31a8, 0x3a0a, 0x426b, 0x7411, 0x9d35,
    0xadb7, 0x9d36, 0x7411, 0x7411, 0x9d56, 0xbe5a, 0x9d56, 0x8cb4,
    0x7c32, 0x636f, 0x5b0e, 0x5b0e, 0x634f, 0x6b90, 0x636f, 0x73f1,
    0x6bb0, 0x52ee, 0x52ed, 0x5b4f, 0x73d1, 0x7411, 0x73f1, 0x6bb0,
    0x7c11, 0x426b, 0x3a09, 0x31e9, 0x3a09, 0x5b4e, 0x9535, 0xb5f7,
    0xb618, 0x8cd4, 0x5b2e, 0x6b90, 0x8472, 0xadb7, 0xbe59, 0xb618,
    0x94f5, 0x7c32, 0x6bb0, 0x5b2e, 0x634f, 0x8453, 0x8472, 0x94f4,
    0x94f4, 0x8473, 0x7c52, 0x8473, 0x94f4, 0x9d35, 0x9515, 0x94f4,
    0x7411, 0x52cc, 0x426b, 0x4a8b, 0x5b2d, 0x8492, 0xb5f7, 0xb618,
    0xb5f7, 0xa596, 0x8472, 0x7c52, 0x8cd3, 0x9d55, 0xc679, 0xb5f8,
    0xadb7, 0x94f4, 0x8452, 0x7c11, 0x6bb0, 0x73d0, 0x8452, 0x9d35,
    0x9d56, 0x94f4, 0x94f4, 0x9515, 0xa556, 0x9514, 0x8c93, 0x94f4,
    0x94f4, 0x73f1, 0x8cb3, 0x9514, 0x9d35, 0xadb7, 0xb618, 0xadd7,
    0x9d55, 0xa596, 0xb5f8, 0xa597, 0xadb7, 0xbe59, 0xbe59, 0xadd7,
    0xadd7, 0x9d55, 0x9514, 0x8c93, 0x73f1, 0x7c31, 0x73f0, 0x7c31,
    0x8472, 0x7c31, 0x8472, 0x94d4, 0x8cb3, 0x94f4, 0x8cd4, 0x9d15,
    0x9d35, 0x9d35, 0xb5d7, 0xbe39, 0xb5f8, 0xb5f8, 0xb5f7, 0xadd7,
    0xa596, 0xa576, 0xadd7, 0xb5f8, 0xbe59, 0xc69a, 0xce9a, 0xce9a,
    0xc679, 0xb618, 0xb5f7, 0xadd7, 0x9d55, 0x9514, 0x8cd4, 0x8493,
    0x8493, 0x8493, 0x8493, 0x8452, 0x8472, 0x8cb3, 0x94f4, 0x9d15,
    0xb5f7, 0xbe18, 0xb5f8, 0xbe39, 0xbe59, 0xbe39, 0xbe39, 0xbe59,
    0xbe59, 0xbe59, 0xc679, 0xc69a, 0xceba, 0xd6db, 0xd6fb, 0xd6fb,
    0xd6db, 0xd6db, 0xceba, 0xc69a, 0xc679, 0xbe59, 0xb618, 0xb5d7,
    0xadd7, 0xa596, 0xa556, 0x9d55, 0xa556, 0xa596, 0xadd7, 0xadb7,
    0xcebb, 0xceba, 0xce9a, 0xce9a, 0xce9a, 0xce9a, 0xce9a, 0xceba,
    0xcebb, 0xceba, 0xceba, 0xceba, 0xceba, 0xceba, 0xd6db, 0xd6db,
    0xd6db, 0xd6db, 0xcebb, 0xcebb, 0xd6db, 0xcebb, 0xceba, 0xc69a,
    0xc679, 0xc659, 0xbe59, 0xbe59, 0xc679, 0xc67a, 0xc69a, 0xceba
};