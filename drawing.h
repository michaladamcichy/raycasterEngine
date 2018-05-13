#pragma once
const int SIZE_OF_DRAWN_POINT = 5;

void drawLine(SDL_Renderer* &myRenderer, MyPoint A, MyPoint B, float MINIMAP_SCALE = 1)
{
	SDL_RenderDrawLine(myRenderer, int(A.x*MINIMAP_SCALE), int(A.y*MINIMAP_SCALE), int(B.x*MINIMAP_SCALE), int(B.y*MINIMAP_SCALE));
}
void drawPoint(SDL_Renderer* myRenderer, MyPoint inPoint, float MINIMAP_SCALE = 1)
{
	MyPoint point = scalePoint(inPoint, MINIMAP_SCALE);
	SDL_SetRenderDrawColor(myRenderer, 46, 0, 255, SDL_ALPHA_OPAQUE);

	SDL_Rect rect = { int(point.x) - SIZE_OF_DRAWN_POINT / 2, int(point.y) - SIZE_OF_DRAWN_POINT / 2,
		SIZE_OF_DRAWN_POINT, SIZE_OF_DRAWN_POINT };
	SDL_RenderFillRect(myRenderer, &rect);
}
void drawGood(SDL_Renderer* myRenderer)
{
	SDL_Rect rect = { 0,0, 100, 100 };
	SDL_SetRenderDrawColor(myRenderer, 0, 255, 0, 255);
	SDL_RenderFillRect(myRenderer, &rect);
}
void drawBad(SDL_Renderer* myRenderer)
{
	SDL_Rect rect = { 0,0, 20, 20 };
	SDL_SetRenderDrawColor(myRenderer, 255, 0, 0, 255);
	SDL_RenderFillRect(myRenderer, &rect);
}