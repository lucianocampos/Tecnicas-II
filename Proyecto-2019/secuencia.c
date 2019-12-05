#include "secuencia.h"

int const gpio[N]= {25,29,28,27,26,6,5,4};								/* lista de GPIO disponibles en la placa(se trunca con N en secuencia.h)*/
int flag=0;



/**************************************** INICIALIZA GPIO *****************************************/



int set_gpio (void){
	
	if( flag == 0){
		pcf8591Setup(BASE,Adress);										/* evita errores por declaraciones multiples del pin 64. aka PCF8591 */
		flag = 1;
	}
	
	if(wiringPiSetup() == -1)	return (1);								/* 1 como error */
		

	for (int i=0; i<N; i++){
		pinMode(gpio[i], OUTPUT);
		digitalWrite ( gpio[i], 0);	
	}
				
	pinMode(2, INPUT);													/* inicia pulsadores */
	pinMode(0, INPUT);
	
	return 0;															/* como finalización correcta */
};



/************************************ SETEO VELOCIDAD INICIAL *************************************/



float set_vel (void){

	int x,y;

	getmaxyx(stdscr, y,x);
	move(9, x/2 -33);
	float vel = 0, ultimo = -1;	/* Se incializa a un valores que no son posibles de obtener con el potenciometro para que no cumplan el if de comparación*/
	printw("(Presione SW2 o enter para confirmar)\t    Periodo [mS]: ");
	getyx(stdscr, y, x);												/* Obtiene la posición actual del cursor en pantalla. */ 
	
	while ( digitalRead(2)==0 && getch()!= 10 ){
		flushinp();
		if( ultimo != analogRead(A0)){									/* Compara si hubo cambios desde la ultima entrada. */
		
		ultimo = analogRead(A0);										/* actualiza el valor del potenciometro. */
		vel = 2*v*(1+ delta*(ultimo/255.00) );
		move(y,x);														/* Se coloca el cursor en la posición obtenida anteriormente. */
		clrtoeol();								
		printw("%.2f", vel);
		refresh();														/* Vuelca en pantalla todos los cambios. */
		}
	}
	getmaxyx(stdscr, y,x);
	move(13, x/2 - 33);
	mvprintw( 13, (x/2 - 33), "En cualquier momento presione:\t <e> para finalizar la secuencia.");
	mvprintw( 14, (x/2 - 33), "\t\t\t\t\t <u> para aumentar  la velocidad.");
	mvprintw( 15, (x/2 - 33), "\t\t\t\t\t <d> para disminuir la velocidad.\n");	
	refresh();
	flushinp();
	return vel;
}



/**************************** CONTROL VELOCIDAD EN TIEMPO DE EJECUCIÓN ****************************/



float control_velocidad(char tecla, float vel_actual){
	
	int y, x; 												/* Periodo minimo en milisegundos. */
	float tope = 10 ;

	getmaxyx(stdscr, y,x);
	move(18, (x/2 -33));
	clrtoeol();
	
	if( tecla == 'd'){
		if( (2*vel_actual) <= 5000.00){							/* Velocidad minima admitida 1/5 hertz */
		
			mvprintw( 18, (x/2 - 33), "velocidad disminuida.\t\t T: %.2f [mS] | f: %.2f [Hz]", 2*vel_actual, 500/(vel_actual));
			refresh();
				
			return (2*vel_actual);										/* Disminuye la velocidad  a la mitad aumentado el periodo al doble. */
			}
			
		else if ((2*vel_actual) >= 5000){
			
			mvprintw( 18, (x/2 - 33), "velocidad minima alcanzada.\t T: 5000.00 [mS] | f: 0.20 [Hz]");
			refresh();
				
			return (5000.00);											/* Disminuye la velocidad  a la mitad aumentado el periodo al doble. */
			}
	}			
	else if (tecla == 'u'){	
		
		if( (0.75*vel_actual) >= tope){
			
			mvprintw( 18, (x/2 - 33), "velocidad aumentada.\t\t T: %.2f [mS] | f: %.2f [Hz]", 0.75*vel_actual, 1000/(0.75*vel_actual) );
			refresh();
			
			return (0.75*vel_actual);									/* Aumenta velocidad actual en 33% disminuyendo el periodo 25%. */
		}
		else
		{
			mvprintw( 18, (x/2 - 33), "velocidad maxima alcanzada.\t T: %.2f [mS] | f: %.2f [Hz]", tope, 1000/(tope) );
			refresh();
			
			return (tope); 										/* Velocidad maxima 1/tope[ms]. */
		}
	}	
	
	else 	exit(1);	
}



