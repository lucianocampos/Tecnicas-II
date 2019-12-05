#include <stdio.h>
#include <string.h>
#include <wiringPi.h>
#include <ncurses.h>
#include "secuencia.h"
#include "password.h"


int main(){

	int x, x_aux,y,i=20, modo=3;	/* "i" y "modo" se incializan de manera que no cumplan las condiciones de los loops de las líneas 53 y 84*/

	initscr();						/* inicia el sistema curses y asgina memoria para la ventana especificada (stdscr) */
	
	keypad(stdscr, TRUE);   		/* 	habilita teclas y combinaciones especiales (F1,F2,ctrl+x..) en la ventana especificada (estándar)
										se emplea para poder utilizar Ctrl+C para detener el programa en caso de ser necesario*/	
	clear();						/* limpia la ventana actual (ventana standart <stdrscr>)*/
	echo();							/* activa el echo de la consola */

	start_color();										/* permite el uso de modificadores de color para texto */
	init_pair(1, COLOR_RED, COLOR_BLACK);				/* define el par de colores 1 para poder usar colores como atributo */
	init_pair(2, COLOR_GREEN, COLOR_BLACK);				/* define el par de colores 2 para poder usar colores como atributo */	

	if( password()==0 ){
		
		do{
			
			getmaxyx( stdscr, y, x);					/* Obtiene los limites maximos de la ventana */
			move( y/2-8, (x/2-15) );					/* centra la impresion (15 es la mitad de la proxima cadena a imprimir) */

			attron( A_UNDERLINE);						/* incio de texto subrayado */
			printw("Seleccione secuencia deseada");
			attroff( A_UNDERLINE);						/* fin de texto subrayado */
			
			mvprintw((y/2-5), (x/2-18),"apilada--------------------------1");	
			mvprintw((y/2-4), (x/2-18),"auto fantastico------------------2");
			mvprintw((y/2-3), (x/2-18),"carrera--------------------------3");	
			mvprintw((y/2-2), (x/2-18),"choque---------------------------4");
			mvprintw((y/2-1), (x/2-18),"intermitente---------------------5");
			mvprintw((y/2), (x/2-18),  "navidad--------------------------6");
			mvprintw((y/2+1), (x/2-18),"rebote---------------------------7");
			mvprintw((y/2+2), (x/2-18),"tren-----------------------------8");
			mvprintw((y/2+4), (x/2-18),"salir del programa---------------0");
	
			move( (y/2+7), (x/2 -18));
			x_aux = x;
			getyx(stdscr, y, x);
			attron( A_BOLD);							/* imprime en negrita */
			noecho();									/* desactiva el eco de la consola */
			do{
				flushinp();								/* vacía el buffer de entrada (utilidad: solo si se repite el loop por no cumplir la linea 56) */
				mvprintw( y , (x_aux/2 -18) ,"Opcion: ");
				refresh();
				i = getchar()- 48;						
				
				if(i>=0 && i<=8)						/* verifica que se haya ingresado un número válido */
				{
					printw("%d\n\n", i);
					refresh();
					delay(250);
				}
				
				else{	
					move ( y, x);
					clrtoeol();
					attron( COLOR_PAIR(1) );							/*	habilita impresión en negrita color rojo */
					mvprintw(y, (x_aux/2 -10), "OPCION INCORRECTA");
					attroff( COLOR_PAIR(1));							/* 	desactiva la impresión en negrita color rojo */
					refresh();				
					delay(1000);
					move ( y, x );										/* borrado de la impresion de "opcion incorrecta" */
					clrtoeol();
					refresh();
				}
				
			} while (i<0 || i>8);
			
			if(i!=0){
				getyx(stdscr, y, x);
				
				do{
					flushinp();	
					mvprintw( y , (x_aux/2 -18) ,"Modo (0-Local, 1-Remoto) : ");
					refresh();
					modo = getchar()- 48;
					
					if(modo==0 || modo==1)								/* verifica que se haya ingresado un número válido */
					{
						printw("%d", modo);
						refresh();
						delay(250);
					}
					
					else{	
						move ( y, x);
						clrtoeol();
						attron( COLOR_PAIR(1) );						/*	habilita impresión en negrita color rojo */
						mvprintw(y, (x_aux/2 -10), "OPCION INCORRECTA");
						attroff( COLOR_PAIR(1));						/* 	desactiva la impresión en negrita color rojo */
						refresh();				
						delay(1000);
						move ( y, x );									/* borra la impresion de "opcion incorrecta" */
						clrtoeol();
						refresh();
						flushinp();
					}
				
				} while (modo!=0 && modo!=1);	
			}
					
			cbreak();													/* Modo no canonico */ 
			nodelay(stdscr, TRUE);

			clear();
			getmaxyx(stdscr, y, x);
			attron( COLOR_PAIR(2) );
	
			if(modo==0)
				mvprintw(4, x_aux/2 -33, "MODO DE TRABAJO: LOCAL");
			
			else if(modo==1)
				mvprintw(4, x_aux/2-33, "MODO DE TRABAJO: REMOTO");
	
			else{
				mvprintw(y/2, x/2, "ERROR");
				i=0;
			}
			
			move( 6 , x/2 -33);
									
			switch (i){ 
					
				case 1:	printw("*********************** secuencia: apilada ***********************");
						attroff( A_BOLD | COLOR_PAIR(2) );
						if(apilada(modo)==1)	exit(1);			/* Las secuencias devuelven 0 como finalización correcta y 1 en caso contrario */ 
						break;

				case 2: printw("******************* secuencia: auto fanstatico *******************"); 
						attroff( A_BOLD | COLOR_PAIR(2) );						
						if(auto_fantastico(modo))	exit(1);					
						break;

				case 3: printw("*********************** secuencia: carrera ***********************"); 
						attroff( A_BOLD | COLOR_PAIR(2) );						
						if(carrera(modo))	exit(1); 
						break;	

				case 4: printw("*********************** secuencia: choque ************************");
						attroff( A_BOLD | COLOR_PAIR(2) );						
						if(choque(modo))		exit(1);
						break;	
		
				case 5: printw("********************* secuencia: intermitente ********************"); 
						attroff( A_BOLD | COLOR_PAIR(2) );						
						if(parpadeo(modo))	exit(1); 
						break;	
		
				case 6: printw("*********************** secuencia: navidad ***********************"); 
						attroff( A_BOLD | COLOR_PAIR(2) );						
						if(navidad(modo))	exit(1); 
						break;	
		
				case 7: printw("************************ secuencia: rebote ***********************"); 
						attroff( A_BOLD | COLOR_PAIR(2) );						
						if(rebote(modo))		exit(1); 
						break;	

				case 8: printw("************************ secuencia : tren ************************");
						attroff( A_BOLD | COLOR_PAIR(2) );						 
						if(tren(modo))		exit(1); 
						break;	
		  			
				case 0: clear();
						move(y/2-1, x/2-9);
						attron( A_BOLD);	
						printw("PROGRAMA FINALIZADO");
						move(50,0);
						refresh(); 
						attroff( A_BOLD);
						break;	
				
				default: break;	
			}
			
			delay(500); 			
			cbreak();
			nodelay(stdscr, FALSE);		/* reestablece la terminal */
			echo();
			flushinp();
			clear();
			refresh();
		
	}while( i != 0 );
	}									/* cierre if password */
	
	clear();
	keypad(stdscr, FALSE);
	endwin();
	
	return 0;
}

