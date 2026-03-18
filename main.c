#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>

typedef struct screen
{
    uint32_t *Pixels;
    int Pitch;
    int Width;
    int Height;
} screen;

uint32_t RGBA2BGRA(uint8_t Red, uint8_t Green, uint8_t Blue, uint8_t Alpha)
{
    return (Alpha << 24)|(Red << 16)|(Green << 8)|Blue;    
}

void DrawPixel(screen *Screen, int X, int Y, uint32_t Color)
{
    if(X >= 0 && X < Screen->Width && Y >= 0 && Y < Screen->Height)
        Screen->Pixels[Y * (Screen->Pitch / 4) + X] = Color;
}

int main(int argc, char **argv)
{
    int ScreenWidth = 1024;
    int ScreenHeight = 576;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *Window = SDL_CreateWindow("Tedit", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    ScreenWidth, ScreenHeight, SDL_WINDOW_SHOWN);

    SDL_Surface *ScreenSurface = SDL_GetWindowSurface(Window);
    
    screen Screen = {0};
    Screen.Width = ScreenWidth;
    Screen.Height = ScreenHeight;
    Screen.Pitch = ScreenSurface->pitch;
    Screen.Pixels = ScreenSurface->pixels;

    int Running = 1;
    while(Running)
    {
        SDL_Event Event;
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                Running = 0;
                break;
            }
        }

        // Clear Screen
        SDL_LockSurface(ScreenSurface);
        for(int y = 0; y < 576; ++y)
        {
            for(int x = 0; x < 1024; ++x)
                DrawPixel(&Screen, x, y, RGBA2BGRA(255, 150, 0, 0)); //BGRA
        }
        SDL_UnlockSurface(ScreenSurface);

        

        SDL_UpdateWindowSurface(Window);
        SDL_Delay(10);
    }

    SDL_Quit();

    return 0;
}