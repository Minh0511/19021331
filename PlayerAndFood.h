#ifndef PLAYERANDFOOD_H
#define PLAYERANDFOOD_H
#include <iostream>
#include <SDL.h>
using namespace std;
//Create player
void renderPlayer(SDL_Renderer* renderer, SDL_Rect player, int x, int y, int scale, vector<int> tailX, vector<int> tailY, int tailLength){
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	player.w = scale;
	player.h = scale;

	// Render tail blocks
	for (int i = 0; i < tailLength; i++){
		player.x = tailX[i];
		player.y = tailY[i];
		SDL_RenderFillRect(renderer, &player);
	}
	player.x = x;
	player.y = y;
	SDL_RenderFillRect(renderer, &player);
}
//Render food
void renderFood(SDL_Renderer* renderer, SDL_Rect food){
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &food);
}

// Spawn food and make sure it dont spawn on the body of player
pair<int, int> getFood(vector<int> tailX, vector<int> tailY, int playerX, int playerY, int scale, int wScale, int tailLength){
    bool valid = false;
	int x = 0;
	int y = 0;
	srand(time(0));
	x = scale * (rand() % wScale);
	y = scale * (rand() % wScale);
	valid = true;

	// Check all tail blocks and player block
	for (int i = 0; i < tailLength; i++) {
		if ((x == tailX[i] && y == tailY[i]) || (x == playerX && y == playerY)){
			valid = false;
		}
	}
	if (valid == false) {
		pair<int, int> foodLoc;
		foodLoc = make_pair(NULL, NULL);
		return foodLoc;
	}

	pair<int, int> foodLoc;
	foodLoc = make_pair(x, y);
	return foodLoc;
}

#endif // PLAYERANDFOOD_H