/*************************************** SECUENCIA APILADA ****************************************/



int apilada(int modo)
{ 
	int i,j,ultimo=7;
	char monitor = 'p';
	
	if(set_gpio())	exit(1);
	
	float vel = set_vel();

	if( modo == 0 ){
		do{
	
			do{
				flushinp();
				delay(vel);
				
				for(i=-1; i<=ultimo; i++){				/* Secuencia de Ida */
					delay(vel);
					digitalWrite(gpio[i+0],0);
					digitalWrite(gpio[i+1],1);
					if(i+1 == ultimo){
						ultimo --;
						for (j=0; j<8; j++){
							digitalWrite(gpio[i+1], !digitalRead(gpio[i+1]));
							delay(2*vel);
						}
						if (ultimo==-1){				/* reinicia la secuencia */
							delay( 4*vel );					
							for(j=(N-1); j>=0; j--)	digitalWrite(gpio[i],0);
							ultimo=7;
						}
					}
				}
			
				monitor= getch();
				
			}while( (monitor != 'e') && (monitor != 'u') && (monitor != 'd') );
			
			if( monitor != 'e')
				vel = control_velocidad(monitor, vel);
					
		}while( monitor != 'e' );
	}
	
	else if( modo == 1 ){
			int  tabla [173]= {1, 2, 4, 8, 16, 32, 64, 128, 0, 128, 0, 128, 0, 128,
					0, 128, 0, 128, 0, 128, 0, 128, 0, 128, 129, 130, 132,
					136, 144, 160, 192, 128, 192, 128, 192, 128, 192, 128,
					192, 128, 192, 128, 192, 128, 192, 128, 192, 193, 194,
					196, 200, 208, 224, 192, 224, 192, 224, 192, 224, 192,
					224, 192, 224, 192, 224, 192, 224, 192, 224, 225, 226,
					228, 232, 240, 224, 240, 224, 240, 224, 240, 224, 240,
					224, 240, 224, 240, 224, 240, 224, 240, 241, 242, 244,
					248, 240, 248, 240, 248, 240, 248, 240, 248, 240, 248,
					240, 248, 240, 248, 240, 248, 249, 250, 252, 248, 252,
					248, 252, 248, 252, 248, 252, 248, 252, 248, 252, 248,
					252, 248, 252, 253, 254, 252, 254, 252, 254, 252, 254,
					252, 254, 252, 254, 252, 254, 252, 254, 252, 254, 255,
					254, 255, 254, 255, 254, 255, 254, 255, 254, 255, 254,
					255, 254, 255, 254, 255, 255, 255, 255, 255, 255,0,0,0,0};
			if( serie(tabla, 173, vel) == 1 )	 
			return 1;
	}
	
	else exit(1);
	
	clear();
	refresh();
	for(i=0; i<=7; i++)		digitalWrite(gpio[i],0);
	return 0; 			
}



/************************************ SECUENCIA AUTO FANTASTICO ***********************************/



int auto_fantastico (int modo){
	 
    int i=-1,k=1;
    char monitor = 'p';
    
	if(set_gpio())	exit(1);
	
	float vel = set_vel();
	  
	if( modo == 0 ){  	
		do{    															/* monitorea solicitud de finalizado */
			do{															/* monitorea cambios de velocidad */
				flushinp();
				if(i>=0)	digitalWrite(gpio[i+0], 0);
				digitalWrite(gpio[i+k], 1);
				i=i+k;
				if(i==7)	k=-1;
				if(i==0)	k=1;
				delay(vel);
				
			monitor = getch();	
			}while( (monitor != 'e') && (monitor != 'u') && (monitor != 'd'));
		
			if( monitor != 'e')
				vel = control_velocidad(monitor, vel);
			
		}while(monitor != 101);
	}
	
	else if( modo == 1 ){
		int const tabla[14]={1,2,4,8,16,32,64,128,64,32,16,8,4,2};		
		if( serie(tabla, 14, vel) == 1 )	 
			return 1;
	}
	
	else exit(1);
	
	clear();
	refresh();

    for(i=0; i<N; i++)	digitalWrite(gpio[i],0);
    	
	return 0;
}



