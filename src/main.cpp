#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "AnalogClock.h"
#include "ClockWindow.h"

int main(int argc, char *args[])
{
    glutInit(&argc, args);
    SDL_Init(SDL_INIT_VIDEO);

    ClockWindow clockWindow(800, 850);
    AnalogClock my_clock(0, 250, 150, 3, "Moscow", clockWindow);
    AnalogClock ny_clock(0, -30, 100, -4, "New York", clockWindow);
    AnalogClock moscow_clock(0, -280, 100, 3, "Moscow", clockWindow);
    AnalogClock paris_clock(250, -30, 100, 2, "Paris", clockWindow);
    AnalogClock london_clock(250, -280, 100, 1, "London", clockWindow);
    AnalogClock tokyo_clock(-250, -30, 100, 9, "Tokyo", clockWindow);
    AnalogClock sydney_clock(-250, -280, 100, 10, "Sydney", clockWindow);

    bool quit = false;
    SDL_Event e;

    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        clockWindow.Clear();
        my_clock.Draw();
        ny_clock.Draw();
        paris_clock.Draw();
        london_clock.Draw();
        moscow_clock.Draw();
        tokyo_clock.Draw();
        sydney_clock.Draw();
        clockWindow.SwapBuffers();
        SDL_Delay(1000);
    }

    SDL_Quit();
    return 0;
}
