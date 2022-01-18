#include <exception>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL.h>
using namespace std;

SDL_Window* window = NULL;
SDL_Surface* screen = NULL;

#define szerokosc 512
#define wysokosc 340

#define tytul "GKiM2021 - projekt"

//

void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);
SDL_Color getPixel(int x, int y);

void czyscEkran(Uint8 R, Uint8 G, Uint8 B);

int dodajKolor(SDL_Color kolor);
bool porownajKolory(SDL_Color k1, SDL_Color k2);
int sprawdzKolor(SDL_Color kolor);

void Funkcja1();
void Funkcja2();
void Funkcja3();
void Funkcja4();
void Funkcja5();
void Funkcja6();
void Funkcja7();
void Funkcja8();
void Funkcja9();


SDL_Color paleta[256];
int ileKolorow = 0;

void Funkcja1() {

    //...

    SDL_UpdateWindowSurface(window);
}

void Funkcja2() {

    //...

    SDL_UpdateWindowSurface(window);
}
SDL_Color* createpalette() {
    SDL_Color* palette = new SDL_Color[32];
    SDL_Color color;
    int tmp = 0;
    for (int i = 0; i < 4; i++)
    {
        color.r = i * 85;

        for (int j = 0; j < 4; j++)
        {
            color.g = j * 85;

            for (int k = 0; k < 2; k++)
            {
                color.b = k * 255;
                palette[tmp] = color;

                tmp++;
            }
        }
    }
    return palette;
}
void closest(int*R,int*G,int*B, int *bladR, int*bladG,int*bladB) {
    
    SDL_Color* palette = createpalette();
    
    int oldR = *R, oldG = *G, oldB = *B;
    int diffR = 0, diffG = 0, diffB = 0;
    int smallestDiffR = 255, smallestDiffG = 255, smallestDiffB = 255;
    
    for (int i = 0; i < 32; i++) {
        diffR = abs(oldR - palette[i].r);
        if (diffR < smallestDiffR) {
            smallestDiffR = diffR;
            *R = palette[i].r;
            *bladR = oldR - *R;
        }

        diffG= abs(oldG - palette[i].g);
        if (diffG < smallestDiffG) {
            smallestDiffG = diffG;
            *G = palette[i].g;
            *bladG = oldG - *G;
        }

        diffB = abs(oldB - palette[i].b);
        if (diffB < smallestDiffB) {
            smallestDiffB = diffB;
            *B = palette[i].b;
            *bladB = oldB - *B;
        }
    }
}
void Funkcja3() {

    SDL_Color kolor;
    float bledyR[(szerokosc / 2) + 2][(wysokosc / 2) + 1];
    memset(bledyR, 0, sizeof(bledyR));
    float bledyG[(szerokosc / 2) + 2][(wysokosc / 2) + 1];
    memset(bledyG, 0, sizeof(bledyG));
    float bledyB[(szerokosc / 2) + 2][(wysokosc / 2) + 1];
    memset(bledyB, 0, sizeof(bledyB));
    float bledy[(szerokosc / 2) + 2][(wysokosc / 2) + 1];
    memset(bledy, 0, sizeof(bledy));

    int przesuniecie = 1;
    int bladR = 0, bladG = 0, bladB = 0;

    int R, G, B;
    int i = 0;

    for (int x = 0; x < szerokosc / 2; x++) {
        for (int y = 0; y < wysokosc / 2; y++) {

            kolor = getPixel(x, y);
            R = kolor.r + bledyR[x + przesuniecie][y];
            G = kolor.g + bledyG[x + przesuniecie][y];
            B = kolor.b + bledyB[x + przesuniecie][y];
            int oldR = kolor.r, oldG = kolor.g, oldB = kolor.b;
            kolor.r = R;
            kolor.g = G;
            kolor.b = B;

            closest(&R,&G,&B, &bladR, &bladG, &bladB);
           
            setPixel(x + szerokosc / 2, y, R, G, B);

            bledyR[x + przesuniecie + 1][y] += (bladR * 7.0 / 16.0);
            bledyR[x + przesuniecie - 1][y + 1] += (bladR * 3.0 / 16.0);
            bledyR[x + przesuniecie][y + 1] += (bladR * 5.0 / 16.0);
            bledyR[x + przesuniecie + 1][y + 1] += (bladR * 1.0 / 16.0);

            bledyG[x + przesuniecie + 1][y] += (bladG * 7.0 / 16.0);
            bledyG[x + przesuniecie - 1][y + 1] += (bladG * 3.0 / 16.0);
            bledyG[x + przesuniecie][y + 1] += (bladG * 5.0 / 16.0);
            bledyG[x + przesuniecie + 1][y + 1] += (bladG * 1.0 / 16.0);

            bledyB[x + przesuniecie + 1][y] += (bladB * 7.0 / 16.0);
            bledyB[x + przesuniecie - 1][y + 1] += (bladB * 3.0 / 16.0);
            bledyB[x + przesuniecie][y + 1] += (bladB * 5.0 / 16.0);
            bledyB[x + przesuniecie + 1][y + 1] += (bladB * 1.0 / 16.0);

        }
    }

    ileKolorow = 0;
    for (int x = szerokosc / 2; x < szerokosc; x++) {
        for (int y = 0; y < wysokosc / 2; y++) {
            kolor = getPixel(x, y);
            sprawdzKolor(kolor);
        }
    }
    cout << ileKolorow;

    SDL_UpdateWindowSurface(window);


}

