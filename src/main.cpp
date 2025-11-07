#include <SDL3/SDL.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    int displayCount = 0;
    SDL_DisplayID *displays = SDL_GetDisplays(&displayCount);

    // Check for displays
    if(!displays || displayCount == 0)
    {
        SDL_Log("No displays found: %s", SDL_GetError());
        return -1;
    }

    SDL_DisplayID disp = displays[0];
    SDL_free(displays);
    const SDL_DisplayMode *displayMode = SDL_GetCurrentDisplayMode(disp);

    // Check for any other display-related failure
    if (!displayMode) {
        SDL_Log("SDL_GetCurrentDisplayMode failed: %s", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Raytracer", displayMode->w/2, displayMode->h/2, SDL_WINDOW_RESIZABLE);

    if (!window) {
        SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
