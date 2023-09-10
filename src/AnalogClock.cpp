#include <chrono>
#include <string>
#include <GL/gl.h>
#include <GL/glut.h>
#include "AnalogClock.h"

/**
 * @brief Constructor of the AnalogClock class.
 *
 * @param x X-coordinate of the clock center.
 * @param y Y-coordinate of the clock center.
 * @param radius The radius of the clock.
 * @param gmt Timezone offset from GMT.
 * @param label Label for the clock.
 * @param clockWindow Reference to the window where the clock is displayed.
 */
AnalogClock::AnalogClock(int x,
                         int y,
                         int radius,
                         int gmt,
                         std::string label,
                         ClockWindow &clockWindow) : x_(x),
                                                     y_(y),
                                                     radius_(radius),
                                                     offsetHours(gmt),
                                                     label_(label),
                                                     clockWindow_(clockWindow) {}

/**
 * @brief Draw the clock.
 */
void AnalogClock::Draw()
{
    glPopMatrix();
    glLoadIdentity();
    glTranslatef(x_, y_, 0);
    DrawDial();
    DrawNumber();
    DrawLabel();

    auto currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct tm *timeInfo = localtime(&currentTime);
    int myOffsetHours = timeInfo->tm_gmtoff / 3600; // calculate offset
    int hours = timeInfo->tm_hour + offsetHours - myOffsetHours;
    int minutes = timeInfo->tm_min;
    int seconds = timeInfo->tm_sec;

    glColor3f(0.4, 0.4, 0.4);
    DrawClockHand(-minutes * 6, radius_ * 0.7, radius_ * 0.025); // minutes hand clock (min - 6 degrees)
    glColor3f(0.1, 0.1, 0.1);
    DrawClockHand(-((hours % 12 * 30) + (minutes * 0.5)), radius_ * 0.5, radius_ * 0.035); // hours hand clock (hours - 30 degrees)
    glColor3f(1.0, 0.2, 0.2);
    DrawClockHand(-seconds * 6, radius_ * 0.85, radius_ * 0.015); // seconds hand clock

    glPushMatrix();
    glFlush();
}

/**
 * @brief Draw a clock hand.
 *
 * @param angle Rotation angle of the clock hand.
 * @param length Length of the clock hand.
 * @param weight Thickness of the clock hand.
 */
void AnalogClock::DrawClockHand(int angle, float length, float weight)
{
    glPushMatrix();
    glRotatef(angle, 0, 0, 1); // Rotate clock hand

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(weight, 0);
    glVertex2f(weight, length);
    glVertex2f(-weight, length);
    glVertex2f(-weight, 0);
    glEnd();

    DrawCircle(-90, 90, 0, length, weight, GL_TRIANGLE_FAN);
    DrawCircle(0, 360, 0, 0, weight * 2, GL_TRIANGLE_FAN);

    glPopMatrix();
}

/**
 * @brief Draw a circle.
 *
 * @param angleBegin Starting angle of the arc.
 * @param angleEnd Ending angle of the arc.
 * @param x X-coordinate of the circle center.
 * @param y Y-coordinate of the circle center.
 * @param r Radius of the circle.
 * @param typeDraw Type of drawing (GL_TRIANGLE_FAN, GL_LINE_LOOP, etc.).
 */
void AnalogClock::DrawCircle(int angleBegin, int angleEnd, int x, int y, int r, int typeDraw)
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

/**
 * @brief Draw numbers on the clock face.
 */
void AnalogClock::DrawNumber(){
    for (int i = 1; i <= 12; ++i)
    {
        float radian = 3.14159265358979323846 * i / 6;
        float x = radius_ * 0.85 * sin(radian);
        float y = radius_ * 0.85 * cos(radian);
        char numerical[3];
        snprintf(numerical, sizeof(numerical), "%2d", i); // Formatted number
        if (i < 10)
            glRasterPos2f(x - 7, y - 7);
        else
            glRasterPos2f(x - 10, y - 7);

        for (int j = 0; j < 2; ++j)
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, numerical[j]);
        }
    }
}

/**
 * @brief Draw a label below the clock.
 */
void AnalogClock::DrawLabel(){
    glColor3f(1.0, 1.0, 1.0);
    int size = label_.size();
    glRasterPos2f(-7 * size, -radius_ - 20);
    for (int i = 0; i < size; ++i)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, label_[i]);
    }
}

/**
 * @brief Draw the clock face.
 */
void AnalogClock::DrawDial()
{
    glColor3ub(195, 196, 198);
    DrawCircle(0, 360, 0, 0, radius_, GL_TRIANGLE_FAN); // Circle
    glEnable(GL_LINE_SMOOTH);
    glColor3ub(0, 0, 0);
    DrawCircle(0, 360, 0, 0, radius_, GL_LINE_LOOP); // Ring
}
