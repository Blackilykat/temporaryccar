#include <stdlib.h>

#include "geometry.c"

typedef struct Wall {
	Position start;
	Position end;
} Wall;

typedef struct WallNode {
	Wall wall;
	struct WallNode *prev;
	struct WallNode *next;
} WallNode;

typedef struct RaceMap {
	Position start;
	WallNode *firstWall;
	WallNode *lastWall;
} RaceMap;


void map_add_wall(RaceMap *map, Wall *wall) {
	WallNode *node = malloc(sizeof(WallNode));
	node->wall = *wall;
	if(map->lastWall != NULL) {
		map->lastWall->next = node;
		node->prev = map->lastWall; 
		node->next = NULL;
		map->lastWall = node;
	} else {
		map->firstWall = node;
		map->lastWall = node;
		node->prev = NULL;
		node->next = NULL;
	}
}

