﻿#include <exception>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL.h>
#include <set>
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
void znajdzWszystkieKolory();

void podzielZakresy(int glebokosc, int indeks_poczatkowy, int indeks_koncowy);
int znajdzNajszerszyKanal(int indeks_poczatkowy, int indeks_koncowy);
void kwantyzacja(int indeks_poczatkowy, int indeks_koncowy);
void swap(int i, int j);
int split(int l, int r, int kanal);
void quickSort(int l, int r, int kanal);

void Funkcja1();
void Funkcja2();
void Funkcja3();
void Funkcja4();
void Funkcja5();
void Funkcja6();
void Funkcja7();
void Funkcja8();
void Funkcja9();

SDL_Color paleta[szerokosc / 2 * wysokosc / 2];
SDL_Color dopasowanaPaleta[32];
int ileKolorow = 0;
int dopasowanychKolorow = 0;
int dopasowana = false;

// uzywana do debuggowania, wypisuje w konsoli wartosci kolorow
void wypiszPalete() {

    cout << "Zwykla:\n";
    for (int i = 0; i < ileKolorow; i++) {
        cout << i << ": [" << (int)paleta[i].r << "," << (int)paleta[i].g << "," << (int)paleta[i].b << "]" << endl;

    }

    cout << "Dopasowana:\n";
    for (int i = 0; i < dopasowanychKolorow; i++) {
        cout << i << ": [" << (int)dopasowanaPaleta[i].r << "," << (int)dopasowanaPaleta[i].g << "," << (int)dopasowanaPaleta[i].b << "]" << endl;

    }
}

// wyswietl palete uzywana podczas konwersji obrazka
void wyswieltPalete() {
    int i = 0;
    Uint8 R{}, G{}, B{};

    // jesli uzyta zostala dopasowana paleta wyswietl ja
    if (dopasowana and dopasowanychKolorow > 0)
        for (int x = 0; x < szerokosc; x++) {
            if (x % (szerokosc / dopasowanychKolorow) == 0 and x != 0)
                i++;
            for (int y = 0; y < wysokosc / 16; y++) {
                setPixel(x, y + wysokosc / 2, dopasowanaPaleta[i].r, dopasowanaPaleta[i].g, dopasowanaPaleta[i].b);
            }
        }

    // jesli uzyta zostala narzucona paleta wyswietl ja
    if (!dopasowana and ileKolorow > 0 and ileKolorow < szerokosc)
        for (int x = 0; x < szerokosc; x++) {
            if (x % (szerokosc / ileKolorow) == 0 and x != 0)
                i++;
            for (int y = 0; y < wysokosc / 16; y++) {

                setPixel(x, y + +wysokosc / 2, paleta[i].r, paleta[i].g, paleta[i].b);
            }
        }

}

//funkcja zapisuje wszystkie unikatowe kolory palecie
void znajdzWszystkieKolory() {
    SDL_Color kolor;
    ileKolorow = 0;

    for (int y = 0; y < wysokosc / 2; y++) {
        for (int x = 0; x < szerokosc / 2; x++) {
            kolor = getPixel(x, y);
            sprawdzKolor(kolor);
        }
    }
}

//Funkcja odszukuje kanał cechujacy sie najwiekszym zakresem barw i zwraca jego indeks
int znajdzNajszerszyKanal(int indeks_poczatkowy, int indeks_koncowy) {

    int r_min, g_min, b_min;            // najmniejsze znalezione wartosci dla poszczegolnych kanalow 
    int r_max, g_max, b_max;            // najwieksze znalezione wartosci dla poszczegolnych kanalow
    int r_zakres, g_zakres, b_zakres;   // znalezione zakresy dla poszczegolnych kanalow
    int wielkosc = indeks_koncowy - indeks_poczatkowy;  // ilosc rozpatrywanych kolorow

    r_min = g_min = b_min = INT_MAX;
    r_max = g_max = b_max = INT_MIN;

    for (int i = indeks_poczatkowy; i <= indeks_koncowy; i++) {

        //kanal czerwony
        if ((int)paleta[i].r > r_max)
            r_max = (int)paleta[i].r;
        if ((int)paleta[i].r < r_min)
            r_min = (int)paleta[i].r;

        //kanal zielony
        if ((int)paleta[i].g > g_max)
            g_max = (int)paleta[i].g;
        if ((int)paleta[i].g < g_min)
            g_min = (int)paleta[i].g;

        //kanal niebieski
        if ((int)paleta[i].b > b_max)
            b_max = (int)paleta[i].b;
        if ((int)paleta[i].b < b_min)
            b_min = (int)paleta[i].b;

    }

    r_zakres = r_max - r_min;
    g_zakres = g_max - g_min;
    b_zakres = b_max - b_min;

    //kanał pierwszy(R) najwiekszy zakres
    if (r_zakres >= g_zakres and r_zakres >= b_zakres)
        return 0;

    //kanał drugi(G) najwiekszy zakres
    else if (g_zakres >= b_zakres and g_zakres >= g_zakres)
        return 1;

    //kanał trzeci(B) najwiekszy zakres
    else if (b_zakres >= r_zakres and b_zakres >= g_zakres)
        return 2;

}

