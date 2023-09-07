#pragma once
#include <SDL2/SDL.h>
#include <GL/gl.h>

class ClockWindow
{
public:
    ClockWindow(int width, int height);
    ~ClockWindow();
    void Clear();
    void SwapBuffers();
    int GetWidth() const;
    int GetHeight() const;

private:
    SDL_Window *window;
    int width_;
    int height_;
};
