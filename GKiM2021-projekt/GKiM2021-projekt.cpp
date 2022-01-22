#include <exception>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL.h>
#include <string>
using namespace std;

SDL_Window* window = NULL;
SDL_Surface* screen = NULL;

#define szerokosc 512
#define wysokosc 340

#define tytul "GKiM2021 - projekt"

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
int closestBW(int BW);
void createpalette();
int najblizszaDopasowana(int* R, int* G, int* B, int* bladR, int* bladG, int* bladB);
void ladujBMP(char const* nazwa, int x, int y);

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
int ideksy[szerokosc / 2*wysokosc / 2];
int ileKolorow = 0;
int dopasowanychKolorow = 0;
int dopasowana = true;
int ileZapisanych = 0;
bool tablica40bitow[40]{};
void wybierzObrazek();

void waitEvent(string s, bool* task) {
    bool done = false;
    SDL_Event event;
    cout << s;
    while (!done) {
        SDL_WaitEvent(&event);
        switch (event.type) {
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_1)
            {
                *task = true;
                done = true;
            }
            else if (event.key.keysym.sym == SDLK_2)
            {
                *task = false;
                done = true;
            }
        }
    }

    //cout << "task" << task;
}
void menu() {
   
    //SDL_Event event;

    //cout << "1. Odczyt bmp\n2. Odczyt nasze ";
    bool quit = true;
    while (quit)
    {
        bool kolor = 0;                 //wybór między paletą kolorową, a skalą szarości (true - kolorowa, false - skala szarości)
        bool odczyt = 0;                //wybór odczytu lub zapisu pliku (true - odczyt, false - zapis)
        bool dithering = 0;             //wybór czy ma wykonać się funkcja z ditheringiem czy nie (true - wykonywanie ditheringu, false - nie wykonywanie ditheringu)
        bool narzucona = 0;             //wybór między paletą narzucona, a dedykowaną (true - narzucona, false - dedykowana)
        bool metoda = 0;                //wybór między przesunięciem, a najbliższym sąsiadem (true - przesunięcia bitowe, false - najbliższy sąsiad)
        bool wybor = false;
        bool done = false;
        while (!done) {
            //SDL_WaitEvent(&event);
            waitEvent("1. Odczyt bmp\n2. Odczyt nasze", &odczyt);

            if (odczyt)
            {
                wybierzObrazek();


                waitEvent("\nCzy obraz ma byc kolorowy?\n1. Tak\n2. Nie", &kolor);

                if (kolor)
                    waitEvent("\nWybierz tryb koloru:\n1. Paleta narzucona\n2. Paleta dopasowana", &narzucona);


                waitEvent("\nCzy obraz ma byc poddany ditheringowi?\n1. Tak\n2. Nie", &dithering);


                if (kolor == true && narzucona == true && dithering == false)
                    waitEvent("\nWybierz metode:\n1. Przesuniecie bitowe\n2. Poszukiwanie najblizszego sasiada", &metoda);

                waitEvent("\nCzy wyswietlic wybrane opcje?\n1. Tak\n2. Nie", &wybor);
                done = true;
            }
            else if (!odczyt) {
                Funkcja8();
                done = true;
            }

            if (done) break;
        }

        if (kolor == true) {
            if (narzucona == true) {
                if (dithering == true) {
                    Funkcja3();
                }
                else {
                    if (metoda == true) {
                        Funkcja1();
                    }
                    else {
                        Funkcja2();
                    }
                }
            }
            else {
                if (dithering == true) {
                    Funkcja7();
                }
                else {
                    Funkcja6();
                }
            }
        }
        else {
            if (dithering == true) {
                Funkcja5();
            }
            else {
                Funkcja4();
            }
        }


        if (wybor) {
            if (kolor)
                cout << "\nSkala szarosci: tak";
            else
                cout << "\nSkala szarosci: nie";

            if (narzucona)
                cout << "\nTryb kolorow: paleta narzucona";
            else
                cout << "\nTryb kolorow: paleta dedykowana";

            if (dithering)
                cout << "\nDithering: tak";
            else
                cout << "\nDithering: nie";

            if (kolor == true && narzucona == true) {
                if (metoda)
                    cout << "\nMetoda: Przesuniecie bitowe";
                else
                    cout << "\nMetoda: Poszukiwanie najblizszego sasiada";
            }
        }
        waitEvent("\n1. Kontynuuj\n2. Koniec programu ", &quit);
    }

}

