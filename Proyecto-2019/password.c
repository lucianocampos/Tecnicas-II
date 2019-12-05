#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <ncurses.h>
#include "password.h"

#define DEL 127
#define ENTER 10
#define EOL	13

char pass_in[6]="nnnnn",pass[6]="cinco";
int intentos=3;

void ingresar()
{
	int i=0;
	flushinp();

	while(2){
		pass_in[i]=getchar();
	
		if(i==5){									/* se ingresó el limite de caracteres de la contraseña pero no se confirmó el ingreso con un enter */
			while(	(pass_in[i] != ENTER) && (pass_in[i]!= EOL) && (pass_in[i] != DEL))
			{
				flushinp();
				pass_in[i]=getchar();
			}
		}	

		if(pass_in[i] == ENTER || pass_in[i]== EOL)	return;

		
		else if (pass_in[i] != DEL)		printw("*");	/* pass_in[i]!= ENTER && pass_in[i]!=DEL */

		else{											/* pass_in[i]==DEL */ 				
			if( i != 0 ){	
				i=i-2;
				printw("\b%c\b",32);
				fflush(stdout);
			}
			else	i--;
		}
		i++;
		refresh();
	}
	return;
}

int Comparar()
{	
	int x,y;
	
	getmaxyx(stdscr, y,x);

	for(int i=0; i<=5 ;i++){
		if(pass_in[i]==pass[i])	i++;
		else
		{
			intentos--;
			fflush(stdin);
			move(y/2-1, x/2-8);					/* centra la impresión en ventana */
			
			attron( A_BOLD | COLOR_PAIR(1) );	/*	negrita color rojo */	
			printw("ACCESO DENEGADO");
			attroff( A_BOLD | COLOR_PAIR(1));		

			return 0;
		}		
	}
	system("clear");	
	
	move(y/2-1, x/2-9);							/*	se posiciona de manera que la impresion quede centrada en la ventana */
	attron( A_BOLD | COLOR_PAIR(2) );			/*	negrita color verde */
	printw("ACCESO CONCEDIDO");	
	attroff( A_BOLD | COLOR_PAIR(2));		
	refresh();
	delay(1500);
	echo();
	endwin();
	return 1;
}

int password (void){

   	int y, x;

	noecho();									/* desactiva el eco */
	getmaxyx(stdscr, y, x);						/* obtiene los limites de la ventana estandar */

	while(intentos!=0)
	{
		move(y/2 -1, x/2 - 23);					/* se posiciona para centrar la impresión */
		printw("Ingrese Contraseña(Intentos %d): ",intentos);
		refresh();
		ingresar();
		clear();
		if (Comparar()==1)     return 0;
		refresh();
		delay(1500);		
	}

	getmaxyx(stdscr, y, x);
	move(y/2-1, x/2-9);							/*	impresión centrada */
	attron( A_BOLD | COLOR_PAIR(1) );			/*	negrita color verde */
	printw("PROGRAMA FINALIZADO");	
	attroff( A_BOLD | COLOR_PAIR(1));
	refresh();
	
	delay(1500);
		
	echo();
		
	return 1;
}
