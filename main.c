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

#define MAX_LINES 1024
#define MAX_CHARS 256

typedef struct text_buffer
{
    char Lines[MAX_LINES][MAX_CHARS];
    int Cursor[2];
} text_buffer;

void SwapLines(text_buffer *Buffer, int Line0, int Line1)
{

}

void HandleNewLine(text_buffer *Buffer)
{
    // Take everything from the rightside and put it on the next line
    
    if(Buffer->Cursor[1] < MAX_LINES - 1)
    {
        char *Target = Buffer->Lines[Buffer->Cursor[1] + 1];
        int CharsLeft = MAX_CHARS - Buffer->Cursor[0];
        memcpy(Target, Buffer->Lines[Buffer->Cursor[1]] + Buffer->Cursor[0], CharsLeft);
        memset(Buffer->Lines[Buffer->Cursor[1]] + Buffer->Cursor[0], 0, CharsLeft);
        Buffer->Cursor[0] = 0;
        ++Buffer->Cursor[1];
    }
    else
    {
        // TODO: Grow buffer
    }
}

void HandleBackspace(text_buffer *Buffer)
{

}

void HandleUpArrow(text_buffer *Buffer)
{
    if(Buffer->Cursor[1] > 0)
        --Buffer->Cursor[1];
}

void HandleDownArrow(text_buffer *Buffer)
{
    if(Buffer->Cursor[1] < MAX_LINES - 1)
        ++Buffer->Cursor[1];
}

void HandleLeftArrow(text_buffer *Buffer)
{
    if(Buffer->Cursor[0] > 0)
        --Buffer->Cursor[0];
}

void HandleRightArrow(text_buffer *Buffer)
{
    if(Buffer->Cursor[0] < MAX_CHARS - 1)
        ++Buffer->Cursor[0];
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


    text_buffer TextBuffer = {0};
    
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
                for(int Index = 0; Index < Length && TextBuffer.Cursor[0] < MaxChars; ++Index, ++TextBuffer.Cursor[0])
                {
                    TextBuffer.Lines[TextBuffer.Cursor[1]][TextBuffer.Cursor[0]] = Event.text.text[Index];
                }
            }

            if(Event.type == SDL_KEYDOWN)
            {
                switch(Event.key.keysym.sym)
                {
                    case SDLK_RETURN: { HandleNewLine(&TextBuffer); } break;
                    case SDLK_BACKSPACE: { } break;
                    case SDLK_UP: { HandleUpArrow(&TextBuffer); } break;
                    case SDLK_DOWN: { HandleDownArrow(&TextBuffer); } break;
                    case SDLK_LEFT: { HandleLeftArrow(&TextBuffer); } break;
                    case SDLK_RIGHT: { HandleRightArrow(&TextBuffer); } break;
                }
            }

#if 0
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
#endif
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
                    int PX = TextBuffer.Cursor[0] * 8 * FontScale + X;
                    int PY = TextBuffer.Cursor[1] * 8 * FontScale + Y;
                    DrawPixel(&Screen, PX, PY, 0xffffffff);
                }
            }
        }

        for(int Line = 0; Line < MaxLines; ++Line)
        {
            DrawString(&Screen, 0, Line * 8 * FontScale, TextBuffer.Lines[Line]);
        }
        SDL_UnlockSurface(ScreenSurface);

        

        SDL_UpdateWindowSurface(Window);
        SDL_Delay(10);
        printf("Cursor Position: %d %d\n", TextBuffer.Cursor[0], TextBuffer.Cursor[1]);


    }

    SDL_Quit();

    return 0;
}