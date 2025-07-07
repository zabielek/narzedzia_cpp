/**
 * Projekt: System Zarządzania Narzędziami
 * Opis:
 * Program pozwala na zarządzanie zestawem narzędzi (dodawanie, edycja, usuwanie, wyszukiwanie, sortowanie),
 * zapisywanie do pliku oraz wczytywanie danych z pliku CSV.
 * 
 * Format pliku danych: ID,Nazwa,Kategoria,Stan,DataOstatniegoSerwisu,LiczbaWypozyczen
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAKS_NARZEDZIA 100
#define MAKS_DLUGOSC_NAZWY 50
#define MAKS_DLUGOSC_KATEGORII 30

typedef struct {
    int id;
    char nazwa[MAKS_DLUGOSC_NAZWY];
    char kategoria[MAKS_DLUGOSC_KATEGORII];
    char stan[15]; // "dostepne", "wypozyczone", "w naprawie"
    char dataOstatniegoSerwisu[11]; // Format: YYYY-MM-DD
    int liczbaWypozyczen;
} Narzedzie;

Narzedzie narzedzia[MAKS_NARZEDZIA];
int liczbaNarzedzi = 0;

/**
 * Funkcja pomocnicza: pobiera aktualną datę w formacie YYYY-MM-DD
 */
void pobierzAktualnaDate(char* buffer) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buffer, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

/**
 * Sprawdza, czy narzędzie wymaga serwisu (więcej niż rok od ostatniego serwisu)
 */
int wymagaSerwisu(const char* dataOstatniegoSerwisu) {
    char aktualnaData[11];
    pobierzAktualnaDate(aktualnaData);

    struct tm ostatniSerwis = { 0 }, aktualny = { 0 };
    sscanf(dataOstatniegoSerwisu, "%d-%d-%d", &ostatniSerwis.tm_year, &ostatniSerwis.tm_mon, &ostatniSerwis.tm_mday);
    sscanf(aktualnaData, "%d-%d-%d", &aktualny.tm_year, &aktualny.tm_mon, &aktualny.tm_mday);

    ostatniSerwis.tm_year -= 1900;
    ostatniSerwis.tm_mon -= 1;
    aktualny.tm_year -= 1900;
    aktualny.tm_mon -= 1;

    time_t czasOstatniegoSerwisu = mktime(&ostatniSerwis);
    time_t czasAktualny = mktime(&aktualny);

    double roznicaDni = difftime(czasAktualny, czasOstatniegoSerwisu) / (60 * 60 * 24);
    return roznicaDni > 365;
}

/**
 * Dodaje nowe narzędzie do listy
 */
void dodajNarzedzie() {
    if (liczbaNarzedzi >= MAKS_NARZEDZIA) {
        printf("\nNie mozna dodac wiecej narzedzi.\n");
        return;
    }

    Narzedzie noweNarzedzie;
    noweNarzedzie.id = liczbaNarzedzi + 1;

    printf("\nPodaj nazwe narzedzia: ");
    scanf(" %49[^\n]", noweNarzedzie.nazwa);

    printf("Podaj kategorie narzedzia: ");
    scanf(" %29[^\n]", noweNarzedzie.kategoria);

    printf("Podaj stan narzedzia (dostepne, wypozyczone, w naprawie): ");
    scanf(" %14[^\n]", noweNarzedzie.stan);

    printf("Podaj date ostatniego serwisu (YYYY-MM-DD): ");
    scanf(" %10[^\n]", noweNarzedzie.dataOstatniegoSerwisu);

    noweNarzedzie.liczbaWypozyczen = 0;

    narzedzia[liczbaNarzedzi++] = noweNarzedzie;
    printf("\nNarzedzie zostalo dodane.\n");
}

/**
 * Edytuje dane istniejącego narzędzia na podstawie ID
 */
void edytujNarzedzie() {
    int id;
    printf("\nPodaj ID narzedzia do edycji: ");
    scanf("%d", &id);

    for (int i = 0; i < liczbaNarzedzi; i++) {
        if (narzedzia[i].id == id) {
            printf("Edytowanie narzedzia ID %d\n", id);
            printf("Podaj nowa nazwe (aktualna: %s): ", narzedzia[i].nazwa);
            scanf(" %49[^\n]", narzedzia[i].nazwa);

            printf("Podaj nowa kategorie (aktualna: %s): ", narzedzia[i].kategoria);
            scanf(" %29[^\n]", narzedzia[i].kategoria);

            printf("Podaj nowy stan (aktualny: %s): ", narzedzia[i].stan);
            scanf(" %14[^\n]", narzedzia[i].stan);

            printf("Podaj nowa date ostatniego serwisu (aktualna: %s): ", narzedzia[i].dataOstatniegoSerwisu);
            scanf(" %10[^\n]", narzedzia[i].dataOstatniegoSerwisu);

            printf("\nNarzedzie zostalo zaktualizowane.\n");
            return;
        }
    }

    printf("\nNie znaleziono narzedzia o podanym ID.\n");
}

