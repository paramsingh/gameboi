struct gpu
{
    // values in the pixels
    int pixels[160][144];
    // value in the clock of the gpu
    int clock;
    // the mode in which the gpu is
    // 0 - horizontal blank
    // 1 - vertical blank
    // 2 - scanline (accessing oam)
    // 3 - scanline (accessing vram)
    int mode;

    // line number being drawn
    int line;

    // pointer to the cpu with which the gpu will work
    cpu* c;

    // pointer to SDL gui
    gui* screen;

    // constructor that will set everything else to zero
    // and set cpu and the screen to the one passed
    gpu(cpu*, gui*);

    // change mode
    void change_mode(int);
    // change scanline
    void change_scanline();

    // update the gpu timings and mode etc.
    void step();

    // there are two tilemaps in memory
    int which_map;

    // returns if the lcd is on or not
    // this is done by accessing the lcd control register in the
    // cpu
    bool on();

    void render_tiles();


    void draw_pixels();

    int getcolor(int);
    int cnt;
};
