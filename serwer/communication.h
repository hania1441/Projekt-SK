/* 
 * File:   communication.h
 * Author: mariuszl
 *
 */

#ifndef COMMUNICATION_H
#define	COMMUNICATION_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif


//stałe określające rodzaj wiadomości
#define POLACZENIE 1
#define PRZESLANIE_ZNAKU 2
#define ZMIANA_POZYCJI 3

//stałe określajace tryb zmiany litery
#define INSERT 1
#define CHANGE 2


struct Message
{
    int rodzajWiadomosci;//wybieramy ze stałych POLACZENIE, PRZESLANIE_ZNAKU lub ZMIANA_POZYCJI
    int port;//port na którym użytkownik tworzy socket do odbioru wiadomości (tylko połączenie)
    int offset;//zmiana pozycji kursora 
	int znak;//przesyłany znak (tylko wysylanie znaku)
    int trybWprowadzania;//wybieramy ze stalych INSERT, CHANGE (przy wysylaniu znaku)
};


//struktura zawierająca informacje o nowych zmianach
struct Change
{
    int pozycja;//pozycja znaku 
    int znak;//przesyłany znak
    int tryb;//tryb wstawiania znaku
    int rozmiar;
    bool wlasciciel;
};



#endif	/* COMMUNICATION_H */

