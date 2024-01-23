#pragma once
//Include raylib code , math and shapes
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
//Include C++ stuff
#include <iostream>
#include <cmath>
#include <list>




//Some defined stats for player/bullet/comet
#define PLAYER_BASE_SIZE    20.0f
#define PLAYER_SPEED        2.5f
#define BULLET_SPEED		4.0f
#define COMET_SPEED			3.0f
#define COMET_TIMER			1.2f

//Player
struct Triangle
{
	Vector2 position;
	Vector2 speed;
	float rotation;
	Vector3 collider;
	Color color;
};

//Comets, pretty much all the same. Changes are made in code
struct Comet
{
	Vector2 position;
	Vector2 speed;
	int life;
	float rotation;
	Vector3 collider;
	int points;
	bool active;
	Color color;
	int sides;
	float radius;
	bool correctPosition;
};

//Bullets
struct shot
{
	Vector2 position;
	Vector2 speed;
	Vector3 collider;
	float radius;
	bool active;
	int damage;
	Color color;
};

//Init game a.k.a. void Start(){}
void InitGame(void);
//Update function but for drawing
void DrawGame(void);
//Update function
void UpdateGame(void);
//All Together, supposedly 1 frame???
void DrawGameFrame(void);
//Template that accepts position of some objects and check if it's out of screen
template <typename T> void OutOfScreen(T obj);

//Function to return a better position for comets and also remove it from the main code.
Vector2 ReturnCorrectPosition(Vector2 playerPosition);

//Global variables
int screenWidth;
int screenHeight;
