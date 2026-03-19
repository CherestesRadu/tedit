const float FontScale = 2.0f;

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

void DrawCharScaled(screen *Screen, int X, int Y, char Character)
{
    for(int Row = 0; Row < 8 * FontScale; ++Row)
    {
        for(int Col = 0; Col < 8 * FontScale; ++Col)
        {
            int SourceX = (int)(Col / FontScale);
            int SourceY = (int)(Row / FontScale);

            uint32_t Pixel = (font8x8_basic[Character][SourceY] >> SourceX) & 1;
            if(Pixel)
            {
                DrawPixel(Screen, Col + X, Row + Y, RGBA2BGRA(255, 255, 255, 255));
            }
        }
    }
}

void DrawString(screen *Screen, int X, int Y, char *String)
{
    while(*String)
    {
        DrawCharScaled(Screen, X, Y, *String);
        X += (int)(8 * FontScale);
        String++;
    }
}