// zamiana wartosci pod indeksami i oraz j w podstawowej palecie
void swap(int i, int j) {

    SDL_Color temp = paleta[j];
    paleta[j] = paleta[i];
    paleta[i] = temp;

}

// funkcja split dla algorytmu quicksort, sortowanie wzgledem jednego kanalu
int split(int l, int r, int kanal) {

    // wyznacz indeks pivotu na wartosc srodkowa zakresu
    int pivot_index = l + ((r - l) / 2);
    swap(pivot_index, r);

    int pivot = 0;

    // pivot ustaw na wartosc odpowiedniego elementu w tabeli oraz odpowiedniej skladowej
    if (kanal == 0)
        pivot = paleta[r].r;
    else if (kanal == 1)
        pivot = paleta[r].g;
    else
        pivot = paleta[r].b;

    int i = l;

    // sortowania wzgledem jednego kanalu
    for (int j = l; j < r; j++) {

        if (kanal == 0 and paleta[j].r < pivot) {
            swap(j, i);
            i++;
        }

        else if (kanal == 1 and paleta[j].g < pivot) {
            swap(j, i);
            i++;
        }

        else if (kanal == 2 and paleta[j].b < pivot) {
            swap(j, i);
            i++;
        }
    }

    swap(i, r);
    return i;

}

// algorytm szybkiego sortowania, sortowanie wzgledem jednego kanalu
void quickSort(int l, int r, int kanal) {

    //rekurencja ogonowa
    while (l < r) {
        int pivot = split(l, r, kanal);
        quickSort(l, pivot - 1, kanal);
        l = pivot + 1;
    }

}

// kwantyzacja koloru w podanym zakresie
void kwantyzacja(int indeks_poczatkowy, int indeks_koncowy) {

    int srednia_r{}, srednia_g{}, srednia_b{};          // srednie wartosci skladowych w podanym zakresie
    int wielkosc = indeks_koncowy - indeks_poczatkowy;  // wielkosc badanego zakresu


    // wyliczanie sumy dla kazdej ze skladowych
    for (int i = indeks_poczatkowy; i <= indeks_koncowy; i++) {
        srednia_r += paleta[i].r;
        srednia_g += paleta[i].g;
        srednia_b += paleta[i].b;
    }

    // liczenie sredniej kazdej ze skladowych
    srednia_r /= wielkosc;
    srednia_g /= wielkosc;
    srednia_b /= wielkosc;

    // dopisywanie nowego kolory do dopasowanej palety
    dopasowanaPaleta[dopasowanychKolorow].r = srednia_r;
    dopasowanaPaleta[dopasowanychKolorow].g = srednia_g;
    dopasowanaPaleta[dopasowanychKolorow].b = srednia_b;
    dopasowanychKolorow++;
}

// dzielenie palety na mniejsze czesci i sortowanie tych czesci wzgledem najszerszych kanalow
void podzielZakresy(int glebokosc, int indeks_poczatkowy, int indeks_koncowy) {

    // przerwij oczekiwana ilosc barw zostala osiagnieta badz zakres jest niepoprawny
    if (glebokosc < 0 or indeks_koncowy - indeks_poczatkowy <= 0)
        return;

    // jesli paleta zostala podzielona na odpowiednie czesci zacznij kwantyzacje
    if (glebokosc == 0) {
        kwantyzacja(indeks_poczatkowy, indeks_koncowy);
        return;
    }

    // odszukaj najszerszy kanal i sortuj wzgledem niego
    int kanal = znajdzNajszerszyKanal(indeks_poczatkowy, indeks_koncowy);
    quickSort(indeks_poczatkowy, indeks_koncowy, kanal);

    // znajdz srodek badanego zakresu
    int mediana_indeksow = ((indeks_poczatkowy + indeks_koncowy) + 1) / 2;

    // dokonaj kolejnego podzialu
    podzielZakresy(glebokosc - 1, indeks_poczatkowy, mediana_indeksow);
    podzielZakresy(glebokosc - 1, mediana_indeksow, indeks_koncowy);

}

