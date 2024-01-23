#include "Main.h"

using namespace std;

bool gameOver;
int points;
static float shipHeight;
float cometTimerSpawn;

//player
Triangle player;
//bullets default example and list
shot bullet;
list<shot> bullets;
//Comet default example and list
Comet comet;
list<Comet> comets;

int main()
{
	InitGame();

	while (!WindowShouldClose())
	{
		DrawGameFrame();
	}

	//Close Window so it does not get stuck in memory
	CloseWindow();

	return 0;
}

void InitGame(void)
{
	//Global variables
	screenWidth = 800;
	screenHeight = 600;
	//Local variables
	gameOver = false;
	points = 0;
	// NOTE: Defined triangle is isosceles with common angles of 70 degrees.
	shipHeight = (PLAYER_BASE_SIZE / 2) / tanf(20 * DEG2RAD);
	cometTimerSpawn = COMET_TIMER;

	//set windows size
	InitWindow(screenWidth, screenHeight, "Game");

	//Adjust fps to match monitor
	SetWindowState(FLAG_VSYNC_HINT);

	//single player
	player.position = { screenWidth / 2.0f, screenHeight / 2.0f };
	player.speed = { 0,0 };
	player.rotation = 0.0f;
	player.collider = { 0,0,0 };
	player.color = RED;

	//bullets
	bullet.position = { 0, 0 };
	bullet.damage = 1;
	bullet.radius = 3;
	bullet.speed = { 1, 1 };
	bullet.collider = { 0, 0, 0 };
	bullet.color = RED;
	bullet.active = false;

	//All comets start big, after being shot its size is halved and quantity doubled
	comet.active = false;
	comet.collider = { 0, 0, 0 };
	comet.color = WHITE;
	comet.life = 3;
	comet.points = 1;
	comet.position = { 0, 0 };
	comet.rotation = 0.0f;
	comet.speed = { 0.2f, 0.2f };
	comet.sides = 7;
	comet.radius = 20;
	comet.correctPosition = false;


}

void DrawGame(void) {
	//Begins the drawing function
	BeginDrawing();
	//set background to black
	ClearBackground(BLACK);

	//Create the 3 points of the triangle (Matrix Multiplication)
	Vector2 fsPoint = { player.position.x + sinf(player.rotation) * (shipHeight), player.position.y - cosf(player.rotation) * (shipHeight) };
	Vector2 snPoint = { player.position.x - cosf(player.rotation) * (PLAYER_BASE_SIZE / 2) , player.position.y - sinf(player.rotation) * (PLAYER_BASE_SIZE / 2) };
	Vector2 tdPoint = { player.position.x + cosf(player.rotation) * (PLAYER_BASE_SIZE / 2) , player.position.y + sinf(player.rotation) * (PLAYER_BASE_SIZE / 2) };
	//Draw Player
	DrawTriangle(fsPoint, snPoint, tdPoint, player.color);
	//Draw points
	DrawText(TextFormat("Points: %i", points), screenWidth - 150, 20, 20, GRAY);

	//See player's collision
	//DrawCircle( (int) player.collider.x, (int) player.collider.y, player.collider.z, BLUE);
	//DrawText(TextFormat("%f", player.rotation), (int) fsPoint.x, (int) fsPoint.y, 20, LIGHTGRAY);

	//Draw bullet
	for (list<shot>::iterator i = bullets.begin(); i != bullets.end(); i++)
	{
		if (i->active == true)
		{
			DrawCircle((int)i->position.x, (int)i->position.y, i->radius, i->color);
			//Check Collider position
			//DrawCircle((int) i->collider.x, (int) i->collider.y , i->collider.z, BLUE);
		}
	}

	//Draw meteors
	for (list<Comet>::iterator i = comets.begin(); i != comets.end(); i++)
	{
		if (i->active == true)
		{
			DrawPoly({ i->position.x, i->position.y }, i->sides, i->radius, 0, i->color);
			//Check collider position
			//DrawCircle(i->collider.x, i->collider.y, i->collider.z, BLUE);
			//Check direction
			//DrawLine(i->position.x, i->position.y, i->position.x + i->speed.x * 100, i->position.y + i->speed.y * 100, GREEN);
		}
	}

	//If gameover ,gameover text
	if (gameOver)
	{
		DrawText("Game over", screenWidth / 2, screenHeight / 2, 20, LIGHTGRAY);
	}

	//draw fps on the screen
	DrawFPS(10, 10);
	EndDrawing();
}

