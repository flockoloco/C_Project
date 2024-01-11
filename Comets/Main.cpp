#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include <cmath>


#define PLAYER_BASE_SIZE    20.0f

struct Triangle
{
	Vector2 position; 
	Vector2 speed;
	float acceleration;
	float rotation;
	Vector3 collider;
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
	player.position = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
	player.speed = {0,0};
	player.acceleration = 0;
	player.rotation = 0;
	player.collider = {0,0,0};
	player.color = WHITE;
	

	while (!WindowShouldClose())
	{

		float disX = (GetMouseX() - player.position.x );
		float disY = (GetMouseY() - player.position.y );

		player.rotation = atan2(disY, disX) + (90.0f * DEG2RAD);

		





		//Begins the drawing function
		BeginDrawing();
			//set background to black
			ClearBackground(BLACK);
			

			Vector2 fsPoint = { player.position.x + sinf(player.rotation) * (shipHeight), player.position.y - cosf(player.rotation) * (shipHeight) };
			Vector2 snPoint = { player.position.x - cosf(player.rotation) * (PLAYER_BASE_SIZE / 2) , player.position.y - sinf(player.rotation) * (PLAYER_BASE_SIZE / 2) };
			Vector2 tdPoint = { player.position.x + cosf (player.rotation) * (PLAYER_BASE_SIZE / 2) , player.position.y + sinf(player.rotation) * (PLAYER_BASE_SIZE / 2) };
			DrawText(TextFormat("%f", player.rotation), fsPoint.x, fsPoint.y, 20, LIGHTGRAY);
			//DrawPoly(player.fsPoint, 3, 20, angle, WHITE);
			DrawTriangle(fsPoint, snPoint, tdPoint, WHITE);

			//draw fps on the screen
			DrawFPS(10, 10);
		EndDrawing();

	}


	//Close Window so it does not get stuck in memory
	CloseWindow();

	return 0;
}