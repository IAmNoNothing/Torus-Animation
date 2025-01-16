#include "graphics.h"

int main()
{
    init();
    while (true) {
        step();
        draw();
    }
}
