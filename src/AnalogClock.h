#pragma once

#include <chrono>
#include <string>
#include <GL/gl.h>
#include <GL/glut.h>
#include "ClockWindow.h"

class AnalogClock
{
public:
    AnalogClock(int x,
                int y,
                int radius,
                int gmt,
                std::string label,
                ClockWindow &clockWindow);

    void Draw();

private:
    ClockWindow &clockWindow_;
    int x_;
    int y_;
    int radius_;
    int offsetHours;
    std::string label_;

    void DrawClockHand(int angle, float length, float weight);
    void DrawCircle(int angleBegin, int angleEnd, int x, int y, int r, int typeDraw);
    void DrawDial();
    void DrawNumber();
    void DrawLabel();
};