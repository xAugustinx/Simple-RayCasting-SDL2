#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>

#define rozdzielczosc 300
#define windowRealResolutnion 800

#define distanceOfWork 18

#define standardowePrzyspieszeniePocisku -0.05;
#define standardowaPredkoscPocisku 2
#define timeOf 16

#define promienDlugosc 2.0f

int long id = 0;

int czySmiercOdWrogow = 1;
int poprzedniRuchMyszka = 0;

short czyDoszloDoZmianyWysokosci[] = {-2,0,0};


unsigned char czySkaskowacLinie = 0;

int turnOn = 1;
unsigned char tablicaDoRenderowania[rozdzielczosc][rozdzielczosc][3];
unsigned char tablica2D[rozdzielczosc][rozdzielczosc][3];

typedef struct {short pierwszeX; short pierwszyY; short drugiX; short drugiY; unsigned char kolor;  } linia;
typedef struct {float y; float x; unsigned char type; long int id; float kierunek;  } obiekt ;

obiekt * tablicaElementowNaPlanszy;
linia * tablicaLiniWygladzanieKrawedzi; // pierwszy element x to jest ilosc elementow

int liczbaElementowNaCalejTablicy = 5;
int liczbaWszystkichLini = 1;

typedef struct  {float przyspieszenie;float predkosc;float kierunek;float y;float x; int time; char mode; } pocisk;
pocisk pociskiZBroni[64];
unsigned char wyznaczonyPocisk = 0;
SDL_Rect femboyTable[64][2];

pocisk pustyPocisk = {0,0,0,-2,-2, -1};

char rozmiarFemboyTable = 0;
char czyTexture = 0;
int miejsceNaTeksture[7] = {0,0, 0,0, -1,-1,0  };  //wysokosc start, początek lewo, wysokosc start, początek lewo, lokacja Spotkania y Lokacja spotkania x
char kopiowanieBoolDlaRenderu = 0;
SDL_Texture* femboyImage;
float gracz[3] = {100,100, 4.71 };
float renderingMode = 1;

int nygasColors[2];
int inputNaLiczby[2][2] = {{SDLK_w,SDLK_s},{1   ,-1 }};
int typeOf = 1;

SDL_KeyCode listaZnakow[10] = {SDLK_0,SDLK_1,SDLK_2,SDLK_3,SDLK_4,SDLK_5,SDLK_6,SDLK_7,SDLK_8,SDLK_9 };

unsigned char czyPostawicKloca = 0;
int licznikDo3 = 0;
int czyDodacnowyPociskNaListe = 0;

pocisk nowyPocisk;

void usunieciePaska(){
    for (int y = 0; y < rozdzielczosc; y++){
        for (unsigned char z = 0; z < 3; z++) {
            //tablicaDoRenderowania[(int)(rozdzielczosc/2) ][ y][z] = 0;
        }
    }
}


int nowyNaListe(int yD, int xD, int typ) {
    liczbaElementowNaCalejTablicy++;
    id++;
    obiekt *tymczasowy = realloc(tablicaElementowNaPlanszy, liczbaElementowNaCalejTablicy * sizeof(obiekt));
    tablicaElementowNaPlanszy = tymczasowy;
    obiekt tymczasowyObiektMeow = {(float)yD,(float)xD,typ,id,-1  };
    tablicaElementowNaPlanszy[liczbaElementowNaCalejTablicy -1] = tymczasowyObiektMeow;
}

int licznikDo3F() {
    licznikDo3++;
    if (licznikDo3 > 13) {licznikDo3 = 0;}
    return licznikDo3;
}


