
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
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

}

void gui::clear()
{
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    reset_texture();
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
void gui::reset_texture()
{
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    // Fill the surface white
    SDL_FillRect( surface, NULL, SDL_MapRGB(surface->format, 0xff, 0xff, 0xff));
    // Update the surface
    // SDL_UpdateWindowSurface(window);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}
