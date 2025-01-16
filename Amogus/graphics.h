#pragma once

#include <windows.h>
#include <iostream>
#include <glm/glm.hpp>
#include "clock.h"

#define PI 3.1415
#define DEG2RAD PI / 180

extern HANDLE hConsole;
extern char* screenBuffer;
extern int width, height;
extern Clock fpsClock;

namespace util {
	template<typename T>
	void setCursorPos(T x, T y);
	void clear();
	int bufferIndex(int x, int y);
}
 
void init();
void step();
void draw();
glm::vec3 getNormal(glm::vec3 p, float R, float r, glm::vec3 rotation);
char getLight(float light);
char traceTorus(glm::vec3 rd, glm::vec3 ro, glm::vec3 rotation);
glm::vec3 rotate(glm::vec3 p, glm::vec3 angles);