int czyWartoscJestNaLiscie(int yD, int xD ) {
    int czyZwrocicFalse = 0;

    for (int i = 1; i <  liczbaElementowNaCalejTablicy; i++) {
        if ((int)tablicaElementowNaPlanszy[i].y == yD && (int)tablicaElementowNaPlanszy[i].x == xD     ) {
            return tablicaElementowNaPlanszy[i].type;
        }
    }
    return 0;
}
int usuwanieZListy(int yD, int xD  ) {
    int poszukiwany = -1;

    for (int i = 1; i <  liczbaElementowNaCalejTablicy; i++)
    {
        if ((int)tablicaElementowNaPlanszy[i].y == yD && (int)tablicaElementowNaPlanszy[i].x == xD   )
        {poszukiwany = i;break;}
    }
    if (poszukiwany == -1) {return 0;}

    for (int i = poszukiwany; i <  liczbaElementowNaCalejTablicy-1; i++) {
        tablicaElementowNaPlanszy[i] = tablicaElementowNaPlanszy[i+1];
    }
    liczbaElementowNaCalejTablicy--;

    obiekt *tymczasowy = realloc(tablicaElementowNaPlanszy, liczbaElementowNaCalejTablicy * sizeof(obiekt));
    tablicaElementowNaPlanszy = tymczasowy;

    return poszukiwany;
}
int budowanie(int z) {
    int nowyY = (int)gracz[0]/10 + sin(gracz[2]) * 3;
    int nowyX = (int)gracz[1]/10 + cos(gracz[2]) * 3;
    nowyNaListe(nowyY,nowyX, z);
}

int nowyPociskFunkcja(char mango) {
    pocisk nowyPociskMeow =
    {standardowaPredkoscPocisku, standardowaPredkoscPocisku, gracz[2], gracz[0],  gracz[1],0, mango};
    nowyPocisk = nowyPociskMeow;
    czyDodacnowyPociskNaListe = 1;
}

void przesowanieFerisow() {
    for (int i = 1; i <  liczbaElementowNaCalejTablicy; i++) {

        if (tablicaElementowNaPlanszy[i].type == 3 && tablicaElementowNaPlanszy[i].y < gracz[0]/10+distanceOfWork
            && tablicaElementowNaPlanszy[i].y > gracz[0]/10-distanceOfWork
            && tablicaElementowNaPlanszy[i].x < gracz[1]/10+distanceOfWork
            && tablicaElementowNaPlanszy[i].x > gracz[1]/10-distanceOfWork) {
            double sinY =  tablicaElementowNaPlanszy[i].y + sin(tablicaElementowNaPlanszy[i].kierunek);
            double cosX = tablicaElementowNaPlanszy[i].x + cos(tablicaElementowNaPlanszy[i].kierunek);

            if (!(rand() % 34) || tablicaElementowNaPlanszy[i].kierunek == -1  ) {
                tablicaElementowNaPlanszy[i].kierunek = (float)((rand() % 628) / 100);}

            int fajnyBoolMeow = true;

            if ( (int)(gracz[0]/10) == (int)sinY && (int)(gracz[1]/10) == (int)cosX && !czySmiercOdWrogow )
                {fajnyBoolMeow = false;}

            if ((czyWartoscJestNaLiscie( (int)sinY,(int)cosX) == 0 || czyWartoscJestNaLiscie( (int)sinY,(int)cosX) == 3) && fajnyBoolMeow) {

                if (sinY < 1 || sinY > rozdzielczosc-1 || cosX > rozdzielczosc-1 || cosX < 1 ) {
                    tablicaElementowNaPlanszy[i].kierunek*=-1;
                    tablicaElementowNaPlanszy[i].kierunek+=6.28;
                }
                else{
                    tablicaElementowNaPlanszy[i].y = sinY;
                    tablicaElementowNaPlanszy[i].x = cosX;
                }
            }
        }
    }
}

