#include <iostream>
#include <SDL.h>
void renderScore(SDL_Renderer* renderer, int tailLength, int scale, int wScale){
	SDL_Color White = { 255, 255, 255 };

	TTF_Font* font = TTF_OpenFont((char*)"arial.ttf", 10);
	if (font == NULL) {
		cout << "Font loading error" << endl;
	}

	SDL_Surface* score = TTF_RenderText_Solid(font, (string("Score: ") + to_string(tailLength * 10)).c_str(), White);
	SDL_Texture* scoreMessage = SDL_CreateTextureFromSurface(renderer, score);
	SDL_Rect scoreRect;
	scoreRect.w = 100;
	scoreRect.h = 30;
	scoreRect.x = ((scale*wScale) / 2) - (scoreRect.w / 2);
	scoreRect.y = 0;
	SDL_RenderCopy(renderer, scoreMessage, NULL, &scoreRect);

	TTF_CloseFont(font);
}

void renderName(SDL_Renderer* renderer, int scale, int wScale){
	SDL_Color White = { 255, 255, 255 };

	TTF_Font* font = TTF_OpenFont((char*)"arial.ttf", 10);
	if (font == NULL) {
		cout << "Font loading error" << endl;
	}

	SDL_Surface* name = TTF_RenderText_Solid(font, (string("by PVM ")).c_str(), White);
	SDL_Texture* nameMessage = SDL_CreateTextureFromSurface(renderer, name);
	SDL_Rect nameRect;
	nameRect.w = 75;
	nameRect.h = 20;
	nameRect.x = ((scale*wScale) / 2) - (nameRect.w / 2) + 250;
	nameRect.y = ((scale*wScale) / 2) - (nameRect.h / 2) + 270;
	SDL_RenderCopy(renderer, nameMessage, NULL, &nameRect);

	TTF_CloseFont(font);
}