/**
 * Usuwa narzędzie z listy, o ile nie jest wypożyczone
 */
void usunNarzedzie() {
    int id;
    printf("\nPodaj ID narzedzia do usuniecia: ");
    scanf("%d", &id);

    for (int i = 0; i < liczbaNarzedzi; i++) {
        if (narzedzia[i].id == id) {
            if (strcmp(narzedzia[i].stan, "wypozyczone") == 0) {
                printf("\nNie mozna usunac narzedzia, poniewaz jest wypozyczone.\n");
                return;
            }

            for (int j = i; j < liczbaNarzedzi - 1; j++) {
                narzedzia[j] = narzedzia[j + 1];
            }

            liczbaNarzedzi--;
            printf("\nNarzedzie zostalo usuniete.\n");
            return;
        }
    }

    printf("\nNie znaleziono narzedzia o podanym ID.\n");
}

/**
 * Sortuje narzędzia wg wybranego kryterium: kategoria, liczba wypożyczeń, data serwisu
 */
void sortujNarzedzia() {
    int wybor;
    printf("\nSortowanie wg:\n1. Kategorii\n2. Liczby wypozyczen\n3. Daty ostatniego serwisu\nWybor: ");
    scanf("%d", &wybor);

    for (int i = 0; i < liczbaNarzedzi - 1; i++) {
        for (int j = i + 1; j < liczbaNarzedzi; j++) {
            int porownanie = 0;
            if (wybor == 1) {
                porownanie = strcmp(narzedzia[i].kategoria, narzedzia[j].kategoria) > 0;
            }
            else if (wybor == 2) {
                porownanie = narzedzia[i].liczbaWypozyczen < narzedzia[j].liczbaWypozyczen;
            }
            else if (wybor == 3) {
                porownanie = strcmp(narzedzia[i].dataOstatniegoSerwisu, narzedzia[j].dataOstatniegoSerwisu) > 0;
            }

            if (porownanie) {
                Narzedzie temp = narzedzia[i];
                narzedzia[i] = narzedzia[j];
                narzedzia[j] = temp;
            }
        }
    }

    printf("\nNarzedzia zostaly posortowane.\n");
}

/**
 * Wyszukuje narzędzia wg nazwy, kategorii lub stanu
 */
void wyszukajNarzedzia() {
    int wybor;
    char zapytanie[MAKS_DLUGOSC_NAZWY];

    printf("\nWyszukiwanie wg:\n1. Nazwy\n2. Kategorii\n3. Stanu\nWybor: ");
    scanf("%d", &wybor);

    printf("Podaj szukana wartosc: ");
    scanf(" %49[^\n]", zapytanie);

    printf("\nWyniki wyszukiwania:\n");
    printf("ID\tNazwa\t\tKategoria\t\tStan\t\tOstatni serwis\tLiczba wypozyczen\n");
    for (int i = 0; i < liczbaNarzedzi; i++) {
        int dopasowanie = 0;
        if (wybor == 1 && strstr(narzedzia[i].nazwa, zapytanie)) {
            dopasowanie = 1;
        }
        else if (wybor == 2 && strstr(narzedzia[i].kategoria, zapytanie)) {
            dopasowanie = 1;
        }
        else if (wybor == 3 && strcmp(narzedzia[i].stan, zapytanie) == 0) {
            dopasowanie = 1;
        }

        if (dopasowanie) {
            printf("%d\t%s\t\t%s\t\t%s\t\t%s\t%d\n", narzedzia[i].id, narzedzia[i].nazwa, narzedzia[i].kategoria, narzedzia[i].stan, narzedzia[i].dataOstatniegoSerwisu, narzedzia[i].liczbaWypozyczen);
        }
    }
}

/**
 * Wyświetla wszystkie zapisane narzędzia
 */