void Funkcja4() {

    int R, G, B;
    SDL_Color kolor;
    int BW;
    //int BW;
    for (int x = 0; x < szerokosc / 2; x++) {
        for (int y = 0; y < wysokosc / 2; y++) {

            kolor = getPixel(x, y);
            R = kolor.r;
            G = kolor.g;
            B = kolor.b;
           
            BW = 0.299 * R + 0.587 * G + 0.114 * B;
            setPixel(x+szerokosc/2, y + wysokosc / 2, BW, BW, BW);
           BW=BW >> 3;
            BW = BW << 3;
           // BW = round((BW * 32) / 255);
            setPixel(x + szerokosc / 2, y, BW, BW, BW);
  
        }
    }

    SDL_UpdateWindowSurface(window);

}

SDL_Color* createBWpalette() {
    SDL_Color* bwPalette = new SDL_Color[32];
    SDL_Color color;
    for (int i = 0; i < 32; i++) {
        color.r = i * 8;
        color.g = i * 8;
        color.b = i * 8;
        bwPalette[i] = color;
    }
    return bwPalette;
}

int closestBW(int BW) {
    SDL_Color* palette=createBWpalette();
    int diff = 0;
    int smallestDiff = 255;
    int oldBW = BW;
    for (int i = 0; i < 32; i++) {
        diff = abs(oldBW - palette[i].r);
        if (diff < smallestDiff) {
            smallestDiff = diff;
            BW = palette[i].r;
        }

    }
    oldBW = BW;
    return oldBW;
}
void Funkcja5() {

    float bledy[(szerokosc / 2) + 2][(wysokosc / 2) + 1];
    memset(bledy, 0, sizeof(bledy));

    int przesuniecie = 1;
    int blad = 0;
    int BW = 0; 
    int oldBW = 0;
    SDL_Color kolor;
    for (int x = 0; x < szerokosc / 2; x++) {
        for (int y = 0; y < wysokosc / 2; y++) {
            kolor = getPixel(x, y);
        

            BW = 0.299 * kolor.r + 0.587 * kolor.g + 0.114 * kolor.b;
            BW += bledy[x + przesuniecie][y];
            oldBW = BW;
            BW= closestBW(BW);
           
            blad = oldBW - BW;//r,g,b sa takie same
            setPixel(x+szerokosc/2, y, BW, BW, BW);
            kolor.r = BW;
            kolor.g = BW;
            kolor.b = BW;
            sprawdzKolor(kolor);
            bledy[x + przesuniecie + 1][y] += (blad * 7.0 / 16.0);
            bledy[x + przesuniecie + 1][y + 1] += (blad * 1.0 / 16.0);
            bledy[x + przesuniecie][y + 1] += (blad * 5.0 / 16.0);
            bledy[x + przesuniecie - 1][y + 1] += (blad * 3.0 / 16.0);

        }
    }

    SDL_UpdateWindowSurface(window);

}

void Funkcja6() {

    //...

    SDL_UpdateWindowSurface(window);
}

void najblizszaDopasowana(int* R, int* G, int* B, int* bladR, int* bladG, int* bladB) {

    SDL_Color* palette = createpalette();

    int oldR = *R, oldG = *G, oldB = *B;
    int diffR = 0, diffG = 0, diffB = 0;
    int smallestDiffR = 255, smallestDiffG = 255, smallestDiffB = 255;

    for (int i = 0; i < 32; i++) {
        diffR = abs(oldR - palette[i].r);
        if (diffR < smallestDiffR) {
            smallestDiffR = diffR;
            *R = palette[i].r;
            *bladR = oldR - *R;
        }

        diffG = abs(oldG - palette[i].g);
        if (diffG < smallestDiffG) {
            smallestDiffG = diffG;
            *G = palette[i].g;
            *bladG = oldG - *G;
        }

        diffB = abs(oldB - palette[i].b);
        if (diffB < smallestDiffB) {
            smallestDiffB = diffB;
            *B = palette[i].b;
            *bladB = oldB - *B;
        }
    }
}

