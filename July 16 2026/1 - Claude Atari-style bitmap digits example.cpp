#include <SDL.h>
#include <vector>

// Each digit is a 3-wide x 5-tall grid, matching that blocky Atari style.
// 1 = pixel on, 0 = pixel off. Easy to eyeball-design by looking at the grid shape.
const int DIGIT_WIDTH = 3;
const int DIGIT_HEIGHT = 5;

const int digitBitmaps[10][DIGIT_HEIGHT][DIGIT_WIDTH] = {
	// 0
	{{1,1,1},
	 {1,0,1},
	 {1,0,1},
	 {1,0,1},
	 {1,1,1}},
	 // 1
	 {{0,1,0},
	  {1,1,0},
	  {0,1,0},
	  {0,1,0},
	  {1,1,1}},
	  // 2
	  {{1,1,1},
	   {0,0,1},
	   {1,1,1},
	   {1,0,0},
	   {1,1,1}},
	   // 3
	   {{1,1,1},
		{0,0,1},
		{0,1,1},
		{0,0,1},
		{1,1,1}},
		// 4
		{{1,0,1},
		 {1,0,1},
		 {1,1,1},
		 {0,0,1},
		 {0,0,1}},
		 // 5
		 {{1,1,1},
		  {1,0,0},
		  {1,1,1},
		  {0,0,1},
		  {1,1,1}},
		  // 6
		  {{1,1,1},
		   {1,0,0},
		   {1,1,1},
		   {1,0,1},
		   {1,1,1}},
		   // 7
		   {
			{1,1,1},
			{0,0,1},
			{0,0,1},
			{0,0,1},
			{0,0,1}},
			// 8
			{{1,1,1},
			 {1,0,1},
			 {1,1,1},
			 {1,0,1},
			 {1,1,1}},
			 // 9
			 {{1,1,1},
			  {1,0,1},
			  {1,1,1},
			  {0,0,1},
			  {1,1,1}}
};

// Converts a digit's bitmap into a list of SDL_Points, offset to (originX, originY)
std::vector<SDL_Point> GetDigitPoints(int digit, int originX, int originY) {
	std::vector<SDL_Point> points;
	for (int row = 0; row < DIGIT_HEIGHT; row++) {
		for (int col = 0; col < DIGIT_WIDTH; col++) {
			if (digitBitmaps[digit][row][col] == 1) {
				points.push_back({ originX + col, originY + row });
			}
		}
	}
	return points;
}

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);

	const int SCREEN_HEIGHT = 480;
	const int SCREEN_WIDTH = 640;
	const float SCALE = 20.0f;

	SDL_Window* window = SDL_CreateWindow("Digit Sprite Test",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_RenderSetScale(renderer, SCALE, SCALE);

	SDL_SetRenderDrawColor(renderer, 40, 20, 80, 255); // dark purple, Basic Math-ish
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 220, 180, 60, 255); // gold

	std::vector<SDL_Point> nine = GetDigitPoints(9, 5, 3);
	SDL_RenderDrawPoints(renderer, nine.data(), static_cast<int>(nine.size()));

	SDL_RenderPresent(renderer);

	bool running = true;
	SDL_Event e;
	while (running) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) running = false;
		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}