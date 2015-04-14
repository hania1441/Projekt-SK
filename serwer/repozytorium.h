/* 
 * File:   repozytorium.h
 * Author: mariuszl
 *
 */

#ifndef REPOZYTORIUM_H
#define	REPOZYTORIUM_H

#include <vector>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <iostream>

#include "structures.h"
#include "communication.h"

using namespace std;

class repozytorium 
{
	public:
		repozytorium();
		virtual ~repozytorium();
		void addUser(User*);
		int* getDocument();
		int getDocumentSize();
		void addLetter(int letter, int tryb, string user, int);
		void changeCursorPosition(string user, int offset);
	private:    
		void sendChangeToAll(string wlasciciel);
		int getPosition(string user);
		void changePosition(int za, int o_ile);
		void dopelnijEnter(int position, int xMax, int ile);
		int getUzytkownicyCount();

		vector <User*>* uzytkownicy;
		vector <int> document;
		Change lastChange;
};

#endif	/* REPOZYTORIUM_H */