void Funkcja7() {

    SDL_Color kolor;
    float bledyR[(szerokosc / 2) + 2][(wysokosc / 2) + 1];
    memset(bledyR, 0, sizeof(bledyR));
    float bledyG[(szerokosc / 2) + 2][(wysokosc / 2) + 1];
    memset(bledyG, 0, sizeof(bledyG));
    float bledyB[(szerokosc / 2) + 2][(wysokosc / 2) + 1];
    memset(bledyB, 0, sizeof(bledyB));
    float bledy[(szerokosc / 2) + 2][(wysokosc / 2) + 1];
    memset(bledy, 0, sizeof(bledy));

    int przesuniecie = 1;
    int bladR = 0, bladG = 0, bladB = 0;

    int R, G, B;
    int i = 0;

    for (int x = 0; x < szerokosc / 2; x++) {
        for (int y = 0; y < wysokosc / 2; y++) {

            kolor = getPixel(x, y);
            R = kolor.r + bledyR[x + przesuniecie][y];
            G = kolor.g + bledyG[x + przesuniecie][y];
            B = kolor.b + bledyB[x + przesuniecie][y];
            int oldR = kolor.r, oldG = kolor.g, oldB = kolor.b;
            kolor.r = R;
            kolor.g = G;
            kolor.b = B;

            closest(&R, &G, &B, &bladR, &bladG, &bladB);

            setPixel(x + szerokosc / 2, y, R, G, B);

            bledyR[x + przesuniecie + 1][y] += (bladR * 7.0 / 16.0);
            bledyR[x + przesuniecie - 1][y + 1] += (bladR * 3.0 / 16.0);
            bledyR[x + przesuniecie][y + 1] += (bladR * 5.0 / 16.0);
            bledyR[x + przesuniecie + 1][y + 1] += (bladR * 1.0 / 16.0);

            bledyG[x + przesuniecie + 1][y] += (bladG * 7.0 / 16.0);
            bledyG[x + przesuniecie - 1][y + 1] += (bladG * 3.0 / 16.0);
            bledyG[x + przesuniecie][y + 1] += (bladG * 5.0 / 16.0);
            bledyG[x + przesuniecie + 1][y + 1] += (bladG * 1.0 / 16.0);

            bledyB[x + przesuniecie + 1][y] += (bladB * 7.0 / 16.0);
            bledyB[x + przesuniecie - 1][y + 1] += (bladB * 3.0 / 16.0);
            bledyB[x + przesuniecie][y + 1] += (bladB * 5.0 / 16.0);
            bledyB[x + przesuniecie + 1][y + 1] += (bladB * 1.0 / 16.0);

        }
    }

    ileKolorow = 0;
    for (int x = szerokosc / 2; x < szerokosc; x++) {
        for (int y = 0; y < wysokosc / 2; y++) {
            kolor = getPixel(x, y);
            sprawdzKolor(kolor);
        }
    }
    cout << ileKolorow;

    SDL_UpdateWindowSurface(window);

}

void Funkcja8() {

    //...

    SDL_UpdateWindowSurface(window);
}

void Funkcja9() {

    //...

    SDL_UpdateWindowSurface(window);
}

int dodajKolor(SDL_Color kolor) {
    int aktualnyKolor = ileKolorow;
    paleta[aktualnyKolor] = kolor;
    cout << aktualnyKolor << ": [" << (int)kolor.r << "," << (int)kolor.g << "," << (int)kolor.b << "]" << endl;
    ileKolorow++;
    return (aktualnyKolor);
}

bool porownajKolory(SDL_Color k1, SDL_Color k2) {
    bool wynik = true;
    if (k1.r != k2.r) wynik = false;
    if (k1.g != k2.g) wynik = false;
    if (k1.b != k2.b) wynik = false;
    return wynik;
}

int sprawdzKolor(SDL_Color kolor) {
    int wynik = -1;
    if (ileKolorow > 0) {
        for (int k = 0; k < ileKolorow; k++) {
            if (porownajKolory(kolor, paleta[k])) {
                wynik = k;
            }
        }
    }
    if (wynik < 0) {
        wynik = dodajKolor(kolor);
    }
    return wynik;
}