void UpdateGame(void)
{
	
	if (!gameOver)
	{
		//Distance of X and Y 
		float disX = (GetMouseX() - player.position.x);
		float disY = (GetMouseY() - player.position.y);
		//Direction of mouse in radians + offset 
		player.rotation = atan2(disY, disX) + (90.0f * DEG2RAD);

		//Movement Player 
		player.speed.x = (float)IsKeyDown(KEY_D) - (float)IsKeyDown(KEY_A);
		player.speed.y = (float)IsKeyDown(KEY_W) - (float)IsKeyDown(KEY_S);
		player.position.x += (PLAYER_SPEED * player.speed.x);
		player.position.y -= (PLAYER_SPEED * player.speed.y);

		//wall collision logic	
		if (player.position.x > screenWidth + shipHeight) player.position.x = -shipHeight;
		else if (player.position.x < -shipHeight) player.position.x = screenWidth - shipHeight;
		if (player.position.y > screenHeight + shipHeight) player.position.y = -shipHeight;
		else if (player.position.y < -shipHeight) player.position.y = screenHeight - shipHeight;

		//Shooting 
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		{
			auto temp = bullet;
			temp.active = true;
			temp.position.x = player.position.x + sinf(player.rotation) * (shipHeight + temp.radius);
			temp.position.y = player.position.y - cosf(player.rotation) * (shipHeight + temp.radius);
			temp.speed.x = temp.speed.x * sinf(player.rotation) * BULLET_SPEED;
			temp.speed.y = temp.speed.y * -cos(player.rotation) * BULLET_SPEED;
			temp.collider = Vector3{ temp.position.x, temp.position.y, temp.radius };
			bullets.push_back(temp);
		}

		//Comets spawn
		cometTimerSpawn -= GetFrameTime();
		if (cometTimerSpawn <= 0)
		{
			cometTimerSpawn = COMET_TIMER;

			auto temp = comet;

			temp.position = ReturnCorrectPosition(player.position);
			temp.active = true;
			temp.rotation = atan2((screenHeight / 2) - temp.position.y, (screenWidth / 2) - temp.position.x);
			temp.speed.x = (temp.speed.x * cosf(temp.rotation)) * COMET_SPEED;
			temp.speed.y = (temp.speed.y * sinf(temp.rotation)) * COMET_SPEED;
			comets.push_back(temp);
		}

		player.collider = Vector3{ (player.position.x + sinf(player.rotation) * shipHeight / 2.5f), (player.position.y - cosf(player.rotation) * shipHeight / 2.5f), PLAYER_BASE_SIZE / 2 };

		//Comets movement
		for (list<Comet>::iterator i = comets.begin(); i != comets.end(); i++)
		{
			if (i->active == true)
			{
				i->position.x += i->speed.x;
				i->position.y += i->speed.y;
				i->collider = Vector3{ i->position.x, i->position.y, i->radius };

				if (i->position.x >= (screenWidth - i->radius / 2) && i->life < 3) i->speed.x = -i->speed.x;
				else if (i->position.x <= (i->radius / 2) && i->life < 3) i->speed.x = i->speed.x * -1;
				if (i->position.y >= (screenHeight - i->radius / 2) && i->life < 3) i->speed.y = -i->speed.y;
				else if (i->position.y <= (i->radius / 2) && i->life < 3) i->speed.y = i->speed.y * -1;
			}
			if (i->active == true)
			{
				OutOfScreen(i);
			}
			//Collision detection
			if (CheckCollisionCircles(Vector2{ player.collider.x, player.collider.y }, player.collider.z, i->position, i->radius) && i->active)
			{
				gameOver = true;
			}
		}

		//Shots movement and check if its out of screen
		for (list<shot>::iterator i = bullets.begin(); i != bullets.end(); i++)
		{
			if (i->active == true)
			{
				i->position.x += i->speed.x;
				i->position.y += i->speed.y;
				i->collider = Vector3{ i->position.x, i->position.y, i->radius };
			}
			if (i->active == true)
			{
				OutOfScreen(i);
			}
			for (list<Comet>::iterator ii = comets.begin(); ii != comets.end(); ii++)
			{
				if (ii->active && i->active && CheckCollisionCircles(Vector2{ i->collider.x, i->collider.y }, i->collider.z, Vector2{ ii->collider.x, ii->collider.y }, ii->collider.z))
				{
					ii->active = false;
					i->active = false;
					points += ii->points;
					//Spawn 2 smaller orbs from this one that just died
					if (ii->life > 1) {
						for (int a = 1; a <= 2; a++)
						{
							Comet temp = *ii;
							temp.active = true;

							if (a % 2 == 0)
							{
								temp.position = Vector2{ temp.position.x + temp.radius , temp.position.y };
								temp.rotation += 90.0f * RAD2DEG;
								//temp.speed.x = (temp.speed.x * cosf(temp.rotation) + temp.speed.y * sinf(temp.rotation)) * COMET_SPEED;
								//temp.speed.y = (temp.speed.x * -sinf(temp.rotation) + temp.speed.y * cosf(temp.rotation)) * COMET_SPEED;
							}
							else {
								temp.position = Vector2{ temp.position.x - temp.radius , temp.position.y };
								temp.rotation -= 90.0f * RAD2DEG;
								//temp.speed.x = (temp.speed.x * cosf(temp.rotation) + temp.speed.y * -sinf(temp.rotation)) * COMET_SPEED;
								//temp.speed.y = (temp.speed.x * cosf(temp.rotation) + temp.speed.y * sinf(temp.rotation)) * COMET_SPEED;
							}
							temp.speed.x = (temp.speed.x * cosf(temp.rotation)) * COMET_SPEED * 1.2f;
							temp.speed.y = (temp.speed.y * sinf(temp.rotation)) * COMET_SPEED * 1.2f;
							temp.life -= 1;
							temp.points += 2;
							temp.sides /= 1.3f;
							temp.radius /= 1.3f;
							comets.push_back(temp);
						}
					}
				}
			}
		}
	}
}

