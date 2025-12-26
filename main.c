#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>

#define szerokoscWidzenia 1

int turnOn = 1;

int tablica[20][20];

int tablicaDoRenderowania[10][10][3];

float gracz[3] = {100,100, 4.71 };

int inputNaLiczby[3][4] =
{
    {SDLK_w,SDLK_s,SDLK_a,SDLK_d},
    {-1   ,1   , 0,     0},
    {0,        0,  -1, 1},
};

void nowyKlocek() {
    int nowyX;
    int nowyY;
    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 20; x++) {printf("%d ",tablica[y][x]);}
        printf("\n");}

    printf("\n%s\n","Wartość x: ");
    scanf("%d",&nowyX);
    system("clear");
    printf("\n%s\n","Wartość y: ");
    scanf("%d",&nowyY);

    tablica[nowyY][nowyX] = 1;

}

int main() {
    for (int y = 0; y < 20; y++) { for (int x = 0; x < 20; x++) { tablica[y][x] = 0; }}
    tablica[5][10] = 1;

    tablica[15][10] = 1;

    SDL_Window* meowOkno = SDL_CreateWindow ("FajneOkno",800,800,800,800,SDL_WINDOW_SHOWN);
    SDL_Renderer* meowRender = SDL_CreateRenderer (meowOkno, -1, SDL_RENDERER_ACCELERATED);
    SDL_Event meowEvent;
    SDL_RenderSetLogicalSize(meowRender,10,10);

    while (turnOn) {
        //do tablicy

        for (int i = -5; i < 5; i++)
        {
            float pozycjeStartowe[2] = {gracz[0], gracz[1]};
            float colorDoTablicy = 255;

            float stepX = cos(gracz[2] + (i * 0.1));
            float stepY = sin(gracz[2] + (i * 0.1));

            int timer = 0;
            while (1) {

                if (pozycjeStartowe[0] >= 200 || pozycjeStartowe[1] >= 200 || pozycjeStartowe[0] <= 0 || pozycjeStartowe[1] <= 0 || colorDoTablicy >= 255 + 2   ) { colorDoTablicy = 0;  break;}
                else if (tablica[ (int)round(pozycjeStartowe[0]/10) ][  (int)round(pozycjeStartowe[1]/10) ] != 0  ) { break;}

                if (timer) {pozycjeStartowe[0] += stepY;}
                else { pozycjeStartowe[1] += stepX ;}
                colorDoTablicy+= -2;
                timer++;
                if (timer > 1) { timer = 0;}
            }
            for (int y = 0; y < 10; y++) { for (int z = 0; z < 3; z++) {tablicaDoRenderowania[y][(i+5)][z] = colorDoTablicy;}}
        }


        //tablica na render
        for (int x = 0; x < 10; x++) {
            for (int y = 0; y < 10; y++) {
                SDL_SetRenderDrawColor(meowRender,tablicaDoRenderowania[y][x][0],tablicaDoRenderowania[y][x][1],tablicaDoRenderowania[y][x][2],0  );
                SDL_RenderDrawPoint(meowRender,x,y);
            }
        }

        while (SDL_PollEvent(&meowEvent)) {
            if (meowEvent.type == SDL_QUIT ) { turnOn = 0; }
            else if (meowEvent.type == SDL_KEYDOWN)
            {
                if (meowEvent.key.keysym.sym == SDLK_ESCAPE ) {turnOn = 0;}
                else if (meowEvent.key.keysym.sym == SDLK_LEFT) {gracz[2] -= 0.30; if (gracz[2] < 0) { gracz[2] = 6.283;   };}
                else if (meowEvent.key.keysym.sym == SDLK_RIGHT) {gracz[2] += 0.30;   if (gracz[2] > 6.283) { gracz[2] = 0;   };}
                else if (meowEvent.key.keysym.sym == SDLK_e) { nowyKlocek();   }
                for (int x = 0; x < 4; x++) {
                    if (meowEvent.key.keysym.sym == inputNaLiczby[0][x]) {
                        int mango67One = (int)((gracz[0] + (float)inputNaLiczby[1][x]/2)/10);
                        int mango67Two = (int)((gracz[1] + (float)inputNaLiczby[2][x]/2)/10);

                        if (!tablica[mango67One][mango67Two] ) {

                            gracz[0] += (float)inputNaLiczby[1][x]/2;
                            gracz[1] += (float)inputNaLiczby[2][x]/2;
                            printf("%f%c%f%c%f\n",gracz[0],' ',gracz[1],' ',gracz[2]);
                            break;
                        }
                    }
                }
            }
        }
        SDL_RenderPresent(meowRender);
        SDL_Delay(16);
    }

}