void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
    if ((x >= 0) && (x < szerokosc) && (y >= 0) && (y < wysokosc))
    {
        /* Zamieniamy poszczególne składowe koloru na format koloru piksela */
        Uint32 pixel = SDL_MapRGB(screen->format, R, G, B);

        /* Pobieramy informację ile bajtów zajmuje jeden piksel */
        int bpp = screen->format->BytesPerPixel;

        /* Obliczamy adres piksela */
        Uint8* p1 = (Uint8*)screen->pixels + (y * 2) * screen->pitch + (x * 2) * bpp;
        Uint8* p2 = (Uint8*)screen->pixels + (y * 2 + 1) * screen->pitch + (x * 2) * bpp;
        Uint8* p3 = (Uint8*)screen->pixels + (y * 2) * screen->pitch + (x * 2 + 1) * bpp;
        Uint8* p4 = (Uint8*)screen->pixels + (y * 2 + 1) * screen->pitch + (x * 2 + 1) * bpp;

        /* Ustawiamy wartość piksela, w zależnoœci od formatu powierzchni*/
        switch (bpp)
        {
        case 1: //8-bit
            *p1 = pixel;
            *p2 = pixel;
            *p3 = pixel;
            *p4 = pixel;
            break;

        case 2: //16-bit
            *(Uint16*)p1 = pixel;
            *(Uint16*)p2 = pixel;
            *(Uint16*)p3 = pixel;
            *(Uint16*)p4 = pixel;
            break;

        case 3: //24-bit
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p1[0] = (pixel >> 16) & 0xff;
                p1[1] = (pixel >> 8) & 0xff;
                p1[2] = pixel & 0xff;
                p2[0] = (pixel >> 16) & 0xff;
                p2[1] = (pixel >> 8) & 0xff;
                p2[2] = pixel & 0xff;
                p3[0] = (pixel >> 16) & 0xff;
                p3[1] = (pixel >> 8) & 0xff;
                p3[2] = pixel & 0xff;
                p4[0] = (pixel >> 16) & 0xff;
                p4[1] = (pixel >> 8) & 0xff;
                p4[2] = pixel & 0xff;
            }
            else {
                p1[0] = pixel & 0xff;
                p1[1] = (pixel >> 8) & 0xff;
                p1[2] = (pixel >> 16) & 0xff;
                p2[0] = pixel & 0xff;
                p2[1] = (pixel >> 8) & 0xff;
                p2[2] = (pixel >> 16) & 0xff;
                p3[0] = pixel & 0xff;
                p3[1] = (pixel >> 8) & 0xff;
                p3[2] = (pixel >> 16) & 0xff;
                p4[0] = pixel & 0xff;
                p4[1] = (pixel >> 8) & 0xff;
                p4[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4: //32-bit
            *(Uint32*)p1 = pixel;
            *(Uint32*)p2 = pixel;
            *(Uint32*)p3 = pixel;
            *(Uint32*)p4 = pixel;
            break;

        }
    }
}

void setPixelSurface(int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
    if ((x >= 0) && (x < szerokosc * 2) && (y >= 0) && (y < wysokosc * 2))
    {
        /* Zamieniamy poszczególne składowe koloru na format koloru piksela */
        Uint32 pixel = SDL_MapRGB(screen->format, R, G, B);

        /* Pobieramy informację ile bajtów zajmuje jeden piksel */
        int bpp = screen->format->BytesPerPixel;

        /* Obliczamy adres piksela */
        Uint8* p = (Uint8*)screen->pixels + y * screen->pitch + x * bpp;

        /* Ustawiamy wartość piksela, w zależności od formatu powierzchni*/
        switch (bpp)
        {
        case 1: //8-bit
            *p = pixel;
            break;

        case 2: //16-bit
            *(Uint16*)p = pixel;
            break;

        case 3: //24-bit
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4: //32-bit
            *(Uint32*)p = pixel;
            break;
        }
    }
}

SDL_Color getPixel(int x, int y) {
    SDL_Color color;
    Uint32 col = 0;
    if ((x >= 0) && (x < szerokosc) && (y >= 0) && (y < wysokosc)) {
        //określamy pozycję
        char* pPosition = (char*)screen->pixels;

        //przesunięcie względem y
        pPosition += (screen->pitch * y * 2);

        //przesunięcie względem x
        pPosition += (screen->format->BytesPerPixel * x * 2);

        //kopiujemy dane piksela
        memcpy(&col, pPosition, screen->format->BytesPerPixel);

        //konwertujemy kolor
        SDL_GetRGB(col, screen->format, &color.r, &color.g, &color.b);
    }
    return (color);
}