void wybierzObrazek() {
        cout << "\nPodaj literke obrazka(a-k)\nWcisnij escape po jego wybraniu";
        // główna pętla programu
        bool done = false;
        SDL_Event event;
        
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
    }
// uzywana do debuggowania, wypisuje w konsoli wartosci kolorow
void wypiszPalete() {

    cout << "Zwykla:\n";
    // for (int i = 0; i < ileKolorow; i++) {
    //     cout << i << ": [" << (int)paleta[i].r << "," << (int)paleta[i].g << "," << (int)paleta[i].b << "]" << endl;

    // }


    cout << "Dopasowana:\n";
    for (int i = 0; i < dopasowanychKolorow; i++) {
        cout << i << ": [" << (int)dopasowanaPaleta[i].r << "," << (int)dopasowanaPaleta[i].g << "," << (int)dopasowanaPaleta[i].b << "]" << endl;

    }
}

// wyswietl palete uzywana podczas konwersji obrazka
void wyswieltPalete() {
    int i = 0;

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

                setPixel(x, y + wysokosc / 2, paleta[i].r, paleta[i].g, paleta[i].b);
            }
        }
    // wypiszPalete();
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




//Funkcja zapisuje dane do pliku zamieniając wcześniej dane z postaci binarnej na postać dziesiętną

void zapisz5bitDoPliku(ofstream& wyjscie) {

    Uint8 zmienna = 0;
    int iterator = 0;
    for (int i = 0; i < ileZapisanych; i++) {
        zmienna += tablica40bitow[i];
        iterator++;

        //zapisywanie kolejnych bajtów do pliku

        if (iterator == 8) {
            wyjscie.write((char*)&zmienna, sizeof(Uint8));
            iterator = 0;
            zmienna = 0;
        }
        zmienna <<= 1;

    }

}


//Funkcja zeruje tablice przechowującą wcześniej zapisane bity

void zerujTabliceBitow() {

    for (int i = 0; i < ileZapisanych; i++) {
        tablica40bitow[i] = 0;
    }
}


//Funkcja zamienia liczby z postaci dziesiętnej na postać binarną oraz zapisuje kolejne bity do tablicy tablica40bitów

void konwersja10na2(ofstream& wyjscie, int liczba) {

    int i = 0, tab[5]{};
    while (i < 5) //dopóki liczba będzie różna od zera
    {
        tab[i] = liczba % 2;
        liczba /= 2;
        i++;
    }

    //zapisanie kolejnych bitów do tablicy 

    for (int j = i - 1; j >= 0; j--) {
        tablica40bitow[ileZapisanych] = tab[j];
        ileZapisanych++;
    }


    //jeśli w tablicy przechowującej bity zapisana została sekwencja kolejnych 40 bitów zostaje wywołana funkcja do zapisywania danych do pliku

    if (ileZapisanych == 40) {
        zapisz5bitDoPliku(wyjscie);
        zerujTabliceBitow();
        ileZapisanych = 0;
    }

}

void dopiszDoPliku(ofstream& wyjscie){

    if (ileZapisanych != 0) {
        zapisz5bitDoPliku(wyjscie);
    }
    ileZapisanych = 0;

}

