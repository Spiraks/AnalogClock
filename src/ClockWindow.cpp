#include <SDL2/SDL.h>
#include <GL/gl.h>
#include "ClockWindow.h"

ClockWindow::ClockWindow(int width, int height) : width_(width), height_(height)
{
    // init OpenGL and SDL
    window = SDL_CreateWindow("Analog Clock",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              width_,
                              height_,
                              SDL_WINDOW_OPENGL);
    SDL_GL_CreateContext(window);

    glViewport(0, 0, width_, height_);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-width_ / 2, width_ / 2, -height_ / 2, height_ / 2, 1, -1);
    glMatrixMode(GL_MODELVIEW);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

ClockWindow::~ClockWindow()
{
    SDL_DestroyWindow(window);
}

void ClockWindow::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void ClockWindow::SwapBuffers()
{
    SDL_GL_SwapWindow(window);
}

int ClockWindow::GetWidth() const
{
    return width_;
}

int ClockWindow::GetHeight() const
{
    return height_;
}