void * obliczeniaMatematyczne() {
    SDL_Rect musztardaDymczasowa = { -1, -1, -1, -1 };
    for (char i = 0; i < 64; i++ ) {femboyTable[i][0] = musztardaDymczasowa; pociskiZBroni[i] = pustyPocisk; }
    rozmiarFemboyTable = 0;

    pocisk pocisk67 = {4,6,7,4,3,2};
    wyznaczonyPocisk = 0;


    while (turnOn) {
        unsigned char listaWyznaczonychDoUsuniecia[64];
        char ktoryZaznaczyc = 0;

        if (czySkaskowacLinie) {
            liczbaWszystkichLini = 1;
            linia * nyaUwu = realloc(tablicaLiniWygladzanieKrawedzi, liczbaWszystkichLini * sizeof(linia));
            tablicaLiniWygladzanieKrawedzi = nyaUwu;
        }
        czySkaskowacLinie = 1;

        for (char i = 0; i < 64; i++) {
            if (  pociskiZBroni[i].x <= rozdzielczosc*10 &&  pociskiZBroni[i].y <= rozdzielczosc*10  && pociskiZBroni[i].x >= 0 &&  pociskiZBroni[i].y >= 0 && pociskiZBroni[i].predkosc > 0  ) {pociskiZBroni[i].time += 1;}
        }
        for (char i = 0; i < wyznaczonyPocisk; i++) {
            if (  pociskiZBroni[i].x < 0 || pociskiZBroni[i].y < 0 || pociskiZBroni[i].predkosc <= 0 || pociskiZBroni[i].x > rozdzielczosc || pociskiZBroni[i].y > rozdzielczosc  ) {
                listaWyznaczonychDoUsuniecia[ktoryZaznaczyc] = i;
                ktoryZaznaczyc++;
                wyznaczonyPocisk--;

                if (pociskiZBroni[i].predkosc <= 0 && pociskiZBroni[i].mode == 1) {
                    nowyNaListe((int)pociskiZBroni[i].y/10,(int)pociskiZBroni[i].x / 10, 1);
                }
            }
        }
        for (char i = ktoryZaznaczyc-1; i >= 0; i--) {
            for (char j = listaWyznaczonychDoUsuniecia[i]; j < 63; j++ ) {pociskiZBroni[j] = pociskiZBroni[j+1];}
        }

        if (czyDodacnowyPociskNaListe) {
            pociskiZBroni[wyznaczonyPocisk] = nowyPocisk;
            wyznaczonyPocisk++;
            czyDodacnowyPociskNaListe = 0;
        }
        for (char i = 0; i < wyznaczonyPocisk; i++) {
            pociskiZBroni[i].y += sin(pociskiZBroni[i].kierunek) * pociskiZBroni[i].predkosc;
            pociskiZBroni[i].x += cos(pociskiZBroni[i].kierunek) * pociskiZBroni[i].predkosc;
            pociskiZBroni[i].predkosc += pociskiZBroni[i].przyspieszenie;
        }

        rozmiarFemboyTable = 0;
        int czyZnalazlSieDwaWOguleWcalym = 0;

        usunieciePaska();

        for (int i = 0; i < rozdzielczosc; i++ ) {for (int j = 0; j < rozdzielczosc; j++ ) {for (int z = 0; z < 3; z++) {tablica2D[i][j][z] = 0;}}}

        for (int promien = -rozdzielczosc/2; promien < rozdzielczosc/2; promien++)
        {

            unsigned char musztardaKolorPrzedMeow = 0;
            int i = promien;
            //if (!promien) { promien = 1; }

            float pozycjeStartowe[2] = {gracz[0], gracz[1]};

            float poprzedniaPozycjaStartowa[2] = {gracz[0],gracz[1]};

            double colorDoTablicy = 255;

            float stepX = ((float)cos(gracz[2] + ((float)i/(rozdzielczosc/10)   * 0.1))) / promienDlugosc;
            float stepY = ((float)sin(gracz[2] + ((float)i/(rozdzielczosc/10)   * 0.1))) / promienDlugosc;

            float wysokoscTimer = 1;
            int timer = 1;
            char czyDwa = 0;
            int czyZaczac = 0;

            int tablicaPocisku[3] = {  0,0, 0  };  //bool i wysokosc
            unsigned char czyByl = 0;

            int colorSaturationNaZaznaczonym2 = 0;
            int nygaWysokoscMeow = 0;
            int pozycjaPoczotkowaDlaTekstury[2];
            int pozycjaPoczotkowaDlaBloku[2];
            int wysokoscPrzyszla = 0;


            while (1) {


                if ((int)poprzedniaPozycjaStartowa[0]/10 != (int)pozycjeStartowe[0]/10 || (int)poprzedniaPozycjaStartowa[1]/10 != (int)pozycjeStartowe[1]/10  ) {
                    for (char i = 0; i < 2; i++) {
                        poprzedniaPozycjaStartowa[i] = pozycjeStartowe[i];
                    }
                }


                int aktualnyBlok = czyWartoscJestNaLiscie((int)pozycjeStartowe[0]/10, (int)pozycjeStartowe[1]/10);

                if (pozycjeStartowe[0] >= rozdzielczosc*10 || pozycjeStartowe[1] >= rozdzielczosc*10 || pozycjeStartowe[0] <= 0 || pozycjeStartowe[1] <= 0 || colorDoTablicy < 0   ) { colorDoTablicy = 0;  break;}
                else if (aktualnyBlok  == 1  ) { break;}
                else if ((aktualnyBlok == 2 || aktualnyBlok == 3) && czyByl == 0 ) {
                    czyDwa = 1;  czyZnalazlSieDwaWOguleWcalym = 1; colorSaturationNaZaznaczonym2 =  (int)colorDoTablicy; czyByl = 1; nygaWysokoscMeow = (int)((wysokoscTimer * (rozdzielczosc * 0.01) ) - rozdzielczosc ) * -1 +1;
                    pozycjaPoczotkowaDlaTekstury[0] =pozycjeStartowe[0]; pozycjaPoczotkowaDlaTekstury[1] =pozycjeStartowe[1];
                }

                pozycjeStartowe[0] += stepY;
                pozycjeStartowe[1] += stepX;

                colorDoTablicy = colorDoTablicy -  (2 / promienDlugosc)  ;

                for (unsigned char i = 0; i < wyznaczonyPocisk; i++) {
                    if ( (int)pociskiZBroni[i].y ==  (int)pozycjeStartowe[0] && (int)pociskiZBroni[i].x ==  (int)pozycjeStartowe[1]   ) {
                        tablicaPocisku[0] = 1;
                        tablicaPocisku[1] = (int)((((wysokoscTimer * (rozdzielczosc * 0.01) ) - rozdzielczosc ) * -1 +1) * 1)  /7  ;
                        tablicaPocisku[2] = colorDoTablicy;
                        break;
                    }
                }

                for (int y = 0; y < 3; y++) {tablica2D[(int)pozycjeStartowe[0]/10][(int)pozycjeStartowe[1]/10][y] = colorDoTablicy;}
                if (wysokoscTimer < rozdzielczosc * 10 && timer+1  ) { wysokoscTimer+= 1/promienDlugosc;}

                timer*= -1;
            }

            int wysokosc = (int)((wysokoscTimer * (rozdzielczosc * 0.01) ) - rozdzielczosc ) * -1 +1;

            unsigned char koloryTla[] = {237, 40, 40};

            for (int y = 0; y < rozdzielczosc/2; y++) {
                for (int z = 0; z < 3; z++) {
                    tablicaDoRenderowania[y][(promien+rozdzielczosc/2)][z] = koloryTla[z];
                    tablicaDoRenderowania[(y - rozdzielczosc) * -1][(promien+rozdzielczosc/2)][z] = koloryTla[z];
                    if (koloryTla[z] > 4) { koloryTla[z] = koloryTla[z] - koloryTla[z]/ (rozdzielczosc * 0.11 );  }
                }
            }


            if (czyTexture && (miejsceNaTeksture[4] != (int)(pozycjaPoczotkowaDlaTekstury[0]/10) || miejsceNaTeksture[5] != (int)(pozycjaPoczotkowaDlaTekstury[1]/10)))
            {czyDwa = 0;}


            if (!czyTexture && czyDwa ) {
                czyTexture++;
                miejsceNaTeksture[0] = nygaWysokoscMeow;
                miejsceNaTeksture[1] =  i+(rozdzielczosc/2);
                miejsceNaTeksture[3] = 0;
                miejsceNaTeksture[4] = (int)(pozycjaPoczotkowaDlaTekstury[0]/10);
                miejsceNaTeksture[5] = (int)(pozycjaPoczotkowaDlaTekstury[1]/10);
                miejsceNaTeksture[6] = colorSaturationNaZaznaczonym2;

            }
            else if (czyTexture && !czyDwa || czyDwa && i == rozdzielczosc/2 -1 ) {
                czyTexture = 0;

                miejsceNaTeksture[2] = nygaWysokoscMeow * 0.75;
                miejsceNaTeksture[3]*=2;

                SDL_Rect tymczasowyMangoMusztarda = {miejsceNaTeksture[1],(int)(rozdzielczosc - miejsceNaTeksture[0]*0.75 ) /2 ,miejsceNaTeksture[3],(int)(rozdzielczosc + miejsceNaTeksture[0] * 0.75) /2  };

                femboyTable[rozmiarFemboyTable][1].x = miejsceNaTeksture[6];
                femboyTable[rozmiarFemboyTable][0] = tymczasowyMangoMusztarda;
                rozmiarFemboyTable++;
                kopiowanieBoolDlaRenderu = 1;
                miejsceNaTeksture[3] = 0;
            }
            else if (czyDwa && czyTexture) {czyTexture++; miejsceNaTeksture[3]++;  }
            else if (!czyDwa && !czyTexture) {
                for (int y = (rozdzielczosc - wysokosc)/2 ; y < (rozdzielczosc + wysokosc)/2; y++) {

                    for (int z = 0; z < 3; z++) {tablicaDoRenderowania[y][(promien+rozdzielczosc/2)][z] = colorDoTablicy;}
                }
            }

            //system generowania dodatkowych lini AA
            int czyMozeInnyMeow  = 0;

            if (czyDoszloDoZmianyWysokosci[0] == 2 && (miejsceNaTeksture[4] != (int)(pozycjaPoczotkowaDlaBloku[0]/10) || miejsceNaTeksture[5] != (int)(pozycjaPoczotkowaDlaBloku[1]/10)))
            {czyMozeInnyMeow = 1;}


            if (czyDoszloDoZmianyWysokosci[0] == -2 && colorDoTablicy > 1 && !czyMozeInnyMeow) {
                czyDoszloDoZmianyWysokosci[0] = 2;

                liczbaWszystkichLini++;
                linia *tymczasowy = realloc(tablicaLiniWygladzanieKrawedzi, liczbaWszystkichLini * sizeof(linia));
                linia tymczasowaLinia = { (promien+rozdzielczosc/2) ,wysokosc, -1, -1, colorDoTablicy  };
                tymczasowy[liczbaWszystkichLini -1] = tymczasowaLinia;
                tablicaLiniWygladzanieKrawedzi = tymczasowy;

            }
            else if (czyDoszloDoZmianyWysokosci[0] == 2 && (colorDoTablicy < 1 || i == rozdzielczosc/2 -1) || czyMozeInnyMeow  ) {
                czyDoszloDoZmianyWysokosci[0] = -2;
                tablicaLiniWygladzanieKrawedzi[liczbaWszystkichLini -1].drugiX = (promien+rozdzielczosc/2);
                tablicaLiniWygladzanieKrawedzi[liczbaWszystkichLini -1].drugiY = czyDoszloDoZmianyWysokosci[1];
                tablicaLiniWygladzanieKrawedzi[liczbaWszystkichLini -1].kolor =  (tablicaLiniWygladzanieKrawedzi[liczbaWszystkichLini -1].kolor + musztardaKolorPrzedMeow) /2;
            }

            musztardaKolorPrzedMeow = colorDoTablicy;

            czyDoszloDoZmianyWysokosci[1] = wysokosc;
            //koniec AA

            unsigned char mango[] = {227, 12, 199};
            if (tablicaPocisku[0] ) {
                for (int y = (rozdzielczosc - tablicaPocisku[1])/2 ; y < (rozdzielczosc + tablicaPocisku[1])/2; y++) {
                    for (int z = 0; z < 3; z++) {tablicaDoRenderowania[y][(promien+rozdzielczosc/2)][z] = tablicaPocisku[2] + mango[z]   ;}
                }
            }
        }

        for (int y = 0; y < rozdzielczosc; y++) {
            for (int x = 0; x < rozdzielczosc; x++) {
                if (czyWartoscJestNaLiscie(y, x) == 1) {tablica2D[y][x][0] = 209;tablica2D[y][x][1] = 195;tablica2D[y][x][2] = 0;}
                else if  (czyWartoscJestNaLiscie(y, x) == 2) {tablica2D[y][x][0] = 255;tablica2D[y][x][1] = 18;tablica2D[y][x][2] = 188;}
                else if (czyWartoscJestNaLiscie(y, x) == 3) {tablica2D[y][x][0] = 80;tablica2D[y][x][1] = 210 ;tablica2D[y][x][2] = 255;}
            }
        }
        tablica2D[ (int)gracz[0]/10 ][(int)gracz[1]/10][0] = 255; tablica2D[ (int)gracz[0]/10 ][(int)gracz[1]/10][1] = 0; tablica2D[ (int)gracz[0]/10 ][(int)gracz[1]/10][2] = 0;

        if (!czyZnalazlSieDwaWOguleWcalym) {
            kopiowanieBoolDlaRenderu = 0;
            for (char i = 0; i < 64; i++ ) {
                femboyTable[i][0] = musztardaDymczasowa; femboyTable[i][0] = musztardaDymczasowa;
            }
        }
        for (unsigned char i = 0; i < wyznaczonyPocisk; i++) {
            tablica2D[ (int)pociskiZBroni[i].y/10  ][ (int)pociskiZBroni[i].x/10][0] = 244  ;
            tablica2D[ (int)pociskiZBroni[i].y/10  ][ (int)pociskiZBroni[i].x/10][0] = 255 ;
            tablica2D[ (int)pociskiZBroni[i].y/10  ][ (int)pociskiZBroni[i].x/10][0] = 173  ;
        }
        for (unsigned char i = 0; i < wyznaczonyPocisk; i++) {
            for (unsigned char j = 0; j < (int)round( pociskiZBroni[i].predkosc - pociskiZBroni[i].przyspieszenie) ; j++) {

                int yMeow = (int) ( pociskiZBroni[i].y -  sin(pociskiZBroni[i].kierunek)   )  / 10;
                int xMeow = (int) ( pociskiZBroni[i].x - cos(pociskiZBroni[i].kierunek) )  / 10;

                if (  czyWartoscJestNaLiscie(yMeow,xMeow) != 0 && pociskiZBroni[i].mode == 0 ) {
                    pociskiZBroni[i] = pustyPocisk;
                    usuwanieZListy(yMeow,xMeow);
                }
            }
        }
        if (czyPostawicKloca) {
            budowanie(typeOf);
            czyPostawicKloca = 0;
        }
        SDL_Delay(timeOf * 5);
    }
}
void *renderowanie() {
    SDL_Window* meowOkno = SDL_CreateWindow ("FajneOkno",windowRealResolutnion,windowRealResolutnion,windowRealResolutnion,windowRealResolutnion,SDL_WINDOW_SHOWN);
    SDL_Renderer* meowRender = SDL_CreateRenderer (meowOkno, -1, SDL_RENDERER_ACCELERATED);
    SDL_Event meowEvent;
    SDL_RenderSetLogicalSize(meowRender,rozdzielczosc,rozdzielczosc);
    femboyImage = IMG_LoadTexture(meowRender,"feris.png");
    SDL_Texture* tekstura = SDL_CreateTexture(meowRender,SDL_PIXELFORMAT_RGB24,SDL_TEXTUREACCESS_STREAMING,rozdzielczosc,rozdzielczosc);
    SDL_Texture* tekstura2D = SDL_CreateTexture(meowRender,SDL_PIXELFORMAT_RGB24,SDL_TEXTUREACCESS_STREAMING,rozdzielczosc,rozdzielczosc);
    IMG_Init(IMG_INIT_PNG);
    SDL_Texture* bron = IMG_LoadTexture(meowRender,"bronPalna.png");
    SDL_Rect miejsceBroni = { rozdzielczosc * 0.40625, rozdzielczosc * 0.8125, rozdzielczosc * 0.1875, rozdzielczosc * 0.1875};

    SDL_ShowCursor(SDL_DISABLE);

    while (turnOn) {
        if (renderingMode == 1) {
            SDL_UpdateTexture(tekstura, NULL, tablicaDoRenderowania, rozdzielczosc * 3);SDL_RenderCopy(meowRender, tekstura, NULL, NULL);

            for (int i = 1; i < liczbaWszystkichLini; i++ ) {
                //printf("%d %d %d %d \n", tablicaLiniWygladzanieKrawedzi[i].pierwszeX, tablicaLiniWygladzanieKrawedzi[i].pierwszyY,tablicaLiniWygladzanieKrawedzi[i].drugiX,tablicaLiniWygladzanieKrawedzi[i].drugiY);
                unsigned char meow = tablicaLiniWygladzanieKrawedzi[i].kolor;
                SDL_SetRenderDrawColor(meowRender, meow, meow, meow, 0  );

                if (tablicaLiniWygladzanieKrawedzi[i].pierwszeX > -1 &&tablicaLiniWygladzanieKrawedzi[i].pierwszyY > -1 &&tablicaLiniWygladzanieKrawedzi[i].drugiX > -1 &&tablicaLiniWygladzanieKrawedzi[i].drugiY > -1) {
                    for (int o = -4; o <= 4; o++) {
                        SDL_RenderDrawLine(
                        meowRender,tablicaLiniWygladzanieKrawedzi[i].pierwszeX,(rozdzielczosc - tablicaLiniWygladzanieKrawedzi[i].pierwszyY) / 2 +o,tablicaLiniWygladzanieKrawedzi[i].drugiX,(rozdzielczosc - tablicaLiniWygladzanieKrawedzi[i].drugiY) / 2 +o) ;
                        SDL_RenderDrawLine(meowRender,tablicaLiniWygladzanieKrawedzi[i].pierwszeX,(rozdzielczosc + tablicaLiniWygladzanieKrawedzi[i].pierwszyY) / 2+o,tablicaLiniWygladzanieKrawedzi[i].drugiX,(rozdzielczosc + tablicaLiniWygladzanieKrawedzi[i].drugiY) / 2 +o);
                    }
                }
            }

            czySkaskowacLinie = 1;

            if (kopiowanieBoolDlaRenderu) {
                int rozmiarFemboyTableRender = rozmiarFemboyTable;
                for (char i = 0; i < rozmiarFemboyTableRender; i++ ) {
                    Uint8 mango = (Uint8)femboyTable[i][1].x;
                    SDL_SetTextureColorMod(femboyImage, mango , mango, mango);
                    SDL_RenderCopy(meowRender, femboyImage, NULL, &femboyTable[i][0]);
                }
            }

            SDL_RenderCopy(meowRender, bron, NULL, &miejsceBroni);
        }
        else if (renderingMode == -1)
        {SDL_UpdateTexture(tekstura2D, NULL, tablica2D, rozdzielczosc * 3); SDL_RenderCopy(meowRender, tekstura2D, NULL, NULL);}

        while (SDL_PollEvent(&meowEvent)) {
            if (meowEvent.type == SDL_QUIT ) { turnOn = 0; }

            if (meowEvent.type == SDL_MOUSEMOTION) {

                if (meowEvent.motion.xrel == -1) {gracz[2] -= 0.02; if (gracz[2] < 0) { gracz[2] = 6.283;   };}
                else if (meowEvent.motion.xrel == 1) {gracz[2] += 0.02;   if (gracz[2] > 6.3) { gracz[2] = 0;   };}

                SDL_WarpMouseInWindow(meowOkno, rozdzielczosc /2, (rozdzielczosc / 2) );
            }
            else if (meowEvent.type == SDL_MOUSEBUTTONDOWN && !czyDodacnowyPociskNaListe  && wyznaczonyPocisk < 63) {
                if (meowEvent.button.button == SDL_BUTTON_LEFT ) {nowyPociskFunkcja(0);}
                else if (meowEvent.button.button == SDL_BUTTON_RIGHT ) {czyPostawicKloca = 1; }

            }
            else if (meowEvent.type == SDL_KEYDOWN)
            {
                if (meowEvent.key.keysym.sym == SDLK_ESCAPE ) {renderingMode = 1;}
                else if (meowEvent.key.keysym.sym == SDLK_m) { renderingMode *= -1;  }
                for (int x = 0; x < 2; x++) {
                    if (meowEvent.key.keysym.sym == inputNaLiczby[0][x]) {
                        int mango67One = (int)((gracz[0] + sin(gracz[2]) * inputNaLiczby[1][x] ) / 10 ) ;
                        int mango67Two = (int)((gracz[1] + cos(gracz[2]) * inputNaLiczby[1][x] ) / 10 )  ;

                        double meow67One = gracz[0] + (float)sin(gracz[2]) * inputNaLiczby[1][x];
                        double meow67Twoo = gracz[1] + (float)cos(gracz[2]) * inputNaLiczby[1][x];

                        if (!(czyWartoscJestNaLiscie(mango67One,mango67Two)) && meow67One < rozdzielczosc*10 &&
                            meow67One+meow67Twoo < rozdzielczosc*10 && meow67One > 0 && meow67Twoo > 0    ) {
                            gracz[0] = meow67One;
                            gracz[1] = meow67Twoo;
                            break;
                        }
                        else if (czyWartoscJestNaLiscie(mango67One,mango67Two) == 3 && czySmiercOdWrogow) {
                            turnOn = false;
                        }
                    }
                }
                for (char i = 0; i < 10; i++) {
                    if (meowEvent.key.keysym.sym == listaZnakow[i]   ) {
                        typeOf = i;
                    }
                }
            }
        }
        SDL_RenderPresent(meowRender);
        SDL_Delay(timeOf);
    }

    SDL_DestroyRenderer(meowRender);
    SDL_DestroyWindow(meowOkno);
    SDL_DestroyTexture(bron);
    SDL_DestroyTexture(tekstura);
    IMG_Quit();
    SDL_Quit();
}

