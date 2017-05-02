#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


typedef uint8_t  BYTE;

int main(int argc, char *argv[])
{
    //ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover memCard");
        return 1;
    }
    
    //remeber file name
    char *memCard = argv[1];
    
    //open the file
    FILE *raw_file = fopen(memCard, "r");
    if (raw_file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", memCard);
        return 2;
    }
    
    //initialize buffer array 
    BYTE buffer[512];
    
    char filename[8];  
    int count = 0;
    
    FILE *img = NULL;
    
    //read 512 bytes into a buffer
    while(fread (buffer, sizeof(buffer), 1, raw_file) == 1)
    {
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
           (buffer[3] & 0xf0) == 0xe0)
        {
            if (img != NULL)
            {
                fclose(img);
            }
            sprintf(filename, "%03i.jpg", count);
            //open and write to a new file
            img = fopen(filename, "w");
            fwrite(buffer, 512, 1, img); 
            count++;
            
        }
        else if (img != NULL)
        {
            //keep writting to the same file
            fwrite(buffer, 512, 1, img); 
        }
        
    }
    if (img != NULL)
    {
        fclose(img);
    }
    fclose(raw_file);
    return 0;
}
