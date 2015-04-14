/* 
 * File:   klient.cpp
 * Author: hania
 *
 */

#include "communication.h"

void keyOut(int znak, bool insertMode);

void poloczenie()
{
    if ((sckSend = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) 
    {
		perror ("Nie można utworzyć gniazdka");
		exit (EXIT_FAILURE);
    }

    if (connect (sckSend, (struct sockaddr*) &sckSendAddr, sizeof sckSendAddr) < 0) 
    {
		perror ("Brak połączenia");
		exit (EXIT_FAILURE);
    } 
  
}
			  
int enter()
{
    int xMax, yMax, x, y, i, j=0;
    chtype t; 
    getmaxyx(stdscr,yMax,xMax);
    getyx(stdscr, y, x);
    move(y+1,x);
    insertln(); 
    
    move(y,x);
    for(i=x;i<xMax;i++)
    {
		t=inch();
		delch();
		move(y+1, j);
		printw("%c", t);
		j++;
		move(y,x);
    }
    move(y+1,0);
    return yMax;

}
void backspace()
{
    int x, y, xMax, yMax, curr, currM, i;
    chtype t;
    getyx(stdscr, y, x);
    getmaxyx(stdscr,yMax,xMax);
    curr=y;
    currM=yMax;
    if ((x==0)&&(y==0)) 
		return;
                                                     //BACKSPACE			  			
    if(x==0) 
    {
		move(y-1,xMax-1);	
		curr=y-1;
		delch();
    } 
    else
    {
		move(y,x-1);
		delch();
    }
      
    for(i=curr+1;i<currM;i++) 
    {
		move(i,0);
		t=inch();
		delch();
		move(i-1, xMax-1);
		printw("%c", t);
    }  
    if(x==0) 
    {
		move(y-1,xMax-1);
		move(y-1,x-1);
    }
	move(y,x-1);
	  
}
void del()
{
    int x, y, xMax, yMax, curr, currM, i;
    chtype t;
    getyx(stdscr, y, x);
    getmaxyx(stdscr,yMax,xMax);
    curr=y;
    currM=yMax;
    delch();
    for(i=curr+1;i<currM;i++)
    {
		move(i,0);
		t=inch();
		delch();
		move(i-1, xMax-1);
		printw("%c", t);
    }
    move(y,x);     
}
void printChar(int litera)
{
    int xMax, yMax, x, y, curr, currM, i;
    chtype t;
    getyx(stdscr, y,x);
    getmaxyx(stdscr,yMax,xMax);
    if (insertMode==true)
    {
		curr=y;
		currM=yMax;	    
		for(i=currM-1;i>=curr;i--) 
		{
			move(i,xMax-1);
			t=inch();
			move(i+1, 0);
			insch(t);	    
		}
		move(y,x);
		insch(litera);
		if (x!=xMax-1)
			move(y,x+1);  
		else 
			move(y+1,0); 
    } 
    else
      printw("%c", litera); 
	
}
int esc()
{
    int znak, xAkt, yAkt, x, y, xMax, yMax;
    getyx(stdscr,yAkt, xAkt);
    getmaxyx(stdscr,yMax, xMax);
    move(yAkt, xMax-1);
    printw("\nCzy chcesz zakonczyc prace z programem? [T or N]\t");
    znak = getch();
    printw("%c", znak);
    if(( znak == 84 )||( znak == 116 ))  
    {
		printw("\nProgram wylaczy sie po wcisnieciu dowolnego przycisku.\n");
		getch();
		close (sckSend);
		close (sckRcv);
		close (sckServ);
		pthread_cancel(idRead);
		pthread_cancel(idWrite);
		endwin();
		exit (EXIT_SUCCESS);
    }	  
    else 
    {
		while(x!=0)	
		{
			getyx(stdscr, y, x);
			move(y,x-1);
			delch();  
		}
		move(yAkt,xAkt);
      
    }
	return yMax;
}
int left(Message msg)
{
    int x, y, xMax, yMax;
    getyx(stdscr, y, x);
    if (!((x==0)&&(y==0))){
		if(x==0) 
		{
			getmaxyx(stdscr,yMax,xMax);
			y--;
			move(y,xMax-1);		    
		}
		move(y,x-1);
		msg.rodzajWiadomosci=ZMIANA_POZYCJI;
		msg.offset=-1;
		poloczenie();
		write(sckSend, &msg, sizeof(Message)); 
		close(sckSend);
    }
    return yMax;
}
int right(Message msg)
{
    int x, y, xMax, yMax;
    getyx(stdscr, y, x);
    getmaxyx(stdscr,yMax,xMax);    
    if((y*xMax + x) >= documentRozmiar) return 0;
    if (!((x==xMax-1)&&(y==yMax-1)))
    {
		if(x==xMax-1) 
		{
			y++;
			move(y,0);	  
		}
		move(y,x+1);
			//getmaxyx(stdscr, yMax, xMax);               <----to wszystko było przed zmiana communication.h
			//getyx(stdscr, y, x);
			//msg.cursorPosition=xMax*y+x;
			//write(sckSend, &msg, sizeof(Message));
		msg.rodzajWiadomosci=ZMIANA_POZYCJI;
		msg.offset=1;
		poloczenie();
		write(sckSend, &msg, sizeof(Message)); 
		close(sckSend);
    }
    return yMax;
}
void down(Message msg)
{
    int x, y, xMax, yMax;
    getyx(stdscr, y, x);
    getmaxyx(stdscr,yMax,xMax); 
    if(y >= (documentRozmiar/xMax)) return;
    if(y!=yMax-1)  
    {
		if(((y+1)*xMax + x) > documentRozmiar) 
			move(y+1,documentRozmiar % xMax);
		else 
			move(y+1,x);
		getmaxyx(stdscr, yMax, xMax);
	    msg.rodzajWiadomosci=ZMIANA_POZYCJI;
		if(((y+1)*xMax + x) > documentRozmiar) 
			msg.offset = xMax - (x - documentRozmiar % xMax);
		else 
			msg.offset=xMax;
		poloczenie();
		write(sckSend, &msg, sizeof(Message)); 
		close(sckSend);
    }     
}
int up(Message msg)
{
    int x, y, xMax, yMax;
    getyx(stdscr, y, x);
    getmaxyx(stdscr,yMax,xMax);    
    if(y!=0)	
    {
		move(y-1,x);
		getmaxyx(stdscr, yMax, xMax);
		msg.rodzajWiadomosci=ZMIANA_POZYCJI;
		msg.offset=-xMax;
		poloczenie();
		write(sckSend, &msg, sizeof(Message));
		close(sckSend);
    }
    return yMax;
}


int sendChar(int znak)
{
    int xMax, yMax;
    Message msg;
    getmaxyx(stdscr, yMax, xMax);          
    msg.rodzajWiadomosci=PRZESLANIE_ZNAKU;
    msg.znak=znak;
    msg.offset = xMax;
    if (insertMode==true)
		msg.trybWprowadzania=INSERT; 
    else
		msg.trybWprowadzania=CHANGE;
    poloczenie();
    write(sckSend, &msg, sizeof(Message));
    close(sckSend);
    return yMax;
}


void * keyIn(void * arg)             //do wczytania znaku wprowadzonego z klawiatury i wyslania do serwera
{    
    int litera;
    Message msg;
    nodelay(stdscr, TRUE);
    while(true)
    {
        if (result != 0) 
		{
			fprintf(stderr, "Error occured during locking the mutex.\n");
			exit (EXIT_FAILURE);
		}
		if((litera = getch()) != ERR)
		{
			switch(litera)
			{
				case 27:
					esc();                       //ESC
					sendChar(litera);
					break;	
				case KEY_UP:                             //UP
					up(msg);
					break;				      
				case KEY_DOWN:                           //DOWN
					down(msg);
					break;				      
				case KEY_LEFT:
					left(msg);                      //LEFT  
					break;				      
				case KEY_RIGHT:
					right(msg);                     //RIGHT
					break;
				case KEY_IC:                          //MODE
					insertMode=!insertMode;
					break;
				  
				default:
					if(((litera>=32)&&(litera<=127)) || (litera == 10))
						sendChar(litera);
					break;
			}
	
			if (result != 0) 
			{
					fprintf(stderr, "Error occured during unlocking the mutex.\n");
					exit (EXIT_FAILURE);
			}
		}
    }
    pthread_exit(NULL);
  
}
void keyOut(int znak, bool insertMode)                       //  do wypisania znaku odebranego od serwera
{              
    switch(znak)
    {  
		case 127:            //BACKSPACE
			backspace();
			break;		    
					
		case KEY_DC:                   //DELETE
			del();
			break; 	
			
		case 10:                       //ENTER
			enter();
			break;   
					  
		default:
			printChar(znak);
			break;
    }  
}
void * receive(void * arg)
{
    int xMax, x, y, xCurr, yCurr;
    Change chang;    
    while(true)
    {
        if (result != 0) 
		{
				fprintf(stderr, "Error occured during locking the mutex.\n");
				exit (EXIT_FAILURE);
		}
		nTmp = sizeof(struct sockaddr);
	
		if (sckServ < 0) 
		{
			perror ("Can't create a connection's socket.\n");
			exit (EXIT_FAILURE);
		}

		if(read(sckServ, &chang, sizeof(Change))>0)
		{
			documentRozmiar = chang.rozmiar;
			getmaxyx(stdscr, yCurr, xMax);
			getyx(stdscr, yCurr, xCurr);
			y=chang.pozycja/xMax;
			x=chang.pozycja-(y*xMax);
			move(y,x);
			if (chang.tryb==INSERT) 
				insertMode=true;	    
			else
				insertMode=false;
			keyOut(chang.znak, insertMode);
			
			if((chang.znak != 127)  && ( ((yCurr > y) || ((yCurr == y) && (xCurr >= x) ))))
			{
				if((chang.wlasciciel==false) && (chang.znak==10))
				{	
					if(((yCurr + 1)*xMax + xCurr) >= chang.rozmiar)
					{
						yCurr = chang.rozmiar/xMax;
						xCurr = chang.rozmiar % xMax;
						move(yCurr,xCurr);
				   	}
					else 
						move(++yCurr,xCurr);
				 
				}
				else if(xCurr < xMax - 1)
					xCurr++;
				else 
				{
					xCurr = 0; 
					yCurr++;
				}	
			}
			
			if((chang.znak == 10) && (chang.wlasciciel == true))
			{
				move(yCurr + 1, 0);
			}
			else if(chang.znak != 127)
				move(yCurr, xCurr);			
		}
    }    
    pthread_exit(NULL);
}
void registration()
{
    int rozmiar;
    int *buf;
    Message reg; 
    reg.rodzajWiadomosci=POLACZENIE;
    reg.port=SERVER_PORT;
    write(sckSend, &reg, sizeof(Message));
    close(sckSend);
    nTmp = sizeof(struct sockaddr);
    sckServ = accept(sckRcv, (struct sockaddr*)&sckServAddr, &nTmp);
    read(sckServ, &rozmiar, sizeof(int));
    if(rozmiar > 0)
    {
		buf = new int[rozmiar];
		read(sckServ, buf, rozmiar*sizeof(int));
		for(int i = 0; i < rozmiar; i++)
			keyOut(buf[i], 0);         
    }
    documentRozmiar = rozmiar;       
}



void createSockets()
{
    memset (&sckSendAddr, 0, sizeof sckSendAddr);
    sckSendAddr.sin_family = AF_INET;
    inet_aton (server, &sckSendAddr.sin_addr);
    sckSendAddr.sin_port = htons (service_port);
      
    memset(&sckRcvAddr, 0, sizeof(struct sockaddr));
    sckRcvAddr.sin_family = AF_INET;
    sckRcvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sckRcvAddr.sin_port = htons(SERVER_PORT);
    if ((sckSend = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) 
    {
		perror ("Nie można utworzyć gniazdka");
		exit (EXIT_FAILURE);
    }

    if (connect (sckSend, (struct sockaddr*) &sckSendAddr, sizeof sckSendAddr) < 0) 
    {
		perror ("Brak połączenia");
		exit (EXIT_FAILURE);
    }
    if ((sckRcv = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
		perror ("Nie można utworzyć gniazdka");
		exit (EXIT_FAILURE);
    }
      
    setsockopt(sckRcv, SOL_SOCKET, SO_REUSEADDR, (char*)&nFoo, sizeof(nFoo));

    nBind = bind(sckRcv, (struct sockaddr*)&sckRcvAddr, sizeof(struct sockaddr));
    if (nBind < 0) 
    {
		perror ("Can't bind a name to a socket.\n");
		exit (EXIT_FAILURE);
    }
    nListen = listen(sckRcv, QUEUE_SIZE);
    if (nListen < 0) 
    {
		perror ("Can't set queue size.\n");	  
    }
}

int main ()
{
    createSockets();
    initscr();     
    keypad(stdscr, TRUE ); 
    noecho(); 
    registration();    
    pthread_create(&idRead, NULL, &receive, &sckRcv);
    pthread_create(&idWrite, NULL, &keyIn, &sckSend);   
    while(true);   
    close (sckSend);
    close (sckRcv);    
    endwin(); 
    exit (EXIT_SUCCESS);
}
