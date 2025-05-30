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
	
	// weird slightly offset numbers are necessary for all walls to be visible
	Wall northwall = {{1, 0}, {WINDOW_WIDTH, 0}};
	map_add_wall(&map, northwall);

	Wall eastwall = {{WINDOW_WIDTH, 0}, {WINDOW_WIDTH, WINDOW_HEIGHT}};
	map_add_wall(&map, eastwall);
	
	Wall southwall = {{WINDOW_WIDTH, WINDOW_HEIGHT-1}, {1, WINDOW_HEIGHT-1}};
	map_add_wall(&map, southwall);

	Wall westwall = {{1, WINDOW_HEIGHT}, {1, 0}};
	map_add_wall(&map, westwall);

	char placingWall = false;
	Position placingWallStart = {};
	Position placingWallEnd = {};

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

		if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			placingWall = true;
			placingWallStart.x = GetMouseX();
			placingWallStart.y = GetMouseY();
		}
		if(placingWall) {
			placingWallEnd.x = GetMouseX();
			placingWallEnd.y = GetMouseY();
		}
		if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
			placingWall = false;
			Wall wall = {placingWallStart, placingWallEnd};
			map_add_wall(&map, wall);
		}

		ClearBackground(BLACK);

		paint_car(&car);
		paint_walls(&map);
		if(placingWall) {
			DrawLine(placingWallStart.x, placingWallStart.y, placingWallEnd.x, placingWallEnd.y, GREEN);
		}

		EndDrawing();
	}

	WallNode *cursor = map.firstWall;
	while(cursor != NULL) {
		WallNode *prev = cursor;
		cursor = cursor->next;
		free(prev);
	}



	CloseWindow();
}
