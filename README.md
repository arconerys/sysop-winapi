# Systemy Operacyjne

# LAB1
* Wprowadzenie, konfiguracja IDE, kompilacja
  - Flagi: Werror, Wall, pedantic, std=c++11
* Pierwszy program, uruchamianie programów wsystemie Win32, punkt wejścia, przekazywanie
argumentów
  - int _tmain(int argv, TCHAR *argv[], TCHAR *envp[]);
  - int main(int argc, char** argv)
* Wprowadzenie do obsługi plików
  - CreateFile, WriteFile, typ HANDLE
  
# LAB2
* Obsługa plików za pomocą API Win32
	- CreateFile, WriteFile, ReadFile, GetFileSize
* Parametry funkcji CreateFile
	- dwDesiredAccess,
	– dwShareMode,
	– lpSecurityAttributes,
	– dwCreationDisposition
* Porządkowanie kodu, refatkoryzacja, podział na funkcje, klasy, camelCase, linux_case, notacja węgierska

# LAB3
* Tworzenie odbrębnego procesu
	- CreateProcess
* Synchronizacja dostępu do zasobów między procesami
	- dostęp do pliku
	- wykorzystanie pliku w roli mutexa (parametry f. CreateFile)
* wykorzystanie wielu podejść w programowaniu C++
	- C-style (strtok)
	- C++style (ostream, getline)
	- C++ with features (boost)
	
# LAB4
* Wykorzystanie omówionego materiału w praktyce:
 - Stworzony program składa się z dwóch procesów, proces 1 losuje N liczb całkowitych z zakresu 1...100 i zapisuję je do pliku. N podawane jest jako parametr przy wywołaniu programu.
 - Proces 1, przed rozpoczęciem losowania i zapisu danych uruchamia proces 2, który oczekuje na dostęp do pliku (spinlock).
 - Proces 2, po uzyskaniu dostępu do pliku odczytuje zapisane w nim liczby, a następnie oblicza średnią i wyświetla ją na ekranie, proces 2 się kończy.
 - Proces 1 kończy się po zakończeniu procesu 2 (WaitForSingleObject)
 
# LAB5
* Dalsza rozbudowa programu:
	- Przygotowanie dwóch wariantów programu wykonywanego w procesie 2:
* Wariant A: poza średnią, program szuka też wartości maksymalnej i minimalnej, obliczenia wykonywane są sekwencyjnie, w jednym wątku
* Wariant B: funkcjonalność jak w wariancie A, należy stworzyć 3 dodatkowe wątki, każdy odpowiedzialny za jedną z części obliczeń (avg, min, max)
	- Funkcję niezbędne do realizacji tego etapu: HeapAlloc(), HeapFree(), CreateThread(), WaitForSingleObject(), WaitForMultipleObject().