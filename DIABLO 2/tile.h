#pragma once
#include "stdafx.h"
#define TILEX 21
#define TILEY 21
#define TILEZ 6
#define TILESIZEX 192
#define TILESIZEY 96
#define TILESIZEZ 96
#define TILEMAXSIZEX TILEX * TILESIZEX
#define TILEMAXSIZEY TILEY * TILESIZEY
#define TILEMAXSIZEZ TILEZ * TILESIZEZ
#define INITX (22)
#define INITY (22 - TILEMAXSIZEY / 4)
#define TILEMONITORSIZEW 1167
#define TILEMONITORSIZEH 860

enum TERRAIN_TYPE
{
	TER_LOAD,
	TER_WALL,
	TER_VOID,
	TER_END
};
enum OBJECT_TYPE
{
	OBJ_ITEM,
	OBJ_ENEMY,
	OBJ_NPC,
	OBJ_HOUSERENDER,
	OBJ_HOUSE,
	OBJ_PLAYER,
	OBJ_ERASE,
	OBJ_END
};
struct tagIso
{
	RECT iso;
	int x, y, z;
	int floorZ;
	int centerX, centerY;
	int indexX, indexY, indexZ;
	POINT line[4];
	TERRAIN_TYPE ter;
	OBJECT_TYPE obj;
	POINT terFrame;
	POINT objFrame;
	bool tileVoid;
	bool edgePaint;
	bool clickCheck;
};
