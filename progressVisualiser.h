#pragma once
class ProgressVisualiser
{
private:
	SDL_Rect rect;
	int size;
	int step;
public:
	ProgressVisualiser();
	void draw(SDL_Renderer* renderer);
};

ProgressVisualiser::ProgressVisualiser()
{
	size = 50;
	rect = { 0, 0, size, size};
	step = 3;
}

void ProgressVisualiser::draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

	if (rect.x < 0 || rect.x > 20)
	{
		step = -step;
	}
	rect.x += step;
	SDL_RenderFillRect(renderer, &rect);
	SDL_RenderPresent(renderer);
}