/*************************************** SECUENCIA PARPADEO ***************************************/




int parpadeo (int modo){

	int i,j;
	char monitor = 'p';
	
	if(set_gpio())	exit(1);
	
	float vel = set_vel();
	
	vel = vel;

	if( modo == 0){
		do{
		
			do{				
				flushinp();
				for(i=0; i<N; i++){
					for(j=0; j<7;j++){
						digitalWrite(gpio[i], !digitalRead(gpio[i]));
						delay(vel);
					}
					digitalWrite( gpio[i], 1);
					delay(vel*2);
				}
				
				for(i=0; i<N; i++){
					for(j=0; j<7;j++){
						digitalWrite(gpio[i], !digitalRead(gpio[i]));
						delay(vel);
					}
					digitalWrite( gpio[i], 0);
					delay(2*vel);
				}
				
				monitor = getch();	
			
			}while( (monitor != 'e') && (monitor != 'u') && (monitor != 'd'));
		
			if( monitor != 'e')
				vel = control_velocidad(monitor, vel);
			
		}while(monitor != 'e');
	}
	
	else if( modo == 1){
		int const tabla[128]={1, 0, 1, 0, 1, 0, 1, 1, 3, 1, 3, 1, 3, 1, 3,
		3, 7, 3, 7, 3, 7, 3, 7, 7, 15, 7, 15, 7, 15, 7, 15, 15, 31, 15,
		31, 15, 31, 15, 31, 31, 63, 31, 63, 31, 63, 31, 63, 63, 127, 63,
		127, 63, 127, 63, 127, 127, 255, 127, 255, 127, 255, 127, 255, 
		255, 254, 255, 254, 255, 254, 255, 254, 254, 252, 254, 252, 254,
		252, 254, 252, 252, 248, 252, 248, 252, 248, 252, 248, 248, 240, 
		248, 240, 248, 240, 248, 240, 240, 224, 240, 224, 240, 224, 240, 
		224, 224, 192, 224, 192, 224, 192, 224, 192, 192, 128, 192, 128, 
		192, 128, 192, 128, 128, 0, 128, 0, 128, 0, 128, 0, 0};
	
		if( serie(tabla, 128, vel) == 1 )	 
			return 1;
	}
	
	else exit(1);
	
	clear();
	refresh();
	for(i=0; i<N;i++)	digitalWrite(gpio[i],0);
	
	return 0;
}




/**************************************** SECUENCIA REBOTE ****************************************/




int rebote(int modo){

	int i=2,j,k=1;
	char monitor;
	
	if(set_gpio())	exit(1);
	
	float vel = set_vel();

	if( modo == 0 ){
		do{
			do{
				flushinp();
				if( (i-2*k) >= 0)	digitalWrite( gpio[i-(3*k)] , 0 );
				for(j=0; j<3; j++)	digitalWrite( gpio[i-(j*k)] , 1 );
					
				i=i+k;
				
				if(i != 8 && i!= -1)	delay(vel);						/* evita que se detenga en los extremos */
					
				if(i==8){
					i = 5;
					k = -1;
				}
					
				else if(i==-1){
					i = 2;
					k = 1;
				}
				
				monitor = getch();
			}while( (monitor != 'e') && (monitor != 'u') && (monitor != 'd'));
			
			if( monitor != 'e')
				vel = control_velocidad(monitor, vel);
			
		}while(monitor != 'e');
	}
	
	else if( modo == 1 ){
		int const tabla[10]={7,14,28,56,112,224,112,56,28,14};
		if( serie(tabla, 10, vel) == 1 )	 
			return 1;
	}
	
	else exit(1);
	
	clear();
	refresh();
	for(i=0; i<N;i++)	digitalWrite(gpio[i],0);
	
	return 0;
}



/*************************************** SECUENCIA CARRERA ****************************************/



int carrera (int modo){
	
	int i;
	int tabla[16]={1,1,2,2,4,4,8,16,17,18,36,40,80,96,128,0};
				
	if(set_gpio())	exit(1);
	
	float vel = set_vel();
	
	if( modo == 0 )	algoritmo_tabla(tabla, 16, vel);
	
	else if( modo == 1 ){
		if( serie(tabla, 16, vel) == 1 )	 
			return 1;
	}
	
	else exit(1);
	
	for(i=0; i<N; i++)	digitalWrite(gpio[i],0);
		
	return 0;
	
}



