#include <iostream>
#include <SDL.h>
#include <vector>
#include <string>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <ctime>
#include <cstring>
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
	int scale = 30;
	int wScale = 30;

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

	// Food rectangle
	SDL_Rect food;
	food.w = scale;
	food.h = scale;
	food.x = 0;
	food.y = 0;

	pair<int, int> foodLoc = getFood(tailX, tailY, x, y, scale, wScale, tailLength);
	food.x = foodLoc.first;
	food.y = foodLoc.second;

    bool redo = false;

	// Show the window with these settings and apply a renderer to it
	window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, scale*wScale+1, scale*wScale+1, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	float time = SDL_GetTicks() / 100;

	//limit FPS
	const int FPS = 60;
    const int frameDelay = 1000/FPS;

    Uint32 frameStart;
    int frameTime;

    //background music
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1){
        cout<<Mix_GetError<<endl;
    }
    Mix_Music *music = Mix_LoadMUS("cat.mp3");
    {
        if (music == NULL){
            cout<<Mix_GetError<<endl;
        }
    }
    Mix_PlayMusic(music, -1);

    //GameOver sound
    Mix_Music *die = Mix_LoadMUS("darksouls.mp3");
    {
        if (die == NULL){
            cout<<Mix_GetError<<endl;
        }
    }

    //Winning sound
    Mix_Music *win = Mix_LoadMUS("win.mp3");
    {
        if (win == NULL){
            cout<<Mix_GetError<<endl;
        }
    }

    //moving sound
    Mix_Chunk *go = Mix_LoadWAV("bruh.wav");
    {
        if (go == NULL){
            cout<<Mix_GetError<<endl;
        }
    }
	// Main game loop
	while (true){

        frameStart = SDL_GetTicks();

		float newTime = SDL_GetTicks()/75; // 50 is the speed of player, bigger number = slower speed
		float delta = newTime - time;
		time = newTime;

		// Controls
		if (SDL_PollEvent(&event)){

			// Exit
			if (event.type == SDL_QUIT){
				exit(0);
			}

			// If a key is pressed
			if (event.type == SDL_KEYDOWN ){

				// Then check for the key being pressed and change direction accordingly
				if (down == false && event.key.keysym.scancode == SDL_SCANCODE_UP){
					up = true;
					left = false;
					right = false;
					down = false;
				}
				else if (right == false && event.key.keysym.scancode == SDL_SCANCODE_LEFT){
					up = false;
					left = true;
					right = false;
					down = false;
				}
				else if (up == false && event.key.keysym.scancode == SDL_SCANCODE_DOWN){
					up = false;
					left = false;
					right = false;
					down = true;
				}
				else if (left == false && event.key.keysym.scancode == SDL_SCANCODE_RIGHT){
					up = false;
					left = false;
					right = true;
					down = false;
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

            foodLoc = getFood(tailX, tailY, x, y, scale, wScale, tailLength);
			food.x = foodLoc.first;
			food.y = foodLoc.second;
			redo = false;

			if (food.x == NULL && food.y == NULL){
				redo = true;
			}
		}

		// Eat checking, has player collided with food?
		if (checkEat(food.x, food.y, x, y)){

			// Spawn new food after it has been eaten
			foodLoc = getFood(tailX, tailY, x, y, scale, wScale, tailLength);
			food.x = foodLoc.first;
			food.y = foodLoc.second;

			if (food.x == NULL && food.y == NULL){
				redo = true;
			}
			Mix_PlayChannel(-1, go, 0);
			tailLength++;
		}

		if (delta * scale == 30){

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
		if (tailLength == 4){
            Mix_HaltMusic();
            Mix_PlayMusic(win, 1);
			youWin(renderer, event, scale, wScale, tailLength);
			Mix_PlayMusic(music, -1);
			x = 0;
			y = 0;
			up = false;
			left = false;
			right = false;
			down = false;
			tailX.clear();
			tailY.clear();
			tailLength = 0;
			foodLoc = getFood(tailX, tailY, x, y, scale, wScale, tailLength);
			food.x = foodLoc.first;
			food.y = foodLoc.second;

			if (food.x == NULL && food.y == NULL){
				redo = true;
			}
		}

        //GameOver condition
		for (int i = 0; i < tailLength; i++){

			if (x == tailX[i] && y == tailY[i]){
                Mix_HaltMusic();
                Mix_PlayMusic(die, 1);
				gameOver(renderer, event, scale, wScale, tailLength);
				Mix_PlayMusic(music, -1);
				x = 0;
				y = 0;
				up = false;
				left = false;
				right = false;
				down = false;
				tailX.clear();
				tailY.clear();
				tailLength = 0;
				foodLoc = getFood(tailX, tailY, x, y, scale, wScale, tailLength);
                food.x = foodLoc.first;
                food.y = foodLoc.second;
				if (food.x == NULL && food.y == NULL) {
					redo = true;
				}
			}
		}

		// Game over if player out of bounds, also resets the game state
		if (x < 0 || y < 0 || x > scale * wScale - 1 || y > scale * wScale - 1 ){
            Mix_HaltMusic();
            Mix_PlayMusic(die, 1);
			gameOver(renderer, event, scale, wScale, tailLength);
			Mix_PlayMusic(music, -1);
			x = 0;
			y = 0;
			up = false;
			left = false;
			right = false;
			down = false;
			tailX.clear();
			tailY.clear();
			tailLength = 0;
			foodLoc = getFood(tailX, tailY, x, y, scale, wScale, tailLength);
			food.x = foodLoc.first;
			food.y = foodLoc.second;
			if (food.x == NULL && food.y == NULL){
				redo = true;
			}
		}



		// Render everything
		renderFood(renderer, food);
		renderPlayer(renderer, player, x, y, scale, tailX, tailY, tailLength);
		renderScore(renderer, tailLength, scale, wScale);
		renderName(renderer, scale, wScale);


		SDL_RenderDrawLine(renderer, 0, 0, 0, 30 * 30);      //30 * 30 is the size of blocks
		SDL_RenderDrawLine(renderer, 0, 30 * 30, 30 * 30, 30 * 30);
		SDL_RenderDrawLine(renderer, 30 * 30, 30 * 30, 30 * 30, 0);
		SDL_RenderDrawLine(renderer, 30 * 30, 0, 0, 0);

		// Put everything on screen
		SDL_RenderPresent(renderer);

		// Background
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        //FPS limit
		frameTime = SDL_GetTicks() - frameStart;
            if(frameDelay>frameTime){
                SDL_Delay(frameDelay - frameTime);
            }
	}

	SDL_DestroyWindow(window);
	Mix_Quit();
	Mix_FreeMusic(music);
	Mix_FreeChunk(go);
	music = NULL;
	go = NULL;
	TTF_Quit();
	SDL_Quit();
	return 0;
}
