// GUI class here
// will use sdl

const int SCREEN_WIDTH = 160;
const int SCREEN_HEIGHT = 144;

struct gui
{
    SDL_Window*   window;
    SDL_Surface*  surface;
    SDL_Renderer* renderer;
    SDL_Texture*  texture;

    gui();
    void init();
};
