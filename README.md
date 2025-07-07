# System Zarządzania Narzędziami

## Opis

Ten projekt to aplikacja konsolowa w języku C, służąca do zarządzania zestawem narzędzi. Umożliwia dodawanie, edytowanie, usuwanie, sortowanie oraz wyszukiwanie narzędzi, a także ich zapis i odczyt z pliku `narzedzia.txt`.

## Pliki

- `wyporzyczalnia_narzedzi.cpp` – główny plik źródłowy zawierający logikę aplikacji.
- `narzedzia.txt` – plik z danymi narzędzi w formacie CSV.
- `README.md` – ten plik, zawierający dokumentację projektu.

## Struktura danych

Każde narzędzie opisane jest przez następujące pola:
- `id` – unikalny identyfikator narzędzia
- `nazwa` – nazwa narzędzia
- `kategoria` – kategoria (np. Ręczne, Elektryczne, Pomiarowe)
- `stan` – aktualny stan: `dostepne`, `wypozyczone`, `w naprawie`
- `dataOstatniegoSerwisu` – data ostatniego serwisu w formacie `YYYY-MM-DD`
- `liczbaWypozyczen` – liczba wypożyczeń narzędzia

## Jak uruchomić

1. **Skompiluj program**:
   ```bash
   gcc -o narzedzia wyporzyczalnia_narzedzi.c
   ```

2. **Uruchom program**:
   ```bash
   ./narzedzia
   ```

3. **Po uruchomieniu** skorzystaj z menu, aby zarządzać narzędziami.

## Wymagania

- Kompilator zgodny z ANSI C (np. `gcc`)
- System obsługujący wejście/wyjście w terminalu

## Autor
Krzysztof Zabielski