void DrawGameFrame(void)
{
	UpdateGame();
	DrawGame();
}

template <typename T>
void OutOfScreen(T obj)
{
	if (obj->position.x > screenWidth)
	{
		obj->active = false;
	}
	else if (obj->position.x < 0)
	{
		obj->active = false;
	}

	if (obj->position.y > screenHeight)
	{
		obj->active = false;
	}
	else if (obj->position.y < 0)
	{
		obj->active = false;
	}
}

Vector2 ReturnCorrectPosition(Vector2 playerPosition)
{
	bool correctPosition = false;
	Vector2 position = { (float)GetRandomValue(0, screenWidth), (float)GetRandomValue(0, screenHeight) };
	while (!correctPosition)
	{
		if (position.x < (screenWidth / 2.0f) + 100 && position.x > (screenWidth / 2.0f) - 100) 
		{
			position.x = (float)GetRandomValue(0, screenWidth);
		}
		else if (position.x > playerPosition.x - 25 && position.x < playerPosition.x + 25)
		{
			position.x = (float)GetRandomValue(0, screenWidth);
		}
		else correctPosition = true;
	}
	correctPosition = false;
	while (!correctPosition)
	{
		if (position.y < (screenHeight / 2.0f) + 100 && position.y > (screenHeight / 2.0f) - 100) 
		{
			position.y = (float)GetRandomValue(0, screenHeight);
		}
		else if (position.y > (playerPosition.y - 25) && position.y < (playerPosition.y + 25))
		{
			position.y = (float)GetRandomValue(0, screenHeight);
		}
		else correctPosition = true;
	}

	return position;
}

