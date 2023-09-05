#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <chrono>
#include <string>

class ClockWindow
{
public:
    ClockWindow(int width, int height) : width(width), height(height)
    {
        // init OpenGL and SDL
        window = SDL_CreateWindow("Analog Clock",
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  width,
                                  height + 50,
                                  SDL_WINDOW_OPENGL);
        SDL_GL_CreateContext(window);

        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-width, width, -height, height, 1, -1);
        glMatrixMode(GL_MODELVIEW);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    }

    ~ClockWindow()
    {
        SDL_DestroyWindow(window);
    }

    void clear()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void swapBuffers()
    {
        SDL_GL_SwapWindow(window);
    }

    int getWidth() const
    {
        return width;
    }

    int getHeight() const
    {
        return height;
    }

private:
    SDL_Window *window;
    int width, height;
};

class AnalogClock
{
public:
    AnalogClock(int x,
                int y,
                int rad,
                int gmt,
                std::string label,
                ClockWindow &clockWindow) : x(x),
                                            y(y),
                                            radius(rad),
                                            offsetHours(gmt),
                                            label(label),
                                            clockWindow(clockWindow) {}

    void draw()
    {
        glLoadIdentity();
        glTranslatef(x, y, 0);
        drawClock();
    }

private:
    ClockWindow &clockWindow;
    int x, y, radius, offsetHours;
    std::string label;

    void drawClockHand(int angle, float length, float weight)
    {
        glPushMatrix();
        glRotatef(angle, 0, 0, 1); // Rotate clock hand

        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(weight, 0);
        glVertex2f(weight, length);
        glVertex2f(-weight, length);
        glVertex2f(-weight, 0);
        glEnd();

        drawCircle(-90, 90, 0, length, weight, GL_TRIANGLE_FAN);
        drawCircle(0, 360, 0, 0, weight * 2, GL_TRIANGLE_FAN);

        glPopMatrix();
    }

    void drawCircle(int angleBegin, int angleEnd, int x, int y, int r, int typeDraw)
    {
        glBegin(typeDraw);
        for (int i = angleBegin; i <= angleEnd; ++i)
        {
            float radian = 3.14159265358979323846 * i / 180;
            float x_ = r * sin(radian) + x;
            float y_ = r * cos(radian) + y;
            glVertex2f(x_, y_);
        }
        glEnd();
    }

    void drawClock()
    {
        glColor3ub(195, 196, 198);
        drawCircle(0, 360, 0, 0, radius, GL_TRIANGLE_FAN); // Circle
        glEnable(GL_LINE_SMOOTH);
        glColor3ub(0, 0, 0);
        drawCircle(0, 360, 0, 0, radius, GL_LINE_LOOP); // Ring
        glPushMatrix();

        // Draw number
        for (int i = 1; i <= 12; ++i)
        {
            float radian = 3.14159265358979323846 * i / 6;
            float x = radius * 0.8 * sin(radian);
            float y = radius * 0.8 * cos(radian);
            char numerical[3];
            snprintf(numerical, sizeof(numerical), "%2d", i); // Formated number
            if (i < 10)
                glRasterPos2f(x - 10, y - 15);
            else
                glRasterPos2f(x - 20, y - 15);

            for (int j = 0; j < 2; ++j)
            {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, numerical[j]);
            }
        }

        // Draw label
        glColor3f(1.0, 1.0, 1.0);
        int size = label.size();
        glRasterPos2f(-11 * size, -radius - 50);
        for (int i = 0; i < size; ++i)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, label[i]);
        }
        glPopMatrix();

        // Get time
        auto currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        struct tm *timeInfo = localtime(&currentTime);
        int myOffsetHours = timeInfo->tm_gmtoff / 3600; // calculate offset
        int hours = timeInfo->tm_hour + offsetHours - myOffsetHours;
        int minutes = timeInfo->tm_min;
        int seconds = timeInfo->tm_sec;

        glColor3f(0.4, 0.4, 0.4);
        drawClockHand(-minutes * 6, radius * 0.7, radius * 0.025); // minutes hand clock (min - 6 degrees)
        glColor3f(0.1, 0.1, 0.1);
        drawClockHand(-((hours % 12 * 30) + (minutes * 0.5)), radius * 0.5, radius * 0.035); // hours hand clock (hours - 30 degrees)
        glColor3f(1.0, 0.2, 0.2);
        drawClockHand(-seconds * 6, radius * 0.85, radius * 0.015); // seconds hand clock
        glFlush();
    }
};

int main(int argc, char *args[])
{
    glutInit(&argc, args);
    SDL_Init(SDL_INIT_VIDEO);

    ClockWindow clockWindow(800, 850);
    AnalogClock my_clock(0, 550, 300, 3, "Moskow", clockWindow);
    AnalogClock ny_clock(0, -50, 200, -4, "New York", clockWindow);
    AnalogClock paris_clock(500, -50, 200, 2, "Paris", clockWindow);
    AnalogClock london_clock(500, -550, 200, 1, "London", clockWindow);
    AnalogClock moscow_clock(0, -550, 200, 3, "Moskow", clockWindow);
    AnalogClock tokyo_clock(-500, -50, 200, 9, "Tokyo", clockWindow);
    AnalogClock sydney_clock(-500, -550, 200, 10, "Sydney", clockWindow);

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
        clockWindow.clear();
        my_clock.draw();
        ny_clock.draw();
        paris_clock.draw();
        london_clock.draw();
        moscow_clock.draw();
        tokyo_clock.draw();
        sydney_clock.draw();
        clockWindow.swapBuffers();
        SDL_Delay(1000);
    }

    SDL_Quit();
    return 0;
}
