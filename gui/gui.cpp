
gui::gui()
{
    window = NULL;
    renderer = NULL;
    texture = NULL;
}


void gui::init()
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Gameboi", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    // Create renderer for window
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

}