void createpaletteF1() {

    SDL_Color color;
    dopasowanychKolorow = 0;
    for (int i = 0; i < 4; i++)
    {
        color.r = i * 64;

        for (int j = 0; j < 4; j++)
        {
            color.g = j * 64;

            for (int k = 0; k < 2; k++)
            {
                color.b = k * 128;
                dopasowanaPaleta[dopasowanychKolorow] = color;
                
                dopasowanychKolorow++;
            }
        }
    }   
}

//Funkcja wykonująca przesunięcie bitowe
void Funkcja1() {

    ofstream wyjscie("obrazProjekt.bin", ios::binary);

    ileKolorow = 0;
    createpaletteF1();
    dopasowana = true;

    SDL_Color kolor;
    Uint16 szerokoscObrazka = szerokosc / 2;
    Uint16 wysokoscObrazka = wysokosc / 2;
    Uint8 wartosc = 0;
    int R, G, B;
    char wariant[] = "p"; 

    //bw, przesunieta (1), dopasowana, narzucona

    cout << "1. Przesuniecie bitowe\n";
    wyjscie.write((char*)&wariant, sizeof(char));

    for (int x = 0; x < szerokosc / 2; x++) {
        for (int y = 0; y < wysokosc / 2; y++) {


            kolor = getPixel(x, y);
            R = kolor.r;
            G = kolor.g;
            B = kolor.b;

            //przesunięcie bitowe każdej z poszczególnych składowych
            R = R >> 6;
            G = G >> 6;
            B = B >> 7;


            //powrót do wartości składowych RGB w pełnym zakresie
            R = R << 6;
            G = G << 6;
            B = B << 7;


            kolor.r = R;
            kolor.g = G;
            kolor.b = B;
          
            int indeks = znajdzNajlbizszegoSasiada(kolor);
            konwersja10na2(wyjscie, indeks);
            setPixel(x + szerokoscObrazka, y, dopasowanaPaleta[indeks].r,dopasowanaPaleta[indeks].g,dopasowanaPaleta[indeks].b);

        }
    }

    dopiszDoPliku(wyjscie);

    wyswieltPalete();
    wyjscie.close();
    SDL_UpdateWindowSurface(window);
}

void Funkcja2() {

    ofstream wyjscie("obrazProjekt.bin", ios::binary);

    SDL_Color kolor;
    int R, G, B;
    char wariant[] = "n"; 
    ileKolorow = 0;
    createpalette();

    cout << "2. Poszukiwanie najblizszego sasiada\n";

    wyjscie.write((char*)&wariant, sizeof(char));

    for (int x = 0; x < szerokosc / 2; x++)
    {
        for (int y = 0; y < wysokosc / 2; y++)
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
            int indeks = znajdzNajlbizszegoSasiada(kolor);
            konwersja10na2(wyjscie, indeks);
            setPixel(x + szerokosc/2, y, dopasowanaPaleta[indeks].r,dopasowanaPaleta[indeks].g,dopasowanaPaleta[indeks].b);

        }
    }

    dopiszDoPliku(wyjscie);
    wyswieltPalete();
    wyjscie.close();
    SDL_UpdateWindowSurface(window);
    
}
void createpalette() {

    SDL_Color color;
    dopasowanychKolorow = 0;
    for (int i = 0; i < 4; i++)
    {
        color.r = i * 85;

        for (int j = 0; j < 4; j++)
        {
            color.g = j * 85;

            for (int k = 0; k < 2; k++)
            {
                color.b = k * 255;
                dopasowanaPaleta[dopasowanychKolorow] = color;
                
                dopasowanychKolorow++;
            }
        }
    }   
}

// void closest(int* R, int* G, int* B, int* bladR, int* bladG, int* bladB) {

//     createpalette();

//     int oldR = *R, oldG = *G, oldB = *B;
//     int diffR = 0, diffG = 0, diffB = 0;
//     int smallestDiffR = 255, smallestDiffG = 255, smallestDiffB = 255;

