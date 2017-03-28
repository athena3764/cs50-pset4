/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(void)
{

    // open input file 
    FILE* input = fopen("card.raw", "r");
    
    //if file doesn't exist, print an error message and terminate the program.
     if (input == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", "card.raw");
        return 1;
    }
    
    //create a buffer array that contains the block size of 512 bytes.
    BYTE buffer[512];
    
    //Define the image file.
    FILE* img = NULL;
    
    int count = 0;
    
    //Read block of bytes specified by the pointer from stream.
     while (fread(buffer, sizeof(buffer), 1, input))
    
    {
        //JPEGs have "signatures," patterns of bytes that can distinguish them from other file formats.
       if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] == 0xe0 || buffer[3] == 0xe1))
       
       {   
           //if all images are recovered, close the program.
           if(img != NULL){
               
               fclose(img);
               
               img = NULL;
               
           }
           
           //create a filename array that contains 8 bytes.
           char fname[8];
          
           //Write formatted data to string.
           sprintf(fname, "%03d.jpg", count);
           count++;
           
           //open the image file
           img = fopen(fname, "a");
           
           //if file doesn't exist, close the program.
           if (img == NULL)
           {
               fprintf(stderr,"Could not open %s.\n", fname);
               return 1;
               
           }
           
           //writes a block of bytes to the current position in the stream.
           fwrite(buffer, sizeof(buffer), 1, img);
           
       } 
       //if not all images are recovered, writes the remaining block of bytes to the current position.
       else if (img != NULL)
      {
          fwrite(buffer, sizeof(buffer), 1, img);        
      } 

}

        //close the program.
        if (input != NULL){
    
        fclose(input);
 
        }
       
        //the exit code  
        
        return 0;
   
}
