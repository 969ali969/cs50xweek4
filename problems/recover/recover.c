#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef uint8_t BYTE;

#define block_size 512
#define file_size 8

//Header of new JPEG.
int JPEG(BYTE buffer[])
{
    if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
    {
        return 1;
    }
    return 0;
}


int main(int argc, char *argv[])
{
    //Open the file AND if it cant be open
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("cant find the File\n");
        return 1;
    }

    //buffer we need for filenmae
    BYTE buffer[block_size];
    char filename[file_size];
    FILE *output;
    int counter = 0;

    //once first JPEG found, all other are adjacent
    while (fread(buffer, block_size, 1, input) == 1)
    {
        if (JPEG(buffer) == 1)
        {
            if (counter != 0)
            {
                fclose(output);
            }
            sprintf(filename, "%03i.jpg", counter++);
            output = fopen(filename, "w");
            fwrite(buffer, block_size, 1, output);
        }
        //next blocks of rn JPEG, write till we find next Header.
        else if (counter > 0)
        {
            fwrite(buffer, block_size, 1, output);
        }
    }
    //Close all file used.
    fclose(input);
    fclose(output);
}