//     for (int i = 0; i < 32; i++) {
//         diffR = abs(oldR - dopasowanaPaleta[i].r);
//         if (diffR < smallestDiffR) {
//             smallestDiffR = diffR;
//             *R = dopasowanaPaleta[i].r;
//             *bladR = oldR - *R;
//         }

//         diffG = abs(oldG - dopasowanaPaleta[i].g);
//         if (diffG < smallestDiffG) {
//             smallestDiffG = diffG;
//             *G = dopasowanaPaleta[i].g;
//             *bladG = oldG - *G;
//         }

//         diffB = abs(oldB - dopasowanaPaleta[i].b);
//         if (diffB < smallestDiffB) {
//             smallestDiffB = diffB;
//             *B = dopasowanaPaleta[i].b;
//             *bladB = oldB - *B;
//         }
//     }

// }
void Funkcja3() {
    ofstream wyjscie("obrazProjekt.bin", ios::binary);

    SDL_Color kolor;
    dopasowana = true;
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
    char wariant[] = "n"; 
    createpalette();

    int R, G, B;
    int i = 0;
    int indeks{};
    cout << "\n3. dithering\n";
    wyjscie.write((char*)&wariant, sizeof(char));

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

            indeks = najblizszaDopasowana(&R, &G, &B, &bladR, &bladG, &bladB);
            konwersja10na2(wyjscie, indeks);
            R = dopasowanaPaleta[indeks].r;
            G = dopasowanaPaleta[indeks].g;
            B = dopasowanaPaleta[indeks].b;
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

    dopiszDoPliku(wyjscie);
    wyjscie.close();
    wyswieltPalete();

    SDL_UpdateWindowSurface(window);

}

void Funkcja4() {
    ofstream wyjscie("obrazProjekt.bin", ios::binary);

    ileKolorow = 0;
    int R, G, B;
    int BW;
    int indeks = 0;
    SDL_Color kolor;
    char wariant[] = "b"; 

    cout << "4. Skala szarosci\n";
    wyjscie.write((char*)&wariant, sizeof(char));

    //createBWpalette();
    for (int x = 0; x < szerokosc / 2; x++) {
        for (int y = 0; y < wysokosc / 2; y++) {

            kolor = getPixel(x, y);
            R = kolor.r;
            G = kolor.g;
            B = kolor.b;

            BW = 0.299 * R + 0.587 * G + 0.114 * B;
            indeks = closestBW(BW);
            konwersja10na2(wyjscie, indeks);
            BW = dopasowanaPaleta[indeks].r;
            //BW = BW >> 3;
           // BW = BW << 3;
         
            setPixel(x + szerokosc / 2, y, BW, BW, BW);

        }
    }
    
    dopiszDoPliku(wyjscie);
    wyjscie.close();
    wyswieltPalete();

    SDL_UpdateWindowSurface(window);

}

void createBWpalette() {
   
    SDL_Color color;
    for (int i = 0; i < 32; i++) {
        color.r = i * 8;
        color.g = i * 8;
        color.b = i * 8;
        dopasowanaPaleta[i] = color;
    }
    dopasowanychKolorow = 32;
}

