#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringSerial.h>

int main (void){
	
	int puerto, i, j, data;
	int const gpio[8]= {25,29,28,27,26,6,5,4};	//lista de GPIO disponibles en la placa(se trunca con N)
	int tabla[8]={1,2,4,8,16,32,64,128};

	char data_in, data_out;

	if( (puerto = serialOpen("/dev/ttyAMA0", 115200)) < 0){				//Verifica si puede abrirse el puerto especificado a la velocidad indicada
		printf("No se pudo abrir el puerto\n Presione enter para salir");
		exit (1);
	}

	if( wiringPiSetup()== -1){											
		printf("Error iniciando Raspberry\nPresione enter para salir");
		exit (1);
	}
		
	for (i=0; i<8; i++){
		pinMode(gpio[i], OUTPUT);
		digitalWrite(gpio[i], 0);
	}
	
	system("clear");

	while(1){
	
		for( i=0; i<8; i++){
		data_out = tabla[i];
		serialPutchar (puerto, data_out);
		printf("\nt:%d, d_o:%d, %c\t", tabla[i], data_out, data_out);
		fflush(stdout);
				
		if( serialDataAvail(puerto) ){								// Verifica si hay datos disponibles para lectura

			data_in = serialGetchar(puerto);							// Recibe el caracter
			data = data_in;												// Cast char a int
			printf("valor: %d, %c", data_in, data_in);	
			fflush(stdout);
			
			fflush(stdout);
			if( data<256 && data>0 ){									// Verifica que sea un numero de 8 bits
				
				for ( j=0; j<8; j++){								// convierte el decimal en una palabra de 8bits
				
					if(data!=0)	digitalWrite( gpio[7-j],  (data%2) );	// Coloca la salida en alto
					else		digitalWrite( gpio[7-j], 0);			// Salida en bajo
					data/=2;
					printf("%d", digitalRead(gpio[7-j]));				//imprime las salidas
					fflush(stdout);
				}
			}
			printf("\n");		
		}
		delay(1000);
		}
	}
	
	return 0;
}
