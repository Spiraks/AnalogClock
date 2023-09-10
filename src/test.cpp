#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "AnalogClock.h"
#include "ClockWindow.h"

int main(int argc, char *args[])
{
    glutInit(&argc, args);
    SDL_Init(SDL_INIT_VIDEO);

    ClockWindow clockWindow(600, 600);
    AnalogClock my_clock(0, 0, 250, 3, "Moscow", clockWindow);

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
        
        clockWindow.SwapBuffers();
        SDL_Delay(1000);
    }

    SDL_Quit();
    return 0;
}
