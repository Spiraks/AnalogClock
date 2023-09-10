cmake_minimum_required(VERSION 3.0.0)
project(AnalogClock VERSION 0.1.0 LANGUAGES C CXX)
add_executable(AnalogClock src/main.cpp src/AnalogClock.cpp src/ClockWindow.cpp)
target_link_libraries(AnalogClock SDL2main SDL2)
target_link_libraries(AnalogClock glut GL GLU)
add_executable(TEST src/test.cpp src/AnalogClock.cpp src/ClockWindow.cpp)
target_link_libraries(TEST SDL2main SDL2)
target_link_libraries(TEST glut GL GLU)


