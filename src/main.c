#include <stdio.h>

#include "raylib.h"

#include "map.c"
#include "player.c"

#define WINDOW_HEIGHT 1000
#define WINDOW_WIDTH 1000
#define TARGET_FPS 90

int main() {
	printf("Hello, world!\n");
	InitWindow(WINDOW_HEIGHT, WINDOW_WIDTH, "Car");
	SetTargetFPS(TARGET_FPS);
	
	float secondsPassed = 0.0;

	RaceMap map = {{0, 0}, NULL, NULL};
	map_add_wall(&map, &(Wall){{100, 100}, {200, 200}});
	map_add_wall(&map, &(Wall){{200, 200}, {400, 200}});
	map_add_wall(&map, &(Wall){{400, 200}, {400, 300}});

	Car car = {{500, 500}, 0.0, 45, RED};

	while(!WindowShouldClose()) {
		BeginDrawing();

		float deltaTime = GetFrameTime();
		secondsPassed += deltaTime;


		tick_player(&car, (Input){
				IsKeyDown(KEY_W),
				IsKeyDown(KEY_S),
				IsKeyDown(KEY_A),
				IsKeyDown(KEY_D),
			}, deltaTime, &map);
		tick_car(&car, deltaTime, &map);
		


		ClearBackground(BLACK);

		paint_car(&car);
		paint_walls(&map);

		EndDrawing();
	}

	CloseWindow();
}
