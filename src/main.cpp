//#define OLC_PGEX_DEAR_IMGUI_IMPLEMENTATION
//#include "../lib/imgui_impl_pge.h"
//#include "../lib/imgui/imgui_impl_opengl2.h"
//#define OLC_PGE_APPLICATION
//#include "../lib/olcPixelGameEngine.h"
//
#define OLC_PGEX_DEAR_IMGUI_IMPLEMENTATION
#define OLC_PGE_APPLICATION
#include "../inc/Engine.h"


int main() {
	Engine demo;
	if (demo.Construct(640, 360, 2, 2))
		demo.Start();
	return 0;
}
