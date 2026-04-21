#include <iostream>
#include "raylib.h"
using namespace std;

Color Hit = Color{ 224, 211, 173, 255 };
Color Blue = Color{ 27, 35, 48 , 255 };
Color Maroon = Color{ 64, 31, 54 , 255 };
Color Gray = Color{ 35, 35, 36 , 255 };

int player_score = 0;
int cpu_score = 0;

class Ball {
public:
	float x, y;        
	int speed_x, speed_y;
	short int radius;

	void Draw() {
		DrawCircle((int)x, (int)y, radius, Hit);
	}
	void Update()
	{
		x += speed_x;
		y += speed_y;

		if (y + radius >= GetScreenHeight() || y - radius <= 0)
		{
			speed_y *= -1;
		}
		if (x + radius >= GetScreenWidth() || x - radius <= 0)
		{
			speed_x *= -1;
			if (x - radius <= 0) {
				player_score++;
			} else {
				cpu_score++;
			}
		}
	}
};

class Paddle {
protected:
	void LimitMovement() {
		if (y <= 0)
		{
			y = 0;
		}
		if (y + height >= GetScreenHeight())
		{
			y = GetScreenHeight() - height;
		}
	}

public:
	float x, y;
	float width, height;
	int speed;

	void Draw() {
		DrawRectangleRounded(Rectangle{ x, y, width, height }, 0.8f, 4, WHITE); 
	}

	void Update() {
		if (IsKeyDown(KEY_UP))
		{
			y -= speed;
		}
		if (IsKeyDown(KEY_DOWN))
		{
			y += speed;
		}

		LimitMovement();
	}
};

class CpuPaddle : public Paddle {
public:
	void Update(float ball_y)  
	{
		if (y + height / 2 > ball_y)
		{
			y -= speed;
		}
		if (y + height / 2 < ball_y)
		{
			y += speed;
		}
		LimitMovement();
	}
};

Ball ball;
Paddle player;
CpuPaddle cpu;

int main() {
	const auto screen_width = 600;
	const auto screen_height = 400;
	InitWindow(screen_width, screen_height, "basic pong");
	SetTargetFPS(60);

	ball.radius = 15;
	ball.x = screen_width / 2;
	ball.y = screen_height / 2;
	ball.speed_x = 5;
	ball.speed_y = 5;

	player.width = 20;
	player.height = 90;
	player.speed = 5;
	player.x = screen_width - player.width - 5;
	player.y = (screen_height - player.height) / 2;

	cpu.height = 90;
	cpu.width = 20;
	cpu.x = 5;
	cpu.y = (screen_height - cpu.height) / 2;
	cpu.speed = 5;

	while (WindowShouldClose() == false) {
		BeginDrawing();

		ball.Update();
		ClearBackground(Maroon);
		ball.Draw();
		cpu.Update(ball.y);

		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ player.x, player.y, player.width, player.height }))
		{
			ball.speed_x *= -1;
		}

		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ cpu.x, cpu.y, cpu.width, cpu.height }))
		{
			ball.speed_x *= -1;
		}

		DrawLine((screen_width / 2) - 1, 0, (screen_width / 2) + 1, screen_height, WHITE);
		player.Draw();
		cpu.Draw();
		player.Update();
		DrawText(TextFormat("Player: %i", player_score), screen_width / 2 + 20, 10, 20, WHITE);
		DrawText(TextFormat("CPU: %i", cpu_score), screen_width / 2 - 80, 10, 20, WHITE);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
// I know that the computer is impossible to beat, I'm working on it.
