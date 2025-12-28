#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <pthread.h>

#define szerokoscWidzenia 1

int turnOn = 1;

int tablica[20][20];

int tablicaDoRenderowania[20][20][3];


int tablica2D[20][20][3];

float gracz[3] = {100,100, 4.71 };

float renderingMode = 1;


int nygasColors[2];

int inputNaLiczby[2][2] =
{
    {SDLK_w,SDLK_s},
    {1   ,-1 }
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

void * obliczeniaMatematyczne() {
    while (turnOn) {
        for (int i = 0; i < 20; i++ ) {for (int j = 0; j < 20; j++ ) {for (int z = 0; z < 3; z++) {tablica2D[i][j][z] = 0;}}}

        for (int i = -10; i < 10; i++)
        {
            float pozycjeStartowe[2] = {gracz[0], gracz[1]};
            float colorDoTablicy = 255;

            float stepX = (float)cos(gracz[2] + ((float)i/2 * 0.1));
            float stepY = (float)sin(gracz[2] + ((float)i/2 * 0.1));


            int wysokoscTimer = 1;

            while (1) {

                if (pozycjeStartowe[0] >= 200 || pozycjeStartowe[1] >= 200 || pozycjeStartowe[0] <= 0 || pozycjeStartowe[1] <= 0 || colorDoTablicy < 0   ) { colorDoTablicy = 0;  break;}
                else if (tablica[ (int)floor(pozycjeStartowe[0]/10) ][  (int)floor(pozycjeStartowe[1]/10) ] != 0  ) { break;}

                pozycjeStartowe[0] += stepY;
                pozycjeStartowe[1] += stepX;

                colorDoTablicy+= -2;

                for (int y = 0; y < 3; y++) {tablica2D[(int)pozycjeStartowe[0]/10][(int)pozycjeStartowe[1]/10][y] = colorDoTablicy;}
                if (wysokoscTimer < 200) { wysokoscTimer++;}
            }
            int wysokosc = ((wysokoscTimer / 10) - 20 ) * -1 +1;
            for (int y = 0; y < 20; y++) { for (int z = 0; z < 3; z++) {tablicaDoRenderowania[y][(i+10)][z] = 0;}}

            for (int y = (20 - wysokosc)/2 ; y < (20 + wysokosc)/2; y++) { for (int z = 0; z < 3; z++) {tablicaDoRenderowania[y][(i+10)][z] = colorDoTablicy;}}
        }

        for (int y = 0; y < 20; y++) {
            for (int x = 0; x < 20; x++)
            {
                if (tablica[y][x] == 1) {
                    tablica2D[y][x][0] = 209;
                    tablica2D[y][x][1] = 195;
                    tablica2D[y][x][2] = 0;
                }
            }
        }
        tablica2D[ (int)gracz[0]/10 ][(int)gracz[1]/10][0] = 255;
        tablica2D[ (int)gracz[0]/10 ][(int)gracz[1]/10][1] = 0;
        tablica2D[ (int)gracz[0]/10 ][(int)gracz[1]/10][2] = 0;



        SDL_Delay(80);
    }
}
void *renderowanie() {
    SDL_Window* meowOkno = SDL_CreateWindow ("FajneOkno",800,800,800,800,SDL_WINDOW_SHOWN);
    SDL_Renderer* meowRender = SDL_CreateRenderer (meowOkno, -1, SDL_RENDERER_ACCELERATED);
    SDL_Event meowEvent;
    SDL_RenderSetLogicalSize(meowRender,20,20);

    while (turnOn) {

        //tablica na render

        for (int x = 0; x < 20; x++) {
            for (int y = 0; y < 20; y++) {
                if (renderingMode == 1) {
                    SDL_SetRenderDrawColor(meowRender,tablicaDoRenderowania[y][  x ][0],tablicaDoRenderowania[y][x][1],tablicaDoRenderowania[y][x][2],0  );
                    SDL_RenderDrawPoint(meowRender,x,y);
                }
                else {
                    //SDL_SetRenderDrawColor(meowRender, tablica[y][x]*120, tablica[y][x]*120,tablica[y][x]*120,0);
                    SDL_SetRenderDrawColor(meowRender, tablica2D[y][x][0], tablica2D[y][x][1],tablica2D[y][x][2], 90 );
                    SDL_RenderDrawPoint(meowRender,x,y);

                }

            }
        }

        while (SDL_PollEvent(&meowEvent)) {
            if (meowEvent.type == SDL_QUIT ) { turnOn = 0; }
            else if (meowEvent.type == SDL_KEYDOWN)
            {
                if (meowEvent.key.keysym.sym == SDLK_ESCAPE ) {turnOn = 0;}
                else if (meowEvent.key.keysym.sym == SDLK_a) {gracz[2] -= 0.20; if (gracz[2] < 0) { gracz[2] = 6.283;   };}
                else if (meowEvent.key.keysym.sym == SDLK_d) {gracz[2] += 0.20;   if (gracz[2] > 6.3) { gracz[2] = 0;   };}
                else if (meowEvent.key.keysym.sym == SDLK_e) { nowyKlocek();   }
                else if (meowEvent.key.keysym.sym == SDLK_r) { renderingMode *= -1; }
                for (int x = 0; x < 2; x++) {
                    if (meowEvent.key.keysym.sym == inputNaLiczby[0][x]) {
                        int mango67One = (int)((gracz[0] + sin(gracz[2]) * inputNaLiczby[1][x]        ) / 10 ) ;
                        int mango67Two = (int)((gracz[1] + cos(gracz[2]) * inputNaLiczby[1][x]   ) / 10 )  ;

                        if (!tablica[mango67One][mango67Two] ) {
                            gracz[0] += (float)sin(gracz[2]) * inputNaLiczby[1][x] ;
                            gracz[1] += (float)cos(gracz[2]) * inputNaLiczby[1][x] ;
                            break;
                        }
                    }
                }
                printf("%d%c%d%c%f\n",(int)gracz[0],' ',(int)gracz[1],' ',gracz[2]);
            }
        }
        SDL_RenderPresent(meowRender);
        SDL_Delay(16);
    }
}

int main() {
    for (int y = 0; y < 20; y++) { for (int x = 0; x < 20; x++) { tablica[y][x] = 0; }}
    tablica[5][10] = 1;
    tablica[15][10] = 1;
    pthread_t matematyka;
    pthread_t wyswietlanie;

    pthread_create(&matematyka, NULL, obliczeniaMatematyczne, NULL);
    pthread_create(&wyswietlanie, NULL, renderowanie, NULL);

    pthread_detach(matematyka);
    pthread_detach(wyswietlanie);

    while (turnOn) { SDL_Delay(250);  }


}