// algorytm median cut do odnajdywania dopasowanej palety
void medianCut() {

    // na ilu bitach ma sie miescic nowy kolor
    int ileBit = 5;

    dopasowanychKolorow = 0;
    znajdzWszystkieKolory();

    // jesli na obrazku jest wiecej kolorow szukaj palety
    if (ileKolorow > 32)
        podzielZakresy(ileBit, 0, ileKolorow - 1);
    else {
        for (int i = 0; i < ileKolorow; i++)
            dopasowanaPaleta[i] = paleta[i];
        dopasowanychKolorow = ileKolorow;
    }

}

// znajdz najblizszego sasiada w dopasowanej palecie i zwroc jego indeks
int znajdzNajlbizszegoSasiada(SDL_Color kolor) {
    int minRoznica = INT_MAX;   // minimalna roznica znaleziona w 
    int roznica{};              // roznica liczona w kazdej iteracji
    int minIndeks{};            // indeks najblizszego znalezionego koloru


    // przeszukaj cala palete dopasowanych barw 
    for (int i = 0; i < dopasowanychKolorow; i++) {

        // sumuj roznice dla poszczegolnych skladowych koloru
        roznica = 0;
        roznica += abs((int)kolor.r - (int)dopasowanaPaleta[i].r);
        roznica += abs((int)kolor.g - (int)dopasowanaPaleta[i].g);
        roznica += abs((int)kolor.b - (int)dopasowanaPaleta[i].b);

        // sprawdz czy aktualna roznica jest minimalna
        if (roznica < minRoznica) {
            minIndeks = i;
            minRoznica = roznica;
        }

    }

    return minIndeks;
}

void Funkcja1() {

    //...

    SDL_UpdateWindowSurface(window);
}

void Funkcja2() {

    SDL_Color kolor;
    int R, G, B;
    ileKolorow = 0;
    dopasowana = false;

    for (int y = 0; y < wysokosc / 2; y++)
    {
        for (int x = 0; x < szerokosc / 2; x++)
        {
            kolor = getPixel(x, y);
            R = kolor.r;
            G = kolor.g;
            B = kolor.b;

            // RRGGB
            // Szukanie arytmetycznie najblizszego sasiada danej wartosci
            R = round(R * 3.0 / 255.0);
            G = round(G * 3.0 / 255.0);
            B = round(B * 1.0 / 255.0);

            // przeksztalcenie z powrotem na liczbe w pelnym zakresie
            R = R * 255.0 / 3.0;
            G = G * 255.0 / 3.0;
            B = B * 255.0 / 1.0;

            kolor.r = R;
            kolor.g = G;
            kolor.b = B;

            // dodaj kolor do palety
            sprawdzKolor(kolor);
            setPixel(x + (szerokosc / 2), y, R, G, B);

        }
    }

    wyswieltPalete();

    SDL_UpdateWindowSurface(window);
}

void Funkcja3() {

    //...

    SDL_UpdateWindowSurface(window);
}

void Funkcja4() {

    //...

    SDL_UpdateWindowSurface(window);
}

void Funkcja5() {

    //...

    SDL_UpdateWindowSurface(window);
}

//https://muthu.co/reducing-the-number-of-colors-of-an-image-using-median-cut-algorithm/
void Funkcja6() {

    int dopasowanyIndeks{};
    SDL_Color kolor;

    // skorzytaj z algorytu median cut do dopasowania palety
    dopasowana = true;
    medianCut();

    for (int y = 0; y < wysokosc / 2; y++) {
        for (int x = 0; x < szerokosc / 2; x++) {
            kolor = getPixel(x, y);
            dopasowanyIndeks = znajdzNajlbizszegoSasiada(kolor);
            setPixel(x + szerokosc / 2, y, dopasowanaPaleta[dopasowanyIndeks].r, dopasowanaPaleta[dopasowanyIndeks].g, dopasowanaPaleta[dopasowanyIndeks].b);
        }
    }

    wyswieltPalete();

    SDL_UpdateWindowSurface(window);
}

void Funkcja7() {

    //...

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
    //cout << aktualnyKolor << ": [" << (int)kolor.r << "," << (int)kolor.g << "," << (int)kolor.b << "]" << endl;
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
    while (!done) {
        SDL_WaitEvent(&event);
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