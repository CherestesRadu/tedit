#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "font8x8_basic.h"
#include "render.c"

void OpenFile(char **Buffer, char *File)
{
    // TODO: special chars with binary
    FILE *Handle = fopen(File, "r");
    if(!Handle)
    {
        printf("File not open!\n");
        return;
    }
    fseek(File, 0, SEEK_END);
    size_t Filesize = ftell(File);
    rewind(File);


    fclose(File);
}

void WriteFile(char *File)
{

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

    const int MaxLines = 1024;
    const int MaxChars = 256;

    char TextBuffer[MaxLines][MaxChars];
    memset(TextBuffer, 0, MaxLines * MaxChars);
    int Cursor[2] = {0};

    SDL_StartTextInput();

    int Running = 1;
    int CursorVisible = 1;
    uint32_t LastBlinkTime = 0;
    while(Running)
    {
#if 0
        uint32_t CurrentTime = SDL_GetTicks();
        if (CurrentTime - LastBlinkTime >= 250) {
            CursorVisible ^= 1;
            LastBlinkTime = CurrentTime;
        }
#endif


        SDL_Event Event;
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                Running = 0;
                break;
            }
            if(Event.type == SDL_TEXTINPUT)
            {
                int Length = strlen(Event.text.text);
                for(int Index = 0; Index < Length && Cursor[0] < MaxChars; ++Index, ++Cursor[0])
                {
                    TextBuffer[Cursor[1]][Cursor[0]] = Event.text.text[Index];
                }
            }

            if(Event.type == SDL_KEYDOWN)
            {
                if(Event.key.keysym.sym == SDLK_RETURN)
                {
                    // TODO: Middle of line

                    if(Cursor[1] < MaxLines)
                    {
                        ++Cursor[1];
                        Cursor[0] = 0;
                    }
                }
                if(Event.key.keysym.sym == SDLK_BACKSPACE)
                {
                    
                    // No characters left, go up a line
                    if(Cursor[0] == 0)
                    {
                        // Go up only if it's not first
                        if(Cursor[1] != 0)
                        --Cursor[1];

                        int StringLength = strlen(TextBuffer[Cursor[1]]);
                        Cursor[0] = StringLength;
                    }
                    else if(Cursor[0] > 0) // Delete last, move cursor
                    {
                        int StringLength = strlen(TextBuffer[Cursor[1]]);
                        TextBuffer[Cursor[1]][StringLength - 1] = 0;
                        --Cursor[0];
                    }
                }
            }
        }

        // Clear Screen
        SDL_LockSurface(ScreenSurface);
        for(int y = 0; y < 576; ++y)
        {
            for(int x = 0; x < 1024; ++x)
                DrawPixel(&Screen, x, y, RGBA2BGRA(50, 50, 50, 0)); //BGRA
        }

        if(CursorVisible)
        {
            for(int Y = 0; Y < 8 * FontScale; ++Y)
            {
                for(int X = 0; X < 8 * FontScale; ++X)
                {
                    int PX = Cursor[0] * 8 * FontScale + X;
                    int PY = Cursor[1] * 8 * FontScale + Y;
                    DrawPixel(&Screen, PX, PY, 0xffffffff);
                }
            }
        }

        for(int Line = 0; Line < MaxLines; ++Line)
        {
            DrawString(&Screen, 0, Line * 8 * FontScale, TextBuffer[Line]);
        }
        SDL_UnlockSurface(ScreenSurface);

        

        SDL_UpdateWindowSurface(Window);
        SDL_Delay(10);
        printf("Cursor Position: %d %d\n", Cursor[0], Cursor[1]);


    }

    SDL_Quit();

    return 0;
}