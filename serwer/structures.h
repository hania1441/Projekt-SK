/* 
 * File:   structures.h
 * Author: mariuszl
 *
 * Created on 5 listopada 2014, 16:54
 */

#ifndef STRUCTURES_H
#define	STRUCTURES_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <netinet/in.h>
#include <arpa/inet.h>


#ifdef	__cplusplus
}
#endif

//struktura zawierająca dane o użytkowniku
struct User
{
    int userSocket;
    sockaddr_in  userAdress;  
    int cursorPosition;
};



#endif	/* STRUCTURES_H */

