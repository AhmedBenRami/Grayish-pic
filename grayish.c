#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

typedef struct
{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} Pixel;

int main(int argc, char *argv[])
{

    FILE *origin = fopen(argv[1], "rb");
    FILE *copy = fopen("copy.bmp", "wb");

    if (origin == NULL || copy == NULL)
    {
        printf("ERROR OCCURED DURING OPENNING FILES");
        return 1;
    }
    long int width;
    long int height;
    fseek(origin, 18, SEEK_SET);
    fread(&width, sizeof(int), 1, origin);
    fread(&height, sizeof(int), 1, origin);
    fseek(origin, 0, SEEK_SET);

    uint8_t header[54];

    fread(header, sizeof(uint8_t), sizeof(header), origin);
    fwrite(header, sizeof(uint8_t), sizeof(header), copy);

    int padding = 4 - (width % 4);
    Pixel pixel;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width + padding; j++)
        {
            if (j < width)
            {
                fread(&pixel, sizeof(Pixel), 1, origin);
                int luminance = round(0.2126 * pixel.red + 0.7152 * pixel.green + 0.0722 * pixel.blue);
                pixel.blue = luminance;
                pixel.green = luminance;
                pixel.red = luminance;
                fwrite(&pixel, sizeof(Pixel), 1, copy);
            }
            else
            {
                fread(&pixel, sizeof(Pixel), 1, origin);
                fwrite(&pixel, sizeof(Pixel), 1, copy);
            }
        }
    }

    fclose(origin);
    fclose(copy);
}