SDL_Color getPixelSurface(int x, int y, SDL_Surface* surface) {
    SDL_Color color;
    Uint32 col = 0;
    if ((x >= 0) && (x < szerokosc) && (y >= 0) && (y < wysokosc)) {
        //określamy pozycję
        char* pPosition = (char*)surface->pixels;

        //przesunięcie względem y
        pPosition += (surface->pitch * y);

        //przesunięcie względem x
        pPosition += (surface->format->BytesPerPixel * x);

        //kopiujemy dane piksela
        memcpy(&col, pPosition, surface->format->BytesPerPixel);

        //konwertujemy kolor
        SDL_GetRGB(col, surface->format, &color.r, &color.g, &color.b);
    }
    return (color);
}


void ladujBMP(char const* nazwa, int x, int y)
{
    SDL_Surface* bmp = SDL_LoadBMP(nazwa);
    if (!bmp)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
    }
    else
    {
        SDL_Color kolor;
        for (int yy = 0; yy < bmp->h; yy++) {
            for (int xx = 0; xx < bmp->w; xx++) {
                kolor = getPixelSurface(xx, yy, bmp);
                setPixel(xx, yy, kolor.r, kolor.g, kolor.b);
            }
        }
        SDL_FreeSurface(bmp);
        SDL_UpdateWindowSurface(window);
    }

}


void czyscEkran(Uint8 R, Uint8 G, Uint8 B)
{
    SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, R, G, B));
    SDL_UpdateWindowSurface(window);
}



int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    window = SDL_CreateWindow(tytul, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szerokosc * 2, wysokosc * 2, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    screen = SDL_GetWindowSurface(window);
    if (screen == NULL) {
        fprintf(stderr, "SDL_GetWindowSurface Error: %s\n", SDL_GetError());
        return false;
    }
    SDL_UpdateWindowSurface(window);


    bool done = false;
    SDL_Event event;
    // główna pętla programu
    while (SDL_WaitEvent(&event)) {
        // sprawdzamy czy pojawiło się zdarzenie
        switch (event.type) {
        case SDL_QUIT:
            done = true;
            break;

            // sprawdzamy czy został wciśnięty klawisz
        case SDL_KEYDOWN: {
            // wychodzimy, gdy wciśnięto ESC
            if (event.key.keysym.sym == SDLK_ESCAPE)
                done = true;
            if (event.key.keysym.sym == SDLK_1)
                Funkcja1();
            if (event.key.keysym.sym == SDLK_2)
                Funkcja2();
            if (event.key.keysym.sym == SDLK_3)
                Funkcja3();
            if (event.key.keysym.sym == SDLK_4)
                Funkcja4();
            if (event.key.keysym.sym == SDLK_5)
                Funkcja5();
            if (event.key.keysym.sym == SDLK_6)
                Funkcja6();
            if (event.key.keysym.sym == SDLK_7)
                Funkcja7();
            if (event.key.keysym.sym == SDLK_8)
                Funkcja8();
            if (event.key.keysym.sym == SDLK_9)
                Funkcja9();
            if (event.key.keysym.sym == SDLK_a)
                ladujBMP("obrazek1.bmp", 0, 0);
            if (event.key.keysym.sym == SDLK_s)
                ladujBMP("obrazek2.bmp", 0, 0);
            if (event.key.keysym.sym == SDLK_d)
                ladujBMP("obrazek3.bmp", 0, 0);
            if (event.key.keysym.sym == SDLK_f)
                ladujBMP("obrazek4.bmp", 0, 0);
            if (event.key.keysym.sym == SDLK_g)
                ladujBMP("obrazek5.bmp", 0, 0);
            if (event.key.keysym.sym == SDLK_h)
                ladujBMP("obrazek6.bmp", 0, 0);
            if (event.key.keysym.sym == SDLK_j)
                ladujBMP("obrazek7.bmp", 0, 0);
            if (event.key.keysym.sym == SDLK_k)
                ladujBMP("obrazek8.bmp", 0, 0);
            if (event.key.keysym.sym == SDLK_b)
                czyscEkran(0, 0, 10);
            else
                break;
        }
        }
        if (done) break;
    }

    if (screen) {
        SDL_FreeSurface(screen);
    }

    if (window) {
        SDL_DestroyWindow(window);
    }


    SDL_Quit();
    return 0;
}
