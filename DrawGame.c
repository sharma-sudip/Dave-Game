#include<stdio.h>
#include<SDL2/SDL.h>
#define SDL_MAIN_HANDLED

struct level_data{
    uint8_t path[256];
    uint8_t tiles[1000];
    uint8_t padding[24];
};

struct game_data{
    struct level_data level[10];
    uint8_t dave_x;
    uint8_t dave_y;
};
struct game_assets{
    SDL_Texture * asset_pointers[60];
};


//Function prototypes
void renderworld(SDL_Renderer *, struct level_data *, struct game_data *,struct game_assets*);
void init_level(struct game_data*);
void init_assets(struct game_assets*,SDL_Renderer*);
void drawDave(struct game_data*,SDL_Renderer*);



int main(int argc, char *argv[]){
    SDL_Event event;
    struct game_assets* assets;
    struct level_data *level;
    struct game_data *game;
    SDL_Window * window;
    SDL_Renderer *renderer;

    level=malloc(sizeof(struct level_data));
    assets=malloc(sizeof(struct game_assets));
    game=malloc(sizeof(struct game_data));
    if(SDL_Init(SDL_INIT_VIDEO)){
            SDL_Log("SDL Error: %s",SDL_GetError());
    }
    if(SDL_CreateWindowAndRenderer(960,600,0,&window,&renderer)){
        SDL_Log("SDL Error: %s",SDL_GetError());
    }
    SDL_RenderSetScale(renderer,3,3);
    SDL_SetRenderDrawColor(renderer, 0xaa, 0x0a, 0xb0, 0x0d);
    SDL_RenderClear(renderer);
    init_assets(assets,renderer);
    init_level(game);
    unsigned int time_elapsed=SDL_GetTicks();
    printf("the time elapsed is %d",time_elapsed);
    while(1){
        SDL_PollEvent(&event);
        if(event.type==SDL_QUIT){
            break;
        }
        renderworld(renderer,level,game,assets);
        drawDave(game,renderer);
        SDL_Delay(30);
    }
}
void init_level(struct game_data* game){
    FILE * file_ptr;
    for(int i=0;i<10;i++){
        char write_file_name[20];
        write_file_name[0]='\0';
        char write_file_number[4];
        strcat(write_file_name,"level");
        sprintf(&write_file_number[0],"%u",i);
        strcat(write_file_name,write_file_number);
        strcat(write_file_name,".dat");
        file_ptr=fopen(write_file_name,"rb");
        //printf("the level data is:%s\n",write_file_name);
        for(int j=0;j<sizeof(game->level[i].path);j++){
            game->level[i].path[j]=fgetc(file_ptr);
        }
        for(int j=0;j<sizeof(game->level[i].tiles);j++){
            game->level[i].tiles[j]=fgetc(file_ptr);
        }
        for(int j=0;j<sizeof(game->level[i].padding);j++){
            game->level[i].padding[j]=fgetc(file_ptr);
        }
    }
    fclose(file_ptr);
}
void init_assets(struct game_assets* assets,SDL_Renderer * renderer){
    for(int i=0;i<60;i++){
        char write_file_name[20];
        write_file_name[0]='\0';
        char write_file_number[4];
        //strcat(write_file_name,"tile");
        sprintf(&write_file_number[0],"%u",i);
        strcat(write_file_name,write_file_number);
        strcat(write_file_name,".bmp");
        printf("the name of file is: %s\n",write_file_name);
        SDL_Surface* surface=SDL_LoadBMP(write_file_name);
        assets->asset_pointers[i]=SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
}
void renderworld(SDL_Renderer * renderer, struct level_data * level, struct game_data * game,struct game_assets* assets){
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(renderer);
	SDL_Rect rect;
    uint8_t tile_index;
    //for(int k=0;k<10;k++){
        for(int i=0;i<10;i++){
                rect.y=i*16;
                rect.w=16;
                rect.h=16;
                for(int j=0;j<20;j++){
                    rect.x=j*16;
                    tile_index = game->level[0].tiles[100*i+j];
                    //printf("tile index:%d rect x: %d rect y:%d\n",tile_index,rect.x,rect.y);
                    if(tile_index>59){
                        tile_index=0;
                    }
                    if(SDL_RenderCopy(renderer,assets->asset_pointers[tile_index], NULL ,&rect)<0){
                         printf("SDL_Init failed: %s\n", SDL_GetError());
                    }
                }
        }
        SDL_RenderPresent(renderer);

    //}
}
void drawDave(struct game_data* game,SDL_Renderer* renderer){
    SDL_Surface* surface=SDL_LoadBMP("56.bmp");
    SDL_Texture * texture=SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect rect;
    rect.x=32;
    rect.y=128;
    if(SDL_RenderCopy(renderer,texture, NULL ,&rect)<0){
        printf("SDL_Init failed: %s\n", SDL_GetError());
    }
    SDL_RenderPresent(renderer);

}

