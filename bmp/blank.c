/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */
#include <stdio.h>
#include <stdint.h>

int main(void)
{

    // open input file 
    FILE* input = fopen("card.raw", "r");
    
     if (input == NULL)
    {
        printf("Could not open %s.\n", "card.raw");
        return 1;
    }
    
 
    
    uint8_t buffer[512];
    FILE* img = NULL;
   
    int count = 0;
   
     while (fread(&buffer, sizeof(buffer), 1, input))
    
    
    {
       
       
       if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] == 0xe0 || buffer[3] == 0xe1))
       
       {
           
           char title[10];
           
           sprintf(title, "%03d.jpg", count);
 
            img = fopen(title, "a");
           
            if (img != NULL)
            {
                fclose(img);
            }
            
      
              
 
           fwrite(&buffer, sizeof(buffer), 1, img);
                        

           count++;
       }
       
       
}
        // close infile
    
        fclose(input); 
       
    // that's all folks
        
        return 0;
   
    // TODO
}
