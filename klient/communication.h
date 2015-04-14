/* 
 * File:   communication.h
 * Author: hania
 *
 */
#include <ncurses.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <iostream>


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

#define BUFSIZE 10000
#define SERVER_PORT 1279
#define QUEUE_SIZE 100

char *server =  (char *)"192.168.1.10";
short service_port = 2000;	
int documentRozmiar = 0;
char bufor[2];
int litera;
int sckSend, sckRcv, sckServ, odp;
int nBind, nListen;
int nFoo = 1, result = 0;
socklen_t nTmp;
bool insertMode=true;
struct sockaddr_in sckSendAddr, sckRcvAddr, sckServAddr;
pthread_t idRead, idWrite;
struct Message
{
    int rodzajWiadomosci;//POLACZENIE lub PRZESLANIE_ZNAKU
    int port;//port na którym użytkownik tworzy socket do odbioru wiadomości (tylko połączenie)
    int offset;//zmiana pozycji kursora 
	int znak;//przesyłany znak (tylko wysylanie znaku)                           
    int trybWprowadzania;//wybieramy ze stalych INSERT, CHANGE (tylko wysylanie znaku)
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

