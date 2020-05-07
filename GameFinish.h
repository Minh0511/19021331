#ifndef GAMEFINISH_H
#define GAMEFINISH_H
#include<iostream>
#include<SDL.h>
using namespace std;

void youWin(SDL_Renderer* renderer, SDL_Event event, int scale, int wScale, int tailLength){

	SDL_Color Red = { 255, 0, 0 };
	SDL_Color White = { 255, 255, 255 };
	SDL_Color Yellow = { 255, 255, 0 };

	TTF_Font* font = TTF_OpenFont((char*)"arial.ttf", 10);
	if (font == NULL){
		cout << "Font loading error" << endl;
		return;
	}

	SDL_Surface* score = TTF_RenderText_Solid(font, (string("Score: ") + to_string(tailLength * 10)).c_str(),White);
	SDL_Surface* gameover = TTF_RenderText_Solid(font, "You win!", Yellow);
	SDL_Surface* retry = TTF_RenderText_Solid(font, "Press Enter to play again", White);
	SDL_Texture* gameoverMessage = SDL_CreateTextureFromSurface(renderer, gameover);
	SDL_Texture* retryMessage = SDL_CreateTextureFromSurface(renderer, retry);
	SDL_Texture* scoreMessage = SDL_CreateTextureFromSurface(renderer, score);
	SDL_Rect gameoverRect;
	SDL_Rect retryRect;
	SDL_Rect scoreRect;

	scoreRect.h = 30;
	scoreRect.w = 100;
	scoreRect.x = ((scale*wScale)/2) - (scoreRect.w/2);
	scoreRect.y = 0;
	gameoverRect.h = 200;
	gameoverRect.w = 300;
	gameoverRect.x = ((scale*wScale)/2) - (gameoverRect.w/2);
	gameoverRect.y = ((scale*wScale)/2) - (gameoverRect.h/2) - 50;
	retryRect.h = 100;
	retryRect.w = 300;
	retryRect.x = ((scale*wScale)/2) - ((retryRect.w/2));
	retryRect.y = (((scale*wScale)/2) - ((retryRect.h/2)) + 150);
	SDL_RenderCopy(renderer, gameoverMessage, NULL, &gameoverRect);
	SDL_RenderCopy(renderer, retryMessage, NULL, &retryRect);
	SDL_RenderCopy(renderer, scoreMessage, NULL, &scoreRect);

	TTF_CloseFont(font);

	// Show victory screen while Enter has not been pressed
	while (true){
		SDL_RenderPresent(renderer);

		if (SDL_PollEvent(&event)){

			if (event.type == SDL_QUIT){
				exit(0);
			}

			if (event.key.keysym.scancode == SDL_SCANCODE_RETURN){
				return;
			}
		}
	}
}

void gameOver(SDL_Renderer* renderer, SDL_Event event, int scale, int wScale, int tailLength){
	SDL_Color Red = { 255, 0, 0 };
	SDL_Color White = { 255, 255, 255 };

	// Get the font used for displaying text
	TTF_Font* font = TTF_OpenFont((char*)"arial.ttf", 10);
	if (font == NULL){
		cout << "Font loading error" << endl;
		return;
	}

	SDL_Surface* score = TTF_RenderText_Solid(font, (string("Score: ") + to_string(tailLength * 10)).c_str(), White);
	SDL_Surface* gameover = TTF_RenderText_Solid(font, "YOU DIED", Red);
	SDL_Surface* retry = TTF_RenderText_Solid(font, "Press Enter to retry", White);
	SDL_Texture* gameoverMessage = SDL_CreateTextureFromSurface(renderer, gameover);
	SDL_Texture* retryMessage = SDL_CreateTextureFromSurface(renderer, retry);
	SDL_Texture* scoreMessage = SDL_CreateTextureFromSurface(renderer, score);
	SDL_Rect gameoverRect;
	SDL_Rect retryRect;
	SDL_Rect scoreRect;

	scoreRect.h = 30;
	scoreRect.w = 100;
	scoreRect.x = ((scale*wScale)/2) - (scoreRect.w/2);
	scoreRect.y = 0;
	gameoverRect.h = 200;
	gameoverRect.w = 300;
	gameoverRect.x = ((scale*wScale)/2) - (gameoverRect.w/2);
	gameoverRect.y = ((scale*wScale)/2) - (gameoverRect.h/2) - 50;
	retryRect.h = 100;
	retryRect.w = 300;
	retryRect.x = ((scale*wScale)/2) - ((retryRect.w/2));
	retryRect.y = (((scale*wScale)/2) - ((retryRect.h/2)) + 150);
	SDL_RenderCopy(renderer, gameoverMessage, NULL, &gameoverRect);
	SDL_RenderCopy(renderer, retryMessage, NULL, &retryRect);
	SDL_RenderCopy(renderer, scoreMessage, NULL, &scoreRect);

	TTF_CloseFont(font);

	// Show game over screen while Enter has not been pressed
	while (true){
		SDL_RenderPresent(renderer);

		if (SDL_PollEvent(&event)){
			if (event.type == SDL_QUIT){
				exit(0);
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_RETURN){
				return;
			}
		}
	}
}

#endif // GAMEFINISH_H
