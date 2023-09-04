#include <SDL2/SDL.h>
#include <stdio.h>
#include <GL/gl.h>
#include <chrono>
#include <cmath>
#include <iostream>
#include <chrono>

void calculateTime() {
	// Получаем текущее время в UTC
	std::time_t now_c = std::time(nullptr);
    std::tm* local_time_info = std::localtime(&now_c);
    int offset_hours;
    if (local_time_info != nullptr) {
		offset_hours = local_time_info->tm_gmtoff / 3600;
        std::cout << "Ваш текущий часовой пояс (UTC): " << offset_hours << std::endl;
    } else {
        std::cerr << "Не удалось получить информацию о часовом поясе." << std::endl;
    }
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    // Определяем смещение для разных часовых поясов
    int sydney_offset = 10-offset_hours;   // Сидней UTC+10
    // Вычисляем локальное время для каждого часового пояса
    std::chrono::system_clock::time_point sydney_time = now + std::chrono::hours(sydney_offset);
    // Преобразуем локальное время в структуру std::tm для вывода
    std::time_t sydney_time_c = std::chrono::system_clock::to_time_t(sydney_time);
    // Выводим текущее время в разных городах
    std::cout << "Сидней: " << std::asctime(std::localtime(&sydney_time_c));
}

class AnalogClock {
public:
    AnalogClock(int x, int y, int rad) {
        // Инициализация OpenGL и SDL
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow("Analog Clock", 
			SDL_WINDOWPOS_UNDEFINED, 
			SDL_WINDOWPOS_UNDEFINED, 
			SCREEN_WIDTH, 
			SCREEN_HEIGHT, 
			SDL_WINDOW_OPENGL);
        SDL_GL_CreateContext(window);

        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        int win_x = 800;
        glOrtho(-500, 500, -500, 500, 1, -1);
        glMatrixMode(GL_MODELVIEW);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    ~AnalogClock() {
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void draw() {
        glClear(GL_COLOR_BUFFER_BIT);
        drawClock();
        SDL_GL_SwapWindow(window);
        SDL_Delay(1000);  // Задержка 1 секунда
    }

private:
    SDL_Window* window;
    static const int SCREEN_WIDTH = 800;
    static const int SCREEN_HEIGHT = 800;
    void drawClockHand(int angle, float lenght, float weight) { // Отрисовка стрелок
        int x = 0, y = 0;
        glPushMatrix();
		glRotatef(angle, 0, 0, 1);  // оклонение строелки в градусах
		glBegin(GL_TRIANGLE_FAN);
        glVertex2f(weight + x, y);
        glVertex2f(weight + x, lenght + y);
        float r = weight/2;
        glVertex2f(0, lenght);
        for (int i = -90; i <= 90; ++i) {
			float radian = 3.14159265358979323846 * i / 180;
			float x_ = weight * sin(radian);
			float y_ = weight * cos(radian) + lenght;
			glVertex2f(x_, y_);
		}
        glVertex2f(-weight + x, lenght + y);
        glVertex2f(-weight + x, y);
        glVertex2f(x, y);
        weight = weight*2;
        for (int i = 0; i <= 360; ++i) {
			float radian = 3.14159265358979323846 * i / 180;
			float x = weight * sin(radian);
			float y = weight * cos(radian);
			glVertex2f(x, y);
		}

		glEnd();
		glPopMatrix();
        
    }
    void drawClock() {
        // Отрисовка циферблата
		glBegin(GL_TRIANGLE_FAN);
		glColor3ub(195, 196, 198);
		glVertex2f(0, 0);         
		for (int i = 0; i <= 360; ++i) {
			float radian = 3.14159265358979323846 * i / 180;
			float x = 200 * sin(radian);
			float y = 200 * cos(radian);
			glVertex2f(x, y);
		}
        
		glEnd();
        glPushMatrix();

		// Получение текущего времени
		auto currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		struct tm* timeInfo = localtime(&currentTime);
		int hours = timeInfo->tm_hour;
		int minutes = timeInfo->tm_min;
		int seconds = timeInfo->tm_sec;

        glColor3f(0.4, 0.4, 0.4);  // Зеленый цвет
        drawClockHand(-minutes * 6, 120, 2);
        glColor3f(0.1, 0.1, 0.1);  // Синий цвет
        drawClockHand(-((hours % 12 * 30) + (minutes * 0.5)), 90 ,3);
        glColor3f(1.0, 0.2, 0.2);  // Красный цвет
		drawClockHand(-seconds * 6, 150, 1);
		glFlush();
    }
};

int main(int argc, char* args[]) {
    AnalogClock analogClock(0,0,200);

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        analogClock.draw();
    }

    return 0;
}



