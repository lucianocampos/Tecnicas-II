#include <stdio.h>
#include <stdlib.h>

#define N 8

int main ( ){	
	int i=0, puerto, dato_in;
	int const gpio[N]= {25,29,28,27,26,6,5,4};								
	char dato_recibido;									
	
	if(wiringPiSetup() == -1)	exit (1);
	if( (puerto = serialOpen("/dev/ttyAMA0", 9600)) < 0)	exit (1);									
	
	for(i=0; i<N; i++)
	{
		pinMode(gpio[i], OUTPUT);
		digitalWrite(gpio[i], 0);
	}

	system("clear");
	printf("\n\n\n Comunicación Serie establecida");
	
	while(1){		
		
		if(serialDataAvail (puerto)){
				
		dato_recibido = serialGetchar(puerto);
		dato_in = dato_recibido;
				
		if( dato_in >=0 && dato_in<256){
			for(int i=0; i<N;i++){
				if(dato_in!=0)	digitalWrite(gpio[i], (dato_in % 2));
				else digitalWrite(gpio[i],0);
				dato_in = dato_in /2;
		
			}
		}
	}

	return 0;
}
