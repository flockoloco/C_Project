#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include <iostream>
#include <cmath>
#include <list>

using namespace std;

#define PLAYER_BASE_SIZE    20.0f
#define PLAYER_SPEED        2.5f

struct Triangle
{
	Vector2 position;
	Vector2 speed;
	float rotation;
	Vector3 collider;
	Color color;
};

struct Comet
{
	Vector2 position;
	Vector2 speed;
	int life;
	float rotation;
	Vector3 collider;
	Color color;
};

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

// NOTE: Defined triangle is isosceles with common angles of 70 degrees.
static float shipHeight = 0.0f;

bool mousePressed = false;

int main()
{
	//Screen size variables (in case I need to change in game)
	int screenWidth = 800;
	int screenHeight = 600;
	//set windows size
	InitWindow(screenWidth, screenHeight, "Game");
	//Adjust fps to match monitor
	SetWindowState(FLAG_VSYNC_HINT);

	shipHeight = (PLAYER_BASE_SIZE / 2) / tanf(20 * DEG2RAD);


	//Player init
	Triangle player;
	player.position = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	player.speed = { 0,0 };
	player.rotation = 0;
	player.collider = { 0,0,0 };
	player.color = WHITE;
	
	shot bullet;
	bullet.position = {0, 0};
	bullet.damage = 1;
	bullet.radius = 3;
	bullet.speed = {0, 0};
	bullet.collider = {0, 0, 0};
	bullet.color = WHITE;
	bullet.active = false;

	list<shot> bullets{0};

	while (!WindowShouldClose())
	{
		//Distance of X and Y 
		float disX = (GetMouseX() - player.position.x);
		float disY = (GetMouseY() - player.position.y);
		//Direction of mouse in radians + offset 
		player.rotation = atan2(disY, disX) + (90.0f * DEG2RAD);

		//Movement Player 
		player.speed.x = (int) IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
		player.speed.y = (int) IsKeyDown(KEY_W) - IsKeyDown(KEY_S);
		player.position.x += (PLAYER_SPEED * player.speed.x);
		player.position.y -= (PLAYER_SPEED * player.speed.y);


		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		{

		}













		//Begins the drawing function
		BeginDrawing();
		//set background to black
		ClearBackground(BLACK);

		//Create the 3 points of the triangle (Matrix Multiplication)
		Vector2 fsPoint = { player.position.x + sinf(player.rotation) * (shipHeight), player.position.y - cosf(player.rotation) * (shipHeight) };
		Vector2 snPoint = { player.position.x - cosf(player.rotation) * (PLAYER_BASE_SIZE / 2) , player.position.y - sinf(player.rotation) * (PLAYER_BASE_SIZE / 2) };
		Vector2 tdPoint = { player.position.x + cosf(player.rotation) * (PLAYER_BASE_SIZE / 2) , player.position.y + sinf(player.rotation) * (PLAYER_BASE_SIZE / 2) };		
		//Draw Player
		DrawTriangle(fsPoint, snPoint, tdPoint, WHITE);

		//Draw Comets
		DrawPoly({ 100,100 }, 7, 10, 0, WHITE);

		//draw fps on the screen
		DrawFPS(10, 10);
		EndDrawing();

	}


	//Close Window so it does not get stuck in memory
	CloseWindow();

	return 0;
}