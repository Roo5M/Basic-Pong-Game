#include <raylib.h>
#include <vector>
#include <fstream>

Vector2 SnakeFoodSafe(int screenWidth, int screenHeight, int cellsize, float playerposx, float playerposy, std::vector<Vector2>& tail) {
	Vector2 newFoodPos;
	bool validFoodPosition = false;

	while (!validFoodPosition) {
		newFoodPos = {
			static_cast<float>(GetRandomValue(0, (screenWidth / cellsize) - 1) * cellsize),
			static_cast<float>(GetRandomValue(0, (screenHeight / cellsize) - 1) * cellsize)
		};
		validFoodPosition = true;

		if (newFoodPos.x == playerposx && newFoodPos.y == playerposy) {
			validFoodPosition = false;
			continue;
		}

		for (const Vector2& segment : tail) {
			if (newFoodPos.x == segment.x && newFoodPos.y == segment.y) {
				validFoodPosition = false;
				break;
			}
		}
	}
	return newFoodPos;
}

int main() {
	const int screenWidth = 600;
	const int screenHeight = 600;
	const int cellsize = 20;
	const float DelayTime = 0.06f;
	
	InitWindow(screenWidth, screenHeight, "Snake");
	InitAudioDevice();

	float playerposx = 300;
	float playerposy = 300;
	int foodScore = 0;
	int HighScore = 0;

	std::ifstream inFile("highscore.txt");
	if (inFile.is_open()) {
		inFile >> HighScore;
		inFile.close();
	}

	float dirx = 0;
	float diry = 0;

	std::vector<Vector2> tail;
	int snakeLength = 0;

	SetTargetFPS(60);
	float timer = 0.0f;

	Sound bgsound = LoadSound("sounds/snakefood.mp3");
	Sound bgmusic = LoadSound("sounds/TheSoundlings.mp3");

	PlaySound(bgmusic);

	SetSoundVolume(bgmusic, 0.5f);
	SetSoundVolume(bgsound, 0.6f);

	Vector2 foodPos = {
		GetRandomValue(0, (screenWidth / cellsize) - 1) * cellsize,
		GetRandomValue(0, (screenHeight / cellsize) - 1) * cellsize
	};

	while (!WindowShouldClose()) {
		float delta = GetFrameTime();
		timer += delta;
		BeginDrawing();
		ClearBackground(GRAY);

		DrawText(TextFormat("Score: %d", foodScore), 10, 10, 20, PURPLE);
		DrawText(TextFormat("Highest Score: %d", HighScore), 120, 10, 20, YELLOW);

		if (playerposx == foodPos.x && playerposy == foodPos.y) {
			snakeLength++;
			foodScore++;
			PlaySound(bgsound);

			foodPos = SnakeFoodSafe(screenWidth, screenHeight, cellsize, playerposx, playerposy, tail);
		}

		if (IsKeyPressed(KEY_W) && diry != 1) Vector2{dirx = 0, diry = -1};
		if (IsKeyPressed(KEY_S) && diry != -1) Vector2{ dirx = 0, diry = 1 };
		if (IsKeyPressed(KEY_A) && dirx != 1) Vector2{dirx = -1, diry = 0};
		if (IsKeyPressed(KEY_D) && dirx != -1) Vector2{ dirx = 1, diry = 0 };

		if (timer > DelayTime) {
			tail.insert(tail.begin(), Vector2{ playerposx, playerposy });

			if (tail.size() > snakeLength) {
				tail.pop_back();
			}


			playerposx += dirx * cellsize;
			playerposy += diry * cellsize;
			timer = 0.0f;
		}

		if (playerposx < 0 || playerposx >= screenWidth || playerposy < 0 || playerposy >= screenHeight) {
			DrawText("Game Over", 200, 200, 40, RED);
			EndDrawing();
			WaitTime(2.0);

			std::ofstream outFile("highscore.txt");
			if (outFile.is_open() && foodScore > HighScore) {
				outFile << foodScore;
				HighScore = foodScore;
				outFile.close();
			}

			CloseWindow();
			return 0;
		}

		for (const Vector2& segment : tail) {
			DrawRectangleV({ segment.x - 1, segment.y - 1}, Vector2{ cellsize + 1.8, cellsize + 1.8}, BLACK);

			DrawRectangleV(segment, Vector2{ cellsize, cellsize }, GREEN);

			if (playerposx == segment.x && playerposy == segment.y) {
				DrawText("Game Over", 200, 200, 40, RED);
				StopSound(bgmusic);
				EndDrawing();
				WaitTime(2.0);

				std::ofstream outFile("highscore.txt");
				if (outFile.is_open() && foodScore > HighScore) {
					outFile << foodScore;
					HighScore = foodScore;
					outFile.close();
				}
				CloseWindow();
				return 0;
			}
		}

		DrawRectangleV(foodPos, Vector2{ cellsize, cellsize }, RED);

		DrawRectangle(playerposx - 1, playerposy - 1, cellsize + 2.0, cellsize + 2.0, WHITE);
		DrawRectangle(playerposx, playerposy, cellsize, cellsize, BLACK);

		EndDrawing();
	}
	CloseAudioDevice();
	CloseWindow();

	return 0;
}