int closestBW(int BW) {
    createBWpalette();
    int diff = 0;
    int smallestDiff = 255;
    int oldBW = BW;
    for (int i = 0; i < 32; i++) {
        diff = abs(oldBW - dopasowanaPaleta[i].r);
        if (diff < smallestDiff) {
            smallestDiff = diff;
            BW = i;
        }

    }
    return BW;
}
void Funkcja5() {

    ofstream wyjscie("obrazProjekt.bin", ios::binary);

    float bledy[(szerokosc / 2) + 2][(wysokosc / 2) + 1];
    memset(bledy, 0, sizeof(bledy));
    ileKolorow = 0;
    int przesuniecie = 1;
    int blad = 0;
    int BW = 0;
    int oldBW = 0;
    char wariant[] = "b"; 
    int indeks = 0;
    SDL_Color kolor;

    cout << "5. Skala szarosci z ditheringiem\n";

    wyjscie.write((char*)&wariant, sizeof(char));

    for (int x = 0; x < szerokosc / 2; x++) {
        for (int y = 0; y < wysokosc / 2; y++) {
            kolor = getPixel(x, y);
            BW = 0.299 * kolor.r + 0.587 * kolor.g + 0.114 * kolor.b;
            BW += bledy[x + przesuniecie][y];
            oldBW = BW;
            indeks = closestBW(BW);
            BW = dopasowanaPaleta[indeks].r;
            // indeks = znajdzNajlbizszegoSasiada(kolor);
            konwersja10na2(wyjscie, indeks);
            blad = oldBW - BW;//r,g,b sa takie same
            setPixel(x + szerokosc / 2, y, BW, BW, BW);
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

    
    dopiszDoPliku(wyjscie);    
    wyjscie.close();
    wyswieltPalete();

    SDL_UpdateWindowSurface(window);

}

void zapiszPalete(ofstream& wyjscie){

    for(int i = 0;i<dopasowanychKolorow;i++){
        wyjscie.write((char*)&dopasowanaPaleta[i].r, sizeof(Uint8));
        wyjscie.write((char*)&dopasowanaPaleta[i].g, sizeof(Uint8));
        wyjscie.write((char*)&dopasowanaPaleta[i].b, sizeof(Uint8));
    }

}

//https://muthu.co/reducing-the-number-of-colors-of-an-image-using-median-cut-algorithm/
void Funkcja6() {
    ofstream wyjscie("obrazProjekt.bin", ios::binary);

    int indeks{};
    char wariant[] = "d"; 
    SDL_Color kolor;

    cout << "6. poszukiwanie najblizszego sasiada dla dopasowanej palety\n";
    wyjscie.write((char*)&wariant, sizeof(char));

    // skorzytaj z algorytu median cut do dopasowania palety
    medianCut();
    zapiszPalete(wyjscie);

    for (int x = 0; x < szerokosc / 2; x++) {
        for (int y = 0; y < wysokosc / 2; y++) {
            kolor = getPixel(x, y);
            indeks = znajdzNajlbizszegoSasiada(kolor);
            konwersja10na2(wyjscie, indeks);
            setPixel(x + szerokosc / 2, y, dopasowanaPaleta[indeks].r, dopasowanaPaleta[indeks].g, dopasowanaPaleta[indeks].b);
        }
    }
    
    dopiszDoPliku(wyjscie);
    wyswieltPalete();

    SDL_UpdateWindowSurface(window);
    wyjscie.close();
}

int najblizszaDopasowana(int* R, int* G, int* B, int* bladR, int* bladG, int* bladB) {

    int oldR = *R, oldG = *G, oldB = *B;
    int minRoznica = INT_MAX;   // minimalna roznica znaleziona w 
    int roznica = 0;              // roznica liczona w kazdej iteracji
    int min_indeks = 0;
    // przeszukaj cala palete dopasowanych barw 
    for (int i = 0; i < dopasowanychKolorow; i++) {

        // sumuj roznice dla poszczegolnych skladowych koloru
        roznica = 0;
        roznica += abs((int)oldR - (int)dopasowanaPaleta[i].r);
        roznica += abs((int)oldG - (int)dopasowanaPaleta[i].g);
        roznica += abs((int)oldB - (int)dopasowanaPaleta[i].b);

        // sprawdz czy aktualna roznica jest minimalna
        if (roznica < minRoznica) {
            minRoznica = roznica;

            *R = dopasowanaPaleta[i].r;
            *G = dopasowanaPaleta[i].g;
            *B = dopasowanaPaleta[i].b;

            *bladR = oldR - *R;
            *bladG = oldG - *G;
            *bladB = oldB - *B;

            min_indeks = i;
        }

    }

    return min_indeks;
    

}

void Funkcja7() {
    ofstream wyjscie("obrazProjekt.bin", ios::binary);

    SDL_Color kolor;
    float bledyR[(szerokosc / 2) + 2][(wysokosc / 2) + 1];
    float bledyG[(szerokosc / 2) + 2][(wysokosc / 2) + 1];
    float bledyB[(szerokosc / 2) + 2][(wysokosc / 2) + 1];
    float bledy[(szerokosc / 2) + 2][(wysokosc / 2) + 1];

    int przesuniecie = 1;
    int bladR = 0, bladG = 0, bladB = 0;
    int R, G, B;
    int i = 0;
    int indeks = 0;
    char wariant[] = "d"; 

    memset(bledyR, 0, sizeof(bledyR));
    memset(bledyG, 0, sizeof(bledyG));
    memset(bledyB, 0, sizeof(bledyB));
    memset(bledy, 0, sizeof(bledy));


    cout << "7. dithering dla dopasowanej palety\n";
    wyjscie.write((char*)&wariant, sizeof(char));

    medianCut();
    zapiszPalete(wyjscie);

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


            indeks = najblizszaDopasowana(&R, &G, &B, &bladR, &bladG, &bladB);
            konwersja10na2(wyjscie, indeks);
            R = dopasowanaPaleta[indeks].r;
            G = dopasowanaPaleta[indeks].g;
            B = dopasowanaPaleta[indeks].b;

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
    dopiszDoPliku(wyjscie);
    wyswieltPalete();

    SDL_UpdateWindowSurface(window);
    wyjscie.close();
}

//Funkcja odczytuje w pętli zewnętrznej kolejne bajty w postaci zmiennej typu Uint8 natomiast w pętli wewnętrznej rozbija bajt na osiem bitów i zapisuje je pod kolejnymi indeksami w tablicy
void konwersjaUint8naBool(ifstream& wejscie, bool* skladowa) {

    int iterator = 0;
    Uint8 odczytaneDane = 0;
    for (int i = 0; i < 5; i++) {
        wejscie.read((char*)&odczytaneDane, sizeof(Uint8));
        for (int j = 0; j < 8; j++) {
            if ((int)odczytaneDane >= 128) {
                skladowa[iterator] = 1;
            }
            else
                skladowa[iterator] = 0;
            odczytaneDane <<= 1;
            iterator++;
        }
    }

}

void odczytajPalete(ifstream& wejscie){
    for(int i=0; i<32;i++){
        wejscie.read((char*)&dopasowanaPaleta[i].r, sizeof(Uint8));
        wejscie.read((char*)&dopasowanaPaleta[i].g, sizeof(Uint8));
        wejscie.read((char*)&dopasowanaPaleta[i].b, sizeof(Uint8));
    }
    dopasowanychKolorow = 32;
}

//aktulnie funkcja odczytuje z pliku obraz zapisany za pomocą funkcji, w której wykonywane jest przesunięcie bitowe
void Funkcja8() {

    Uint16 szerokoscObrazka = szerokosc / 2;
    Uint16 wysokoscObrazka = wysokosc / 2;
    int R = 0, G = 0, B = 0;
    bool skladowa[40]{ 0 };
    int indeks = 0;
    char wariant[] = " "; 
    ifstream wejscie("obrazProjekt.bin", ios::binary);

    wejscie.read((char*)&wariant, sizeof(char));

    switch((int)wariant[0]){
        case (int)'p':
            createpaletteF1();
            break;
        case (int)'b':
            createBWpalette();
            break;
        case (int)'n':
            createpalette();
            break;
        case (int)'d':
            odczytajPalete(wejscie);
            break;
    }

    cout << "8. Odczyt z pliku\n";

    konwersjaUint8naBool(wejscie, skladowa);

    int osiemBitow = 0;
    int i = 0;
    for (int x = 0; x < szerokosc / 2; x++) {
        for (int y = 0; y < wysokosc / 2; y++) {

            if (osiemBitow == 8) {
                konwersjaUint8naBool(wejscie, skladowa);
                osiemBitow = 0;
                i = 0;
            }
            osiemBitow++;

            indeks = 0;
            for(int j = 0; j < 5; j++){
                indeks += skladowa[i];
                if(j!=4)
                    indeks <<= 1;
                i++;
            }

            // R = skladowa[i];
            // R <<= 1;
            // i++;
            // R += skladowa[i];
            // R <<= 6;

            // i++;
            // G = skladowa[i];
            // G <<= 1;
            // i++;
            // G += skladowa[i];
            // G <<= 6;

            // i++;
            // B = skladowa[i];
            // B <<= 7;
            // i++;

            R = dopasowanaPaleta[indeks].r;
            G = dopasowanaPaleta[indeks].g;
            B = dopasowanaPaleta[indeks].b;

            setPixel(x + szerokoscObrazka, y + wysokoscObrazka, R, G, B);
            //SDL_UpdateWindowSurface(window);
        }

    }

    SDL_UpdateWindowSurface(window);
    wejscie.close();
}

void kompresjaRLE(int wejscie[], int dlugosc) {
    int i = 0;
    //dopoki wszystkie bajty nie sa skompresowane
    while (i < dlugosc)
    {
        //sekwencja powtarzania sie co najmniej 2 bajtow
        if (i < dlugosc - 1 && wejscie[i] == wejscie[i + 1])
        {
            //mierzymy dlugosc sekwencji
            int j = 0;
            while ((i + j < dlugosc - 1 && wejscie[i + j] == wejscie[i + j + 1]) && j < 254)
            {
                j++;
            }
            //wypisujemy spakowana sekwencje
            cout << j + 1 << ", " << wejscie[i + j] << ", ";
            //przesuwamy wskaznik o dlugosc sekwencji 
            i += (j + 1);
        }
        //sekwencja roznych bajtow 
        else
        {
            //mierzymy dlugosc sekwencji
            int j = 0;
            while ((i + j < dlugosc - 1 && wejscie[i + j] != wejscie[i + j + 1]) && j < 254)
            {
                j++;
            }
            //dodajemy jeszcze koncowke
            if (i + j == dlugosc - 1 && j < 254)
            {
                j++;
            }
            //wypisujemy spakowana sekwencje
            cout << (int)0 << ", " << j << ", ";
            for (int k = 0; k < j; k++)
            {
                cout << wejscie[i + k] << ", ";
            }

            if (j % 2 != 0)
            {
                cout << (int)0 << ", ";
            }
            //przesuwamy wskaznik o dlugosc sekwencji
            i += j;
        }
    }
}
void dekompresjaRLE(int wejscie[], int dlugosc)
{
    int i = 0;
    int ile = 0;
    //dopoki wszystkie bajty nie sa zdekompresowane
    while (i < dlugosc)
    {
        //sekwencja powtarzajacych sie bajtow 
        if (wejscie[i] > 0)
        {
            for (int j = 0; j < wejscie[i]; j++)
            {
                cout << wejscie[i + 1] << ", ";
            }
            //przesuwamy wskaznik o dlugosc sekwencji
            i += 2;
        }
        else
        {
            ile = wejscie[i + 1];
            for (int j = 0; j < ile; j++)
            {
                cout << wejscie[i + 1 + j + 1] << ", ";
            }
            //przesuwamy wskaznik o dlugosc sekwencji
            i += ile + 2;
            if (ile % 2 != 0)
            {
                i++;
            }
        }
    }
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
    
    menu();
    /*
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
    }*/

    if (screen) {
        SDL_FreeSurface(screen);
    }

    if (window) {
        SDL_DestroyWindow(window);
    }


    SDL_Quit();
    return 0;
}