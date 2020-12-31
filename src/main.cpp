#define OLC_PGEX_DEAR_IMGUI_IMPLEMENTATION
#define OLC_PGE_APPLICATION
#include "../inc/Engine.h"

int main() {
    galg::Engine demo;
	if (demo.Construct(2*640, 2*360, 1, 1))
		demo.Start();
	return 0;
}
