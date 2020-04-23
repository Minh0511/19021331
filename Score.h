#include <iostream>
#include <SDL.h>
void renderScore(SDL_Renderer* renderer, int tailLength, int scale, int wScale){
	SDL_Color Black = { 255, 255, 255 };

	TTF_Font* font = TTF_OpenFont((char*)"arial.ttf", 10);
	if (font == NULL) {
		cout << "Font loading error" << endl;
	}

	SDL_Surface* score = TTF_RenderText_Solid(font, (string("Score: ") + to_string(tailLength * 10)).c_str(), Black);
	SDL_Texture* scoreMessage = SDL_CreateTextureFromSurface(renderer, score);
	SDL_Rect scoreRect;
	scoreRect.w = 100;
	scoreRect.h = 30;
	scoreRect.x = ((scale*wScale) / 2) - (scoreRect.w / 2);
	scoreRect.y = 0;
	SDL_RenderCopy(renderer, scoreMessage, NULL, &scoreRect);

	TTF_CloseFont(font);
}
