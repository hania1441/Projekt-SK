/* 
 * File:   main.cpp
 * Author: hania
 *
 */

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include <string>
#include "communication.h"
#include "repozytorium.h"
#include "structures.h"

//rozmiar kolejki przychodzących połaczeń i danych
#define QUEUE_SIZE 10000


using namespace std;

//repozytorium
repozytorium* userTab;



int main(int argc, char** argv) {
    //sprawdzenie wprowadzenia numeru portu przy uruchamianiu serwera
    if(argc < 2)
    {
        cout<<"Nie podano numeru portu"<<endl;
        exit(1);
    }
    
    //tablica z użytkownikami
    userTab = new repozytorium();
    
    //sockety
    int socketPoloczeniowy;
    
    //potrzebne porty
    int portPolaczeniowy = atoi(argv[1]);

    //struktury połaczeniowe
    sockaddr_in adresPoloczeniowy;


   //czyszczenie i wypelnienie struktury
   memset(&adresPoloczeniowy, 0, sizeof(sockaddr));
   adresPoloczeniowy.sin_family = AF_INET;
   adresPoloczeniowy.sin_addr.s_addr = htonl(INADDR_ANY);
   adresPoloczeniowy.sin_port = htons(portPolaczeniowy);
   
   //utworzenie socketa 
   socketPoloczeniowy = socket(AF_INET, SOCK_STREAM, 0);
   if (socketPoloczeniowy < 0)
   {
       cout<<"Nie można utowrzyć socketa połączeniowego"<<endl;
       exit(1);
   }
   
   //bindowanie socketa
   if (bind(socketPoloczeniowy, (sockaddr*)&adresPoloczeniowy, sizeof(sockaddr)) < 0)
   {
       cout<<"Nie można zbindować socketa połączeniowego"<<endl;
       exit(1);
   }
   
   //ustawienie kolejki połączeń
   if (listen(socketPoloczeniowy, QUEUE_SIZE) < 0)
   {
       cout<<"Nie można ustawić rozmiaru kolejki dla socketa połączeniowego"<<endl;
   }
   
   
   //pętla odbierająca nowe połączenia 
   socklen_t temp = sizeof(sockaddr);
   int nowePoloczenie;
   sockaddr_in nowyAdres;
   Message* newMessage = new Message;
   
   while(true)
   {
       memset(&nowyAdres, 0, sizeof(nowyAdres));
       nowePoloczenie = accept(socketPoloczeniowy, (sockaddr*)&nowyAdres, &temp); 

       read(nowePoloczenie, newMessage, sizeof(Message));
       
       cout<<"Połączono"<<endl;
       
       if(newMessage->rodzajWiadomosci == POLACZENIE)
       {   
           cout<<"Nowe polaczenie"<<endl;
           User* newUser = new User;
           newUser->userAdress.sin_family = AF_INET;
           newUser->userAdress.sin_addr = nowyAdres.sin_addr;
           newUser->userAdress.sin_port = htons(newMessage->port);
           newUser->userSocket = socket(AF_INET, SOCK_STREAM, 0);
           connect(newUser->userSocket, 
                   (struct sockaddr*) &newUser->userAdress, 
                   sizeof(newUser->userAdress));
           
           cout<<inet_ntoa(nowyAdres.sin_addr)<<endl;
           
           int rozmiar = newUser->cursorPosition = userTab->getDocumentSize();
           write(newUser->userSocket, &rozmiar, sizeof(int));
           rozmiar *= sizeof(int);
           if(rozmiar)
            {
                cout<<"wysylam docsa"<<endl;
                int * doc = userTab->getDocument();
                //int tab [] = {78, 78, 78 , 78};
                write(newUser->userSocket, doc, rozmiar);
                delete doc;
            }
           userTab->addUser(newUser);
           //close(newUser->userSocket);
       }else if(newMessage->rodzajWiadomosci == PRZESLANIE_ZNAKU)
       {
           cout<<"mam literke!!    "<<inet_ntoa(nowyAdres.sin_addr)<<endl;
           cout<<newMessage->znak<<endl;
           
           userTab->addLetter(newMessage->znak, newMessage->trybWprowadzania, 
                   string(inet_ntoa(nowyAdres.sin_addr)), newMessage->offset);
            
       }else if(newMessage->rodzajWiadomosci == ZMIANA_POZYCJI)
       {
           cout<<"zmiana pozycji!!    "<<inet_ntoa(nowyAdres.sin_addr)<<endl;
           userTab->changeCursorPosition(string(inet_ntoa(nowyAdres.sin_addr)), newMessage->offset);
       }
       
       close(nowePoloczenie);
       
   }   
    return 0;
}