/*************************************** SECUENCIA NAVIDAD ****************************************/



int navidad (int modo){
	
	int i;
	int tabla[4]={170,0,85,0};
				
	if(set_gpio())	exit(1);
	
	float vel = set_vel();
			
	if( modo == 0 )	algoritmo_tabla(tabla, 4, vel);
	
	else if( modo == 1 ){
		if( serie(tabla, 4, vel) == 1 )	 
			return 1;
	}
	
	else exit(1);
	
	for(i=0; i<N; i++)	digitalWrite(gpio[i],0);
		
	return 0;
}



/**************************************** SECUENCIA CHOQUE ****************************************/



int choque (int modo){
		
	int i;

	int const tabla[7]={129,66,36,24,36,66,129};
	
	if( set_gpio() )	exit(1);
	
	float vel = set_vel();
	
	if( modo == 0)	algoritmo_tabla( tabla , 7 , vel );
	
	else if( modo == 1 ){
		if( serie( tabla , 7 , vel ) == 1 )	 
			return 1;
	}
	
	else exit(1);
	
	for(i=0; i<N; i++)	digitalWrite(gpio[i],0);
		
	return 0;
}



/***************************************** SECUENCIA TREN *****************************************/



int tren (int modo){
	
	int i;
	int const tabla[8]={7,14,28,56,112,224,193,131};
				
	if(set_gpio())	exit(1);
	
	float vel = set_vel();
	
	if( modo == 0)	algoritmo_tabla( tabla , 8 , vel );
	
	else if( modo == 1 ){
		if( serie( tabla , 8 , vel ) == 1 )	 
			return 1;
	}
	
	else exit(1);
	
	for(i=0; i<N; i++)	digitalWrite(gpio[i],0);
		
	return 0;
}




/**************************************** ALGORITMO TABLA *****************************************/



void algoritmo_tabla(int const *tabla, int tam, float vel ){
	
	int i,j,decimal;
	char monitor = 'p';
	
	do{
		do{
			flushinp();
			for( i=0; i<tam; i++){	
			
			decimal = *(tabla+i);										/* se copia para no modificar la tabla original */
		
			for (j=0; j<N; j++){										/* decimal a binario. gpio[N] se toma como una palabra binaria */
				if(decimal != 0)	digitalWrite( gpio[j], (decimal%2));
				else	digitalWrite( gpio[j], 0);						
				decimal/=2;
			}
			delay(vel);
			}
			
			monitor = getch();
				
		}while( (monitor!= 'e') && (monitor != 'u') && (monitor != 'd'));
	
	if( monitor != 'e')
		vel = control_velocidad(monitor, vel);
			
	}while(monitor != 'e');

	clear();
	refresh();
	
}



/*************************************** COMUNICACIÓN SERIE ***************************************/



int serie (int const *tabla, int tam, float vel ){						/* devuelve 1 por error y 0 por finalización correcta */
	
	int i=0, puerto, dato_in;											/* dato_in integer recibido por serie */
	char dato_enviado, dato_recibido;									/* dato_recibido char recibido por serie */
	
	char monitor = 'p';
	
	if( (puerto = serialOpen("/dev/ttyAMA0", 9600)) < 0)	return 1;
	
	do{
		do{
			flushinp();	
			dato_enviado = *(tabla+i);	
			serialPutchar( puerto, dato_enviado);
			
			i++;
			if(i==tam) i=0;
			
			delay(vel);													/* delay entre envio y recepcion para la misma raspberry */
			
			if(serialDataAvail (puerto)){
				
				dato_recibido = serialGetchar(puerto);
				dato_in = dato_recibido;
				
				if( dato_in >=0 && dato_in<256){
					for(int i=0; i<N;i++){
						if(dato_in!=0)	digitalWrite(gpio[i], (dato_in % 2));
						else digitalWrite(gpio[i],0);
						dato_in = dato_in /2;
				
						refresh();
					}
				}
				refresh();
			}
							
			monitor = getch();
					
		}while( (monitor!= 'e') && (monitor != 'u') && (monitor != 'd'));
	
	if( monitor != 'e')
		vel = control_velocidad(monitor, vel);
			
	}while(monitor != 'e');

	clear();
	refresh();
	return 0;
}
