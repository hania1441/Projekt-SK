/* 
 * File:   repozytorium.cpp
 * Author: mariuszl
 * 
 */

#include "repozytorium.h"


repozytorium::repozytorium() 
{
    uzytkownicy = new vector<User*>;
    document.push_back(77);
    document.push_back(65);
    document.push_back(77);
    document.push_back(65);
    
}

repozytorium::~repozytorium() 
{
}

void repozytorium::addUser(User* newUser)
{
    string nazwa = inet_ntoa(newUser->userAdress.sin_addr);
    newUser->cursorPosition = getDocumentSize();
    for(int i = 0; i < getUzytkownicyCount(); i++)
    {
        if(inet_ntoa(uzytkownicy->at(i)->userAdress.sin_addr) == nazwa)
        {
            uzytkownicy->at(i) = newUser;
            return;
        }
            
    
    }
 
    uzytkownicy->push_back(newUser);
}
int* repozytorium::getDocument()
{
    int* doc = new int[getDocumentSize()]; 
    for(int i = 0; i < getDocumentSize(); ++i)
    {
        if(document.at(i) == 10) doc[i] = 32;
        else doc[i] = document.at(i);         
    }
    return doc;
}

int repozytorium::getDocumentSize()
{   
    return document.size();
}

int repozytorium::getUzytkownicyCount()
{
    return uzytkownicy->size();
}

void repozytorium::addLetter(int letter, int tryb, string user, int xMax)
{
    int position = getPosition(user);
    lastChange.pozycja = position;
    lastChange.tryb = tryb;
    lastChange.znak = letter;
    
    if(lastChange.znak == 127)
    {
        int i = 1;
        vector<int>::iterator it = document.begin();
        while( i < position)
        {
            it++;
            i++;
        }
        document.erase(it);
        changePosition(position, -1);
        lastChange.rozmiar = getDocumentSize();
        sendChangeToAll(user);
    
    }
	else if(lastChange.znak == 10)
	{
        int i = 0;
        vector<int>::iterator it = document.begin();
        cout<<position<<endl;
        while( i < position)
        {
            it++;
            i++;
        }
        int ile = i = (position % xMax);
        for(; i < xMax; i++)
        {
            cout<<i<<endl;
            if(position < getDocumentSize())
            {
				it = document.insert(it, 10);
            }
            else document.push_back(10); 
            changePosition(position, 1);
            position ++;
        }

        cout<<position<<endl;
       
        dopelnijEnter(position, xMax, ile); 
        lastChange.rozmiar = getDocumentSize();
        cout<<"wyslany rozmiar "<< lastChange.rozmiar<<endl;
        sendChangeToAll(user);
        

    }
	else if(tryb == CHANGE)
    {
        if(position < getDocumentSize())
        {
            document.at(position) = letter;
            changeCursorPosition(user, 1);
        }
        else 
        {
            document.push_back(letter);
            changePosition(position, 1);
        }
        lastChange.rozmiar = getDocumentSize();
        sendChangeToAll(user);
    }
	else if(tryb == INSERT)
    {
        int i = 0;
        vector<int>::iterator it = document.begin();
        while( i < position)
        {
            it++;
            i++;
        }
        
        if(position < getDocumentSize())
        {
            document.insert(it, letter);
            changePosition(position, 1);
        }
         else
        {
           document.push_back(letter);
           changePosition(position, 1);
        }
        lastChange.rozmiar = getDocumentSize();
        sendChangeToAll(user); 
    }    
}

void repozytorium::sendChangeToAll(string wlasciciel)
{
    for(int i = 0; i < getUzytkownicyCount(); ++i)
    {
        

        cout<<inet_ntoa(uzytkownicy->at(i)->userAdress.sin_addr)<<endl;
        if(inet_ntoa(uzytkownicy->at(i)->userAdress.sin_addr) == wlasciciel)
        {
            lastChange.wlasciciel = true;
        }
		else
        {
            lastChange.wlasciciel = false;
        }
        write(uzytkownicy->at(i)->userSocket, &lastChange, sizeof(Change));

    }

}

int repozytorium::getPosition(string user)
{
    for(int i = 0; i < getUzytkownicyCount(); ++i)
    {
        if(inet_ntoa(uzytkownicy->at(i)->userAdress.sin_addr ) == user)
        {
            return uzytkownicy->at(i)->cursorPosition;
        }
    }
    return 0;
}

void repozytorium::changePosition(int za, int o_ile)
{
    for(int i = 0; i < getUzytkownicyCount(); ++i)
    {
        if(uzytkownicy->at(i)->cursorPosition >= za)
        {
            uzytkownicy->at(i)->cursorPosition += o_ile;
        }
    }
    
}

void repozytorium::changeCursorPosition(string user, int offset)
{
    cout<<user<<endl;
    for(int i = 0; i < getUzytkownicyCount(); ++i)
    {
        if(inet_ntoa(uzytkownicy->at(i)->userAdress.sin_addr ) == user)
        {
            uzytkownicy->at(i)->cursorPosition += offset;
            break;
        }
    }
    
    for(int i = 0; i < getUzytkownicyCount(); ++i)
    {
           cout<<inet_ntoa(uzytkownicy->at(i)->userAdress.sin_addr)<<endl;
           cout<< uzytkownicy->at(i)->cursorPosition <<endl;
    }


}

void repozytorium::dopelnijEnter(int position, int xMax, int ile)
{
    int pom = 1;
    position++;
    int odniesienie = position + xMax - ile;
    while((pom <= xMax) && (position < getDocumentSize()) && (position < odniesienie))
    {
        position++;
        pom++;
    }
    if(position >= getDocumentSize()) return;
    
    int i = 0;
    vector<int>::iterator it = document.begin();
    cout<<position<<endl;
    while( i < position)
    {
        it++;
        i++;
    }
    it--;
    cout<<"zaczynam wstawianie na pozycji "<< position<<endl;
    while (pom <= xMax)
    {
        it = document.insert(it, 10);
        changePosition(position, 1);
        position ++;
        pom++;
    }
    cout<<"koncze wstawianie na pozycji "<< position<<endl;    
}


