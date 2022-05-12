#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include<SDL.h>


int main(int argc, char * argv[]){

    uint32_t address1=0x120f0;
    uint32_t address2=0xA3F0;
    uint32_t palette_address=0x26b0a;
    uint32_t count=0;
    FILE *file_ptr=fopen("C:\\Users\\Sudip\\Music\\dave.exeNEW","rb");
    if(file_ptr==NULL){
        printf("File cannot be opened. ");
        exit(1);
    }

    fseek(file_ptr,address1,SEEK_SET);
    /*
    uint32_t count=fgetc(file_ptr);
    uint32_t count1=fgetc(file_ptr);
    uint32_t count2=fgetc(file_ptr);
    uint32_t count3=fgetc(file_ptr);
    uint32_t count4=fgetc(file_ptr);
    printf("the value of count1 %d \n the value of count 2 %d \n count 3 %d \n count 4 %d \n count4 %d",count,count1,count2,count3,count4);
    */
    count |= fgetc(file_ptr);
    count |= fgetc(file_ptr)<<8;
    count |= fgetc(file_ptr)<<16;
    count |= fgetc(file_ptr)<<24;


    //printf("%d",count);
    uint32_t current_length=0;
    uint8_t final_file[100000]={0};
    uint8_t buffer;
    /*for(int i=0;i<20;i++){
        buffer=fgetc(file_ptr);
        printf("buffer is %d",buffer);
    }
    printf("\n");*/
    /*buffer=fgetc(file_ptr);
    //printf("buffer is %d",buffer);*/
    while(current_length < count){
      buffer=fgetc(file_ptr);
      //printf("\nputi is buffer%d\n",buffer);
        if(buffer>=128){
            while((buffer-127)>0){
             final_file[current_length++]=fgetc(file_ptr);
             buffer--;
            //printf("\nputi1 is buffer%d\n",buffer);
            }

        }
        else{
          uint8_t next=fgetc(file_ptr);
          //printf("\nlado is buffer%d\n",buffer);
          int i=buffer+3;
          while(i>=1){
            final_file[current_length++]=next;
            i--;
          }
        }

    }
    //for(int i=0;i<50sizeof(final_file);i++){
        //printf("%d ",final_file[950]);
    //}*/
    uint8_t palette[768];
    fseek(file_ptr,palette_address,SEEK_SET);
    for(int i=0;i<256;i++){
        for(int j=0;j<3;j++){
            palette[i*3+j]=fgetc(file_ptr);
            palette[i*3+j]<<=2;
        }
    }
    uint32_t tile_count;
    tile_count  = final_file[0];
    tile_count |= final_file[1]<<8;
    tile_count |= final_file[2]<<16;
    tile_count |= final_file[3]<<24;

    printf("tile count is%d\n",tile_count);

    uint32_t tile_index[tile_count];
    for(int i=0;i<tile_count;i++){
        tile_index[i] =final_file[i*4+4];
        tile_index[i] |=final_file[i*4+5]<<8;
        tile_index[i] |=final_file[i*4+6]<<16;
        tile_index[i] |=final_file[i*4+7]<<24;
    }
    int size=tile_index[1]-tile_index[0]+1;
    //printf("the tile information is %d\n",size);

    struct tile_information{
      uint8_t tile_list[size];
    } tiles[tile_count];
    SDL_Surface * surface;
    uint8_t *pointer;
    pointer=(uint8_t*)surface->pixels;
    //for(int i=53; i<tile_count;i++){
	int i=53;
        surface=SDL_CreateRGBSurface(0,20,16,32,0,0,0,0);
        uint32_t current_tile=tile_index[i];
        uint32_t next_tile=tile_index[i+1];
        int counter=0;
        uint8_t red_buffer=0;
        uint8_t green_buffer=0;
        uint8_t blue_buffer=0;
        while(current_tile<next_tile){
                tiles[i].tile_list[counter]=final_file[tile_index[i]+counter];
                red_buffer=palette[tiles[i].tile_list[counter]*3];
                green_buffer=palette[tiles[i].tile_list[counter]*3+1];
                blue_buffer=palette[tiles[i].tile_list[counter]*3+2];
                pointer[counter*4]=blue_buffer;
                pointer[counter*4+1]=green_buffer;
                pointer[counter*4+2]=red_buffer;
                pointer[counter*4+3]=0xff;
                counter++;
                current_tile++;

        }
        //create a file name for the tiles
        char file_name[20];
        file_name[0]='\0';
        char file_num[4];
        strcat(file_name,"tile");
        sprintf(&file_num[0],"%u",i);
        strcat(file_name,file_num);
        strcat(file_name,".bmp");

        SDL_SaveBMP(surface,file_name);
        SDL_FreeSurface(surface);

    //}
    //for(int i=0;i<257;i++){
      //  printf("values at %d",tiles[0].tile_list[i]);
    //}
    //for(int i=0;i<158;i++){
      //  for(int j=0;j<size;j++) {

        //}
    //}
    //for(int i=0;i<10;i++)
      //  printf("the buffers are %d, %d, %d",red_buffer,green_buffer,blue_buffer);
 return 0;
}

