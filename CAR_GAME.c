#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <raylib.h>
#include <ctype.h>
#include <string.h>

char *box(int, int, bool *check);

const int WIDTH = 480;
const int HEIGHT = 640;

typedef struct Sprite
{
	Texture2D tex;
	Vector2 pos;
} Sprite;

int main()
{
	// others
	srand(time(NULL));
	SetTargetFPS(60);
	SetExitKey(KEY_NULL);
	bool check = false;
	int num;
	FILE *ptr;
	ptr = fopen("C:\\Users\\HP\\Desktop\\PROJECTS\\FOP\\high.txt", "r");
	fscanf(ptr, "%d", &num);
	fclose(ptr);
	// window
	InitWindow(WIDTH, HEIGHT, "CAR GAME");
	Image icon = LoadImage("C:\\Users\\HP\\Desktop\\PROJECTS\\FOP\\logo.png");
	SetWindowIcon(icon);
	// calling functions
	char *n = box(WIDTH, HEIGHT, &check);
	if (check == false)
	{
		exit(0);
	}
	// Audio
	InitAudioDevice();
	Music music = LoadMusicStream("C:\\Users\\HP\\Desktop\\PROJECTS\\FOP\\music.mp3");
	PlayMusicStream(music);
	// creating the player
	Sprite car;
	car.tex = LoadTexture("C:\\Users\\HP\\Desktop\\PROJECTS\\FOP\\car.png");
	car.pos.x = WIDTH / 2 - car.tex.width / 2;
	car.pos.y = HEIGHT * 3 / 5;
	int car_speed = 2;
	int dx = 4;
	// creating trees
	Texture2D ttrees = LoadTexture("C:\\Users\\HP\\Desktop\\PROJECTS\\FOP\\trees.png");
	Rectangle srect[3];
	for (int i = 0; i < sizeof(srect) / sizeof(srect[0]); i++)
	{
		srect[i].width = srect[i].height = 48;
		srect[i].x = i * srect[i].width;
		srect[i].y = 0;
	}
	// randomizing positions of trees
	int trees_num = rand() % 18 + 10;
	Vector2 trees_pos[trees_num];
	for (int i = 0; i < trees_num; i++)
	{
		if (i < (int)trees_num / 2)
		{
			trees_pos[i].x = rand() % WIDTH / 3 + 1;

			if (trees_pos[i].x + ttrees.width / 3 > WIDTH / 3)
			{
				trees_pos[i].x -= ttrees.width / 3;
			}
		}
		else
		{
			trees_pos[i].x = rand() % WIDTH / 3 + 2 * WIDTH / 3;

			if (trees_pos[i].x + ttrees.width / 3 > WIDTH)
			{
				trees_pos[i].x -= ttrees.width / 3;
			}
		}

		trees_pos[i].y = rand() % HEIGHT;
		if (trees_pos[i].y + ttrees.height > HEIGHT)
		{
			trees_pos[i].y -= ttrees.height;
		}
	}
	// creating cars, randomizing positions of cars
	Texture2D tcars = LoadTexture("C:\\Users\\HP\\Desktop\\PROJECTS\\FOP\\cars.png");
	Rectangle srect_cars[5];
	Vector2 cars_pos[5];
	float cars_speed[5] = {0};
	for (int i = 0; i < sizeof(srect_cars) / sizeof(srect_cars[0]); i++)
	{
		srect_cars[i].width = 16;
		srect_cars[i].height = 24;
		srect_cars[i].x = i * srect_cars[i].width;
		srect_cars[i].y = 0;

		int randnum = rand() % 3 + 1;
		if (randnum == 1)
		{
			cars_pos[i].x = WIDTH / 3 + WIDTH / 18 - tcars.width / 12;
		}
		else if (randnum == 2)
		{
			cars_pos[i].x = WIDTH / 2 - tcars.width / 12;
		}
		else
		{
			cars_pos[i].x = WIDTH * 2 / 3 - WIDTH / 18 - tcars.width / 12;
		}

		cars_pos[i].y = rand() % HEIGHT;
		if (cars_pos[i].y + tcars.width / 12 > HEIGHT)
		{
			cars_pos[i].y -= tcars.width / 12;
		}

		cars_speed[i] = 7;
	}
	// dark screen
	Rectangle rmuteScreen = {0, 0, WIDTH, HEIGHT};
	int lives = 5;
	int score = 0;
	double time = 0;
	double vulnerable_time = 0;
	bool gameOver = false;
	bool gameWon = false;
	bool vulnerable = true;
	bool again = true;
	bool esc = false;
	bool pause = false;
	while (again)
	{
		while (!WindowShouldClose())
		{
			UpdateMusicStream(music);
			if (IsKeyPressed(KEY_SPACE))
			{
				pause = !pause;
			}
			if (IsKeyPressed(KEY_ESCAPE))
			{
				esc = !esc;
			}
			if (pause || esc)
			{
				PauseMusicStream(music);
			}
			else
			{
				ResumeMusicStream(music);
			}
			if (!gameOver && !pause && !esc)
			{
				// process events
				if (IsKeyDown(KEY_LEFT) && (car.pos.x > (WIDTH / 3 + 4)))
				{
					car.pos.x -= car_speed;
				}
				else if (IsKeyDown(KEY_RIGHT) && (car.pos.x < (WIDTH * 2 / 3 - (srect_cars[0].width) - 4)))
				{
					car.pos.x += car_speed;
				}
				// update
				for (int i = 0; i < trees_num; i++)
				{
					trees_pos[i].y += car_speed;

					if (trees_pos[i].y > HEIGHT)
					{
						trees_pos[i].y = -ttrees.height;
					}
				}
				for (int i = 0; i < sizeof(cars_pos) / sizeof(cars_pos[0]); i++)
				{
					cars_pos[i].y += cars_speed[i];

					if (cars_pos[i].y > HEIGHT)
					{
						cars_pos[i].y = -tcars.height;

						int randnum = rand() % 3 + 1;
						if (randnum == 1)
						{
							cars_pos[i].x = WIDTH / 3 + WIDTH / 18 - tcars.width / 12;
						}
						else if (randnum == 2)
						{
							cars_pos[i].x = WIDTH / 2 - tcars.width / 12;
						}
						else
						{
							cars_pos[i].x = WIDTH * 2 / 3 - WIDTH / 18 - tcars.width / 12;
						}

						cars_speed[i] = 7;
					}
					Rectangle rec1 = {car.pos.x, car.pos.y, car.tex.width, car.tex.height};
					Rectangle rec2 = {cars_pos[i].x, cars_pos[i].y, tcars.width / 6, tcars.height};
					if (CheckCollisionRecs(rec1, rec2))
					{
						if (vulnerable)
						{
							lives -= 1;
							vulnerable = false;
						}
					}
				}
				if (!vulnerable)
				{
					vulnerable_time += GetFrameTime();
					if (vulnerable_time > 1)
					{
						vulnerable = true;
						vulnerable_time = 0;
					}
				}
				if (lives == 0)
				{
					gameOver = true;
				}
				if (car.pos.x < WIDTH / 3 || car.pos.x + car.tex.width > WIDTH * 2 / 3)
				{
					car_speed = dx / 2;
				}
				else
				{
					car_speed = dx;
				}
				time += GetFrameTime();
				if (time > 1)
				{
					score++;
					time = 0;
				}
				if (score > 999)
				{
					gameWon = true;
					gameOver = true;
				}
			}
			// draw
			BeginDrawing();
			ClearBackground(WHITE);
			// draw background
			DrawRectangle(0, 0, WIDTH / 3, HEIGHT, GREEN);
			DrawRectangle(WIDTH * 2 / 3, 0, WIDTH / 3, HEIGHT, GREEN);
			DrawRectangle(WIDTH / 3, 0, WIDTH / 3, HEIGHT, GRAY);
			DrawRectangle(WIDTH / 3 + WIDTH / 9 - 1, 0, 2, HEIGHT, BLACK);
			DrawRectangle(WIDTH / 3 + WIDTH * 2 / 9 - 1, 0, 2, HEIGHT, BLACK);
			if (!vulnerable)
			{
				Color col = {0, 0, 0, 100};
				DrawTexture(car.tex, car.pos.x, car.pos.y, col);
			}
			else
			{
				DrawTexture(car.tex, car.pos.x, car.pos.y, WHITE);
			}

			for (int i = 0; i < sizeof(srect_cars) / sizeof(srect_cars[0]); i++)
			{
				DrawTextureRec(tcars, srect_cars[i], cars_pos[i], WHITE);
			}

			for (int i = 0; i < trees_num; i++)
			{
				if (i % 2 == 0)
				{
					DrawTextureRec(ttrees, srect[0], trees_pos[i], WHITE);
				}
				else if (i % 3 == 0)
				{
					DrawTextureRec(ttrees, srect[1], trees_pos[i], WHITE);
				}
				else
				{
					DrawTextureRec(ttrees, srect[2], trees_pos[i], WHITE);
				}
			}
			char scoring[12] = {" "};
			sprintf(scoring, "HIGH SCORE:%d", num);
			DrawText(scoring, 11, 5, 30, WHITE);
			sprintf(scoring, "LIVES:%d", lives);
			DrawText(scoring, WIDTH - 125, 5, 30, WHITE);
			sprintf(scoring, "SCORE:%d", score);
			DrawText(scoring, 11, 35, 30, WHITE);
			if (gameOver)
			{
				pause = false;
				Color color = {0, 0, 0, 180};
				DrawRectangle(rmuteScreen.x, rmuteScreen.y, rmuteScreen.width, rmuteScreen.height, color);
				if (gameWon)
				{
					if (score > num)
					{
						DrawText("NEW HIGH SCORE!", WIDTH / 11 - 25, HEIGHT / 3 - 140, 50, WHITE);
						ptr = fopen("C:\\Users\\HP\\Desktop\\PROJECTS\\FOP\\high.txt", "w");
						fprintf(ptr, "%d", score);
						fclose(ptr);
					}
					DrawText(n, WIDTH / 11 - 25, HEIGHT / 3, 50, WHITE);
					DrawText("YOU WON!", WIDTH / 11 - 25, HEIGHT / 3 + 70, 50, WHITE);
					DrawText("AGAIN(Y/N)?", WIDTH / 11 - 25, HEIGHT / 3 + 140, 50, WHITE);
					if (IsKeyDown(KEY_N))
					{
						exit(0);
					}
					if (IsKeyDown(KEY_Y))
					{
						system("CAR_GAME");
					}
				}
				else
				{
					if (score > num)
					{
						DrawText("NEW HIGH SCORE!", WIDTH / 11 - 25, HEIGHT / 3 - 140, 50, WHITE);
						ptr = fopen("C:\\Users\\HP\\Desktop\\PROJECTS\\FOP\\high.txt", "w");
						fprintf(ptr, "%d", score);
						fclose(ptr);
					}
					DrawText(n, WIDTH / 11 - 25, HEIGHT / 3, 50, WHITE);
					DrawText("YOU LOSE!", WIDTH / 11 - 25, HEIGHT / 3 + 70, 50, WHITE);
					DrawText("AGAIN(Y/N)?", WIDTH / 11 - 25, HEIGHT / 3 + 140, 50, WHITE);
					if (IsKeyDown(KEY_N))
					{
						exit(0);
					}
					if (IsKeyDown(KEY_Y))
					{
						system("CAR_GAME");
					}
				}
			}
			if (pause)
			{
				Color color = {0, 0, 0, 180};
				DrawRectangle(rmuteScreen.x, rmuteScreen.y, rmuteScreen.width, rmuteScreen.height, color);
				DrawText("PAUSED!", WIDTH / 11 + 50, HEIGHT / 3, 70, WHITE);
			}
			if (esc)
			{
				Color color = {0, 0, 0, 180};
				DrawRectangle(rmuteScreen.x, rmuteScreen.y, rmuteScreen.width, rmuteScreen.height, color);
				DrawText("QUIT?(Y/N)?", WIDTH / 11 - 25, HEIGHT / 3 + 140, 50, WHITE);
				if (IsKeyDown(KEY_Y))
				{
					if (score > num)
					{
						ptr = fopen("C:\\Users\\HP\\Desktop\\PROJECTS\\FOP\\high.txt", "w");
						fprintf(ptr, "%d", score);
						fclose(ptr);
					}
					exit(0);
				}
				if (IsKeyDown(KEY_N))
				{
					esc = !esc;
				}
			}
			EndDrawing();
		}
		UnloadTexture(tcars);
		UnloadTexture(ttrees);
		UnloadTexture(car.tex);
		UnloadMusicStream(music);
		CloseAudioDevice();
		CloseWindow();
		return 0;
	}
}
char *box(int screenWidth, int screenHeight, bool *check)
{
	SetExitKey(KEY_NULL);
	static char name[8 + 1] = "\0";
	int letterCount = 0;
	Rectangle textBox = {screenWidth / 2.0f - 113, 180, 225, 50};
	bool mouseOnText = false;
	int framesCounter = 0;
	bool end = false;
	int count = 0;
	SetTargetFPS(60);
	// Main loop
	while (!WindowShouldClose() && !end)
	{
		// UpdateMusicStream(music);
		if (CheckCollisionPointRec(GetMousePosition(), textBox))
		{
			mouseOnText = true;
		}
		else
		{
			mouseOnText = false;
		}
		if (mouseOnText)
		{
			SetMouseCursor(MOUSE_CURSOR_IBEAM);
			int key = GetCharPressed();
			while (key > 0)
			{
				if ((key >= 32) && (key <= 125) && (letterCount < 8))
				{
					name[letterCount] = (char)key;
					name[letterCount + 1] = '\0';
					letterCount++;
					count++;
				}
				key = GetCharPressed();
			}
			if (IsKeyPressed(KEY_BACKSPACE))
			{
				letterCount--;
				if (letterCount < 0)
				{
					letterCount = 0;
				}
				name[letterCount] = '\0';
				count--;
			}
		}
		else
		{
			SetMouseCursor(MOUSE_CURSOR_DEFAULT);
		}
		if (mouseOnText)
		{
			framesCounter++;
		}
		else
		{
			framesCounter = 0;
		}
		BeginDrawing();
		ClearBackground(BLACK);
		DrawRectangleRec(textBox, LIGHTGRAY);
		if (mouseOnText)
		{
			DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
		}
		else
		{
			DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);
		}
		for (int i = 0; i < 9; i++)
		{
			name[i] = toupper(name[i]);
		}
		DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);
		DrawText("ENTER YOUR NAME:", screenWidth / 2 - 103, screenHeight / 2 - 80, 20, DARKGRAY);
		if (IsKeyDown(KEY_ENTER) && count != 0)
		{
			end = true;
			*check = true;
		}
		EndDrawing();
	}
	return name;
}