#include <iostream>
#include <SDL.h>
#include <vector>
#include <string>
#include <SDL_ttf.h>
#include <ctime>
#include "GameFinish.h"
#include "PlayerAndFood.h"
#include "Score.h"
using namespace std;

bool checkEat(int foodx, int foody, int playerx, int playery){
	if (playerx == foodx && playery == foody){
		return true;
	}
	return false;
}

int main(int argc, char* argv[]){

	SDL_Init(SDL_INIT_EVERYTHING);

	if (TTF_Init() < 0){
		cout << "Error: " << TTF_GetError() << endl;
	}

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;

	//player rectangle
	SDL_Rect player;
	player.x = 0;
	player.y = 0;
	player.h = 0;
	player.w = 0;

	int tailLength = 0;

	// Vectors for storage of tail block positions
	vector<int> tailX;
	vector<int> tailY;

	// Size of tiles
	int scale = 24;
	int wScale = 24;

	// Player position variables
	int x = 0;
	int y = 0;
	int prevX = 0;
	int prevY = 0;

	// Movement controls
	bool up = false;
	bool down = false;
	bool right = false;
	bool left = false;

	bool inputThisFrame = false;
	bool redo = false;

	// Food rectangle
	SDL_Rect food;
	food.w = scale;
	food.h = scale;
	food.x = 0;
	food.y = 0;

	pair<int, int> foodLoc = getFood(tailX, tailY, x, y, scale, wScale, tailLength);
	food.x = foodLoc.first;
	food.y = foodLoc.second;

	// Show the window with these settings and apply a renderer to it
	window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, scale*wScale+1, scale*wScale+1, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	float time = SDL_GetTicks() / 100;

	//limit FPS
	const int FPS = 60;
    const int frameDelay = 1000/FPS;

    Uint32 frameStart;
    int frameTime;


	// Main game loop
	while (true){

        frameStart = SDL_GetTicks();

        frameTime = SDL_GetTicks() - frameStart;
        if(frameDelay>frameTime){
            SDL_Delay(frameDelay - frameTime);
        }

		float newTime = SDL_GetTicks()/50; // 50 is the speed of player, bigger number = slower speed
		float delta = newTime - time;
		time = newTime;

		inputThisFrame = false;

		// Controls
		if (SDL_PollEvent(&event)){

			// Exit
			if (event.type == SDL_QUIT){
				exit(0);
			}

			// If a key is pressed
			if (event.type == SDL_KEYDOWN && inputThisFrame == false){

				// Then check for the key being pressed and change direction accordingly
				if (down == false && event.key.keysym.scancode == SDL_SCANCODE_UP){
					up = true;
					left = false;
					right = false;
					down = false;
					inputThisFrame = true;
				}
				else if (right == false && event.key.keysym.scancode == SDL_SCANCODE_LEFT){
					up = false;
					left = true;
					right = false;
					down = false;
					inputThisFrame = true;
				}
				else if (up == false && event.key.keysym.scancode == SDL_SCANCODE_DOWN){
					up = false;
					left = false;
					right = false;
					down = true;
					inputThisFrame = true;
				}
				else if (left == false && event.key.keysym.scancode == SDL_SCANCODE_RIGHT){
					up = false;
					left = false;
					right = true;
					down = false;
					inputThisFrame = true;
				}
			}
		}

		// The previous position of the player block
		prevX = x;
		prevY = y;

		if (up){
			y -= delta * scale;
		}
		else if (left){
			x -= delta * scale;
		}
		else if (right){
			x += delta * scale;
		}
		else if (down){
			y += delta * scale;
		}

		if (redo == true){

			redo = false;
			foodLoc = getFood(tailX, tailY, x, y, scale, wScale, tailLength);
			food.x = foodLoc.first;
			food.y = foodLoc.second;

			if (food.x == -100 && food.y == -100){
				redo = true;
			}
		}

		// Eat checking, has played collided with food?
		if (checkEat(food.x, food.y, x, y)){

			// Spawn new food after it has been eaten
			foodLoc = getFood(tailX, tailY, x, y, scale, wScale, tailLength);
			food.x = foodLoc.first;
			food.y = foodLoc.second;

			if (food.x == -100 && food.y == -100){
				redo = true;
			}
			tailLength++;
		}

		// Only runs in the frames where the player block has moved
		if (delta * scale == 24){

			// Update tail size and position
			if (tailX.size() != tailLength){
				tailX.push_back(prevX);
				tailY.push_back(prevY);
			}

			//This updates the blocks from end (farthest from player block) to the start (nearest to player block)
			for (int i = 0; i < tailLength; i++){

				if (i > 0) {
					tailX[i - 1] = tailX[i];
					tailY[i - 1] = tailY[i];
				}
			}

			if (tailLength > 0){
				tailX[tailLength - 1] = prevX;
				tailY[tailLength - 1] = prevY;
			}
		}

		//Win condition
		if (tailLength == 576){
			youWin(renderer, event, scale, wScale, tailLength);
			x = 0;
			y = 0;
			up = false;
			left = false;
			right = false;
			down = false;
			tailX.clear();
			tailY.clear();
			tailLength = 0;
			redo = false;
			foodLoc = getFood(tailX, tailY, x, y, scale, wScale, tailLength);

			if (food.x == -100 && food.y == -100){
				redo = true;
			}

			food.x = foodLoc.first;
			food.y = foodLoc.second;
		}

        //GameOver condition
		for (int i = 0; i < tailLength; i++){

			if (x == tailX[i] && y == tailY[i]){
				gameOver(renderer, event, scale, wScale, tailLength);
				x = 0;
				y = 0;
				up = false;
				left = false;
				right = false;
				down = false;
				tailX.clear();
				tailY.clear();
				tailLength = 0;
				redo = false;

				foodLoc = getFood(tailX, tailY, x, y, scale, wScale, tailLength);
				if (food.x == -100 && food.y == -100) {
					redo = true;
				}

				food.x = foodLoc.first;
				food.y = foodLoc.second;
			}
		}

		// Game over if player out of bounds, also resets the game state
		if (x < 0 || y < 0 || x > scale * wScale - scale || y > scale * wScale - scale){
			gameOver(renderer, event, scale, wScale, tailLength);
			x = 0;
			y = 0;
			up = false;
			left = false;
			right = false;
			down = false;
			tailX.clear();
			tailY.clear();
			tailLength = 0;
			redo = false;
			foodLoc = getFood(tailX, tailY, x, y, scale, wScale, tailLength);
			food.x = foodLoc.first;
			food.y = foodLoc.second;

			if (food.x == -100 && food.y == -100){
				redo = true;
			}
		}



		// Render everything
		renderFood(renderer, food);
		renderPlayer(renderer, player, x, y, scale, tailX, tailY, tailLength);
		renderScore(renderer, tailLength, scale, wScale);


		SDL_RenderDrawLine(renderer, 0, 0, 0, 24 * 24);      //24*24 is the size of blocks
		SDL_RenderDrawLine(renderer, 0, 24*24, 24 * 24, 24 * 24);
		SDL_RenderDrawLine(renderer, 24*24, 24 * 24, 24*24, 0);
		SDL_RenderDrawLine(renderer, 24*24, 0, 0, 0);

		// Put everything on screen
		SDL_RenderPresent(renderer);

		// Background
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
	}

	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
	return 0;
}