int main() {
    srand(time(NULL));

    czySmiercOdWrogow = false;


    for (int y = 0; y < rozdzielczosc; y++) { for (int x = 0; x < rozdzielczosc; x++) {  }}
    tablicaElementowNaPlanszy = (obiekt*) malloc( liczbaElementowNaCalejTablicy * sizeof(obiekt));

    tablicaLiniWygladzanieKrawedzi = ( linia*) malloc(sizeof (linia   ) * liczbaWszystkichLini)  ;




    tablicaElementowNaPlanszy[1].y = 5;
    tablicaElementowNaPlanszy[1].x = 10;
    tablicaElementowNaPlanszy[1].type = 1;
    tablicaElementowNaPlanszy[1].id = 1;
    for (unsigned char i = 0; i < 3; i++) {
        tablicaElementowNaPlanszy[i+2].y = 15;
        tablicaElementowNaPlanszy[i+2].x = (float)i +  9;
        tablicaElementowNaPlanszy[i+2].type = 1;
        tablicaElementowNaPlanszy[i + 2].id = 2 + i;
    }

    id = 4;

    pthread_t matematyka;
    pthread_t wyswietlanie;

    pthread_create(&matematyka, NULL, obliczeniaMatematyczne, NULL);
    pthread_create(&wyswietlanie, NULL, renderowanie, NULL);

    pthread_detach(matematyka);
    pthread_detach(wyswietlanie);
    while (turnOn) { SDL_Delay(250);  }
    free(tablicaElementowNaPlanszy);
    free( tablicaLiniWygladzanieKrawedzi);
    return 67;

}