void wyswietlNarzedzia() {
    printf("\nLista narzedzi:\n");
    printf("ID\tNazwa\t\tKategoria\t\tStan\t\tOstatni serwis\tLiczba wypozyczen\n");
    for (int i = 0; i < liczbaNarzedzi; i++) {
        printf("%d\t%s\t\t%s\t\t%s\t\t%s\t%d\n", narzedzia[i].id, narzedzia[i].nazwa, narzedzia[i].kategoria, narzedzia[i].stan, narzedzia[i].dataOstatniegoSerwisu, narzedzia[i].liczbaWypozyczen);
    }
}

/**
 * Wyświetla listę narzędzi wymagających serwisu
 */
void narzedziaWymagajaceSerwisu() {
    printf("\nNarzedzia wymagajace serwisu:\n");
    printf("ID\tNazwa\t\tKategoria\t\tStan\t\tOstatni serwis\tLiczba wypozyczen\n");
    for (int i = 0; i < liczbaNarzedzi; i++) {
        if (wymagaSerwisu(narzedzia[i].dataOstatniegoSerwisu)) {
            printf("%d\t%s\t\t%s\t\t%s\t\t%s\t%d\n", narzedzia[i].id, narzedzia[i].nazwa, narzedzia[i].kategoria, narzedzia[i].stan, narzedzia[i].dataOstatniegoSerwisu, narzedzia[i].liczbaWypozyczen);
        }
    }
}

/**
 * Zapisuje dane narzędzi do pliku narzedzia.txt
 */
void zapiszDoPliku() {
    FILE* plik = fopen("narzedzia.txt", "w");
    if (!plik) {
        printf("\nBlad podczas zapisywania do pliku.\n");
        return;
    }

    for (int i = 0; i < liczbaNarzedzi; i++) {
        fprintf(plik, "%d,%s,%s,%s,%s,%d\n", narzedzia[i].id, narzedzia[i].nazwa, narzedzia[i].kategoria, narzedzia[i].stan, narzedzia[i].dataOstatniegoSerwisu, narzedzia[i].liczbaWypozyczen);
    }

    fclose(plik);
    printf("\nDane zostaly zapisane do pliku narzedzia.txt.\n");
}

/**
 * Wczytuje dane narzędzi z pliku narzedzia.txt
 */
void wczytajZPliku() {
    FILE* plik = fopen("narzedzia.txt", "r");
    if (!plik) {
        printf("\nBlad podczas wczytywania z pliku.\n");
        return;
    }

    liczbaNarzedzi = 0;
    while (fscanf(plik, "%d,%49[^,],%29[^,],%14[^,],%10[^,],%d\n",
        &narzedzia[liczbaNarzedzi].id,
        narzedzia[liczbaNarzedzi].nazwa,
        narzedzia[liczbaNarzedzi].kategoria,
        narzedzia[liczbaNarzedzi].stan,
        narzedzia[liczbaNarzedzi].dataOstatniegoSerwisu,
        &narzedzia[liczbaNarzedzi].liczbaWypozyczen) == 6) {
        liczbaNarzedzi++;
    }

    fclose(plik);
    printf("\nDane zostaly wczytane z pliku narzedzia.txt.\n");
}

/**
 * Wyświetla menu główne aplikacji
 */
void menu() {
    int wybor;
    do {
        printf("\nMenu:\n");
        printf("1. Dodaj narzedzie\n");
        printf("2. Edytuj narzedzie\n");
        printf("3. Usun narzedzie\n");
        printf("4. Wyswietl wszystkie narzedzia\n");
        printf("5. Wyszukaj narzedzie\n");
        printf("6. Sortuj narzedzia\n");
        printf("7. Wyswietl narzedzia wymagajace serwisu\n");
        printf("8. Zapisz do pliku\n");
        printf("9. Wczytaj z pliku\n");
        printf("0. Wyjdz\n");
        printf("Wybor: ");
        scanf("%d", &wybor);

        switch (wybor) {
        case 1: dodajNarzedzie(); break;
        case 2: edytujNarzedzie(); break;
        case 3: usunNarzedzie(); break;
        case 4: wyswietlNarzedzia(); break;
        case 5: wyszukajNarzedzia(); break;
        case 6: sortujNarzedzia(); break;
        case 7: narzedziaWymagajaceSerwisu(); break;
        case 8: zapiszDoPliku(); break;
        case 9: wczytajZPliku(); break;
        case 0: printf("\nKoniec programu.\n"); break;
        default: printf("\nNieprawidlowy wybor. Sprobuj ponownie.\n");
        }
    } while (wybor != 0);
}

/**
 * Funkcja główna – punkt wejścia programu
 */
int main() {
    wczytajZPliku();
    menu();
    return 0;
}