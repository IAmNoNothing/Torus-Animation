#include "graphics.h"

HANDLE hConsole;
char* screenBuffer;
int width, height;
Clock fpsClock;

void init()
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	width = csbi.dwSize.X;
	height = csbi.dwSize.Y;
    screenBuffer = new char[width * (height - 1) + 1];
    screenBuffer[width * (height - 1)] = '\0';
    screenBuffer[0] = '\0';

    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(hConsole, &cci);
    cci.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cci);

	util::clear();
}

void draw() {
    glm::vec3 rotation(fpsClock.ticks * DEG2RAD / 4);
	for (int y = 0; y < height - 1; y++) {
		for (int x = 0; x < width; x++) {
			glm::vec2 coord(x, y);
			coord /= glm::vec2(width, height);
			coord -= 0.5;
			coord *= 2;
            glm::vec3 rd(coord, -1);
			char c = traceTorus(rd, glm::vec3(0, 0, 3.5), rotation);
			screenBuffer[util::bufferIndex(x, y)] = c;
		}
	}
	printf("%sTorus animation by IAmNoNothing | Console size: %dx%d FPS: %d", screenBuffer, width, height, fpsClock.currentFPS);
}

float torusSDF(glm::vec3 p, float R, float r, glm::vec3 rotation) {
    p = rotate(p, rotation);
    glm::vec2 q = glm::vec2(glm::length(glm::vec2(p.x, p.z)) - R, p.y);
    return glm::length(q) - r;
}

glm::vec3 rotate(glm::vec3 p, glm::vec3 angles) {
    float sinX = sin(angles.x), cosX = cos(angles.x);
    glm::mat3 rotX = glm::mat3(
        1, 0, 0,
        0, cosX, -sinX,
        0, sinX, cosX
    );

    float sinY = sin(angles.y), cosY = cos(angles.y);
    glm::mat3 rotY = glm::mat3(
        cosY, 0, sinY,
        0, 1, 0,
        -sinY, 0, cosY
    );

    float sinZ = sin(angles.z), cosZ = cos(angles.z);
    glm::mat3 rotZ = glm::mat3(
        cosZ, -sinZ, 0,
        sinZ, cosZ, 0,
        0, 0, 1
    );

    return rotZ * rotY * rotX * p;
}

glm::vec3 getNormal(glm::vec3 p, float R, float r, glm::vec3 rotation) {
    float epsilon = 0.001;
    glm::vec3 dx(epsilon, 0, 0);
    glm::vec3 dy(0, epsilon, 0);
    glm::vec3 dz(0, 0, epsilon);

    float nx = torusSDF(p + dx, R, r, rotation) - torusSDF(p - dx, R, r, rotation);
    float ny = torusSDF(p + dy, R, r, rotation) - torusSDF(p - dy, R, r, rotation);
    float nz = torusSDF(p + dz, R, r, rotation) - torusSDF(p - dz, R, r, rotation);

    return glm::normalize(glm::vec3(nx, ny, nz));
}

char getLight(float light) {
    const char* chars = "@WMB$%8&#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'.";
    size_t size = sizeof(chars);

    if (light <= 0) {
        return '.';
    }
    if (light > 1.0) {
        return chars[0];
    }
    size_t index = (size_t)((1.0 - light) * (float)size);
    return chars[index];
}

char traceTorus(glm::vec3 rd, glm::vec3 ro, glm::vec3 rotation) {
    float R = 2.0;
    float r = 0.5;

    float t = 0.0;
    float maxDist = 100.0;
    float epsilon = 0.001;

    for (int i = 0; i < 100; i++) {
        glm::vec3 p = ro + rd * t;
        float dist = torusSDF(p, R, r, rotation);

        if (dist < epsilon) {
            glm::vec3 normal = getNormal(p, R, r, rotation);
            glm::vec3 lightDir = glm::normalize(glm::vec3(-1.0, -1.0, 1.0));
            float light = glm::dot(lightDir, normal);
            light = glm::clamp(light, 0.0f, 1.0f); 
            return getLight(light);
        }

        if (t > maxDist) break;
        t += dist;
    }

    return ' ';
}


void step() {
	util::setCursorPos(0, 0);
    fpsClock.tick();
}

void util::clear()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    SMALL_RECT scrollRect;
    COORD scrollTarget;
    CHAR_INFO fill;

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        return;
    }

    scrollRect.Left = 0;
    scrollRect.Top = 0;
    scrollRect.Right = csbi.dwSize.X;
    scrollRect.Bottom = csbi.dwSize.Y;

    scrollTarget.X = 0;
    scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);

    fill.Char.UnicodeChar = TEXT(' ');
    fill.Attributes = csbi.wAttributes;

    ScrollConsoleScreenBuffer(hConsole, &scrollRect, NULL, scrollTarget, &fill);

    csbi.dwCursorPosition.X = 0;
    csbi.dwCursorPosition.Y = 0;

    SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);
}

int util::bufferIndex(int x, int y)
{
	return y * width + x;
}

template<typename T>
void util::setCursorPos(T x, T y)
{
	COORD pos;
	pos.X = (short)x;
	pos.Y = (short)y;
	SetConsoleCursorPosition(hConsole, pos);
}
