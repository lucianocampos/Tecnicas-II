#ifndef SECUENCIA_H
#define SECUENCIA_H

#include <stdlib.h>
#include <wiringPi.h>
#include <pcf8591.h>
#include <curses.h>
#include <wiringSerial.h>


#define Adress 0x48												/* dirección del PCF8591 */
#define BASE 64													/* definición del pin 64 */
#define A0 BASE+0												/* dirección potenciometro ADC */

#define N 8														/* cantidad de GPIO a emplear de las GPIO definidas como utilizables */
#define v 25													/* base de tiempo en mseg */
#define delta 29												/* define máxima desviación de la velocidad incial: Periodo_max = v (1+ delta)[miliseg] */

//******ALGORITMO*****

int apilada(int modo);
int auto_fantastico(int modo);
int carrera(int modo);
int navidad (int modo);

//*******TABLA********

int choque (int modo);
int parpadeo(int modo);
int rebote(int modo);
int tren (int modo);

//******CONTROL*******

int set_gpio	(void);											/* declara e incializa entradas y salidas */
float set_vel	(void);											/* define la velocidad incial en milisegundos */
float control_velocidad(char tecla, float vel_actual);			/* control de velocidad, <u> aumenta velocidad y <d> disminuye */
int serie(int const *tabla, int tam, float vel );

//******TABLAS********

void algoritmo_tabla (int const *tabla, int tam, float vel);	/* implementa la tabla. tam= tamaño de la tabla. vel= periodo en milisegundos */

#endif 
