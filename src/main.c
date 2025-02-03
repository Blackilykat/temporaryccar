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

	while(!WindowShouldClose()) {
		BeginDrawing();

		float deltaTime = GetFrameTime();
		secondsPassed += deltaTime;





		WallNode *cursor = map.firstWall;
		while(cursor != NULL) {
			DrawLine(cursor->wall.start.x, cursor->wall.start.y, cursor->wall.end.x, cursor->wall.end.y, BLUE);
			cursor = cursor->next;
		}

		ClearBackground(BLACK);

		DrawRectanglePro((Rectangle){500, 600, 100, 200}, (Vector2){50, 100}, secondsPassed * 10, RAYWHITE);

		EndDrawing();
	}

	CloseWindow();
}
