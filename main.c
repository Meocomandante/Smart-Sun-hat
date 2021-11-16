#include <p24fxxxx.h>
#include <stdio.h>
#include <string.h>

// Configuration Bits
#ifdef __PIC24FJ64GA004__ //Defined by MPLAB when using 24FJ64GA004 device
_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx1 & IOL1WAY_ON) 
_CONFIG2( FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRI & I2C1SEL_SEC)
#else
_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx2) 
_CONFIG2( FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRI)
#endif


/*****************************///potenc
/*int ADCValue;
AD1PCFG = 0xFFDF; 
AD1CON1 = 0x0000; 

AD1CHS = 0x0005; // Connect AN5 as CH0 input
// in this example AN5 is the input
AD1CSSL = 0;
AD1CON1bits.ADON = 1; // turn ADC ON
while (1) // repeat continuously
{
AD1CON1bits.SAMP = 1; // start sampling...
Delay(); // Ensure the correct sampling time has elapsed
// before starting conversion.
AD1CON1bits.SAMP = 0; // start Converting
while (!AD1CON1bits.DONE); // conversion done?
ADCValue = ADC1BUF0; // yes then get ADC value
}*/
/*******************************/

void inicializar_adc(){
	
	AD1PCFG = 0xFFD1;	//1FFD1	//todos os registos necessarios para o potenciometros, LDR's e temperatura, em hexadecimal (AN1, AN2, AN5, AN3)
	AD1CON1 = 0x0000; // SAMP bit = 0 ends sampling
	// and starts converting
	AD1CSSL = 0;	// in this example AN2 is the input
	//TRISAbits.TRISA0 = 0;   //output

	AD1CON1bits.ADON = 1; // turn ADC ON
}

void inicializar_UART(){
	U2BRG = 25; //Valor calculado atraves da formula presente no datasheet, para um baud rate de 9600
	U2STA = 0;
	U2MODE = 0x8000; //Enable Uart for 8-bit data
	//no parity, 1 STOP bit
	U2STAbits.UTXEN = 1; //Enable Transmit
}

void putCharUART( char c) {
	int i;
//	while( U2STAbits.UTXBF);    //UTXBF = registo que determina se o buffer ta cheio ou nao. Se for = 1, o buffer esta cheio e nao se pode escrever
								// se for = 0, o buffer nao esta cheio e pode escrever-se pelo menos um caracter	  
	U2TXREG = c; 			//buffer onde se mete o caracter escolhido. TXREG = transmission register

	for(i=0;i<2000; i++);   //é necessário ter este for que serve como delay, sem este for o ecrã só recebe lixo, pois é rápido demais
}

char getCharUART(){

	while( ! U2STAbits.URXDA );	//URXDA = registo que determina se o buffer tem alguma coisa para ler ou nao. Se for = 0, o buffer ta vazio e nao pode ler nada
								//se for = 1, o buffer não está vazio e pode ler-se pelo menos um caracter
	IFS1bits.U2RXIF = 0;
	return U2RXREG;

}

void putStringUART( char *str ) {		//*str -> apontador para a string str

	int i, string_lenght = strlen(str);		//strlen = tamanho da string
	
	for(i=0; i<string_lenght; i++){
		putCharUART( *str );
		*str++;
	}	
for( i = 0 ; i < 2000 ; i++){};	//delay
}


int valor_potenciometro(){
	int i = 0;

	AD1CHS = 5; // Connect AN5 as CH0 input

	AD1CON1bits.SAMP = 1; // start sampling...
	for( i = 0 ; i < 20000 ; i++){};	//delay
	AD1CON1bits.SAMP = 0; // start Converting
	while (!AD1CON1bits.DONE); // conversion done?
	return ADC1BUF0; // yes then get ADC value  (1-1024)
}


int valor_LDR1(){
	int i = 0;

	AD1CHS = 1; // Connect AN1 as CH0 input

	AD1CON1bits.SAMP = 1; // start sampling...
	for( i = 0 ; i < 20000 ; i++){};	//delay
	AD1CON1bits.SAMP = 0; // start Converting
	while (!AD1CON1bits.DONE); // conversion done?
	return ADC1BUF0; // yes then get ADC value  (1-1024)
}

int valor_LDR2(){
	int i = 0;


	AD1CHS = 3; // Connect AN3 as CH0 input

	AD1CON1bits.SAMP = 1; // start sampling...
	for( i = 0 ; i < 20000 ; i++){};	//delay
	AD1CON1bits.SAMP = 0; // start Converting
	while (!AD1CON1bits.DONE); // conversion done?
	return ADC1BUF0; // yes then get ADC value  (1-1024)
}

int sensor_temperatura(){
int i = 0;


	AD1CHS = 0x0002; // Connect AN2 as CH0 input

	AD1CON1bits.SAMP = 1; // start sampling...
	for( i = 0 ; i < 20000 ; i++){};	//delay
	AD1CON1bits.SAMP = 0; // start Converting
	while (!AD1CON1bits.DONE); // conversion done?
	return ADC1BUF0; // yes then get ADC value  (1-1024)
}

void modo_normal(int sensor_esquerda, int sensor_direita){


	int i;	                      
	
	if(sensor_esquerda > sensor_direita){
		//move o motor para a esquerda		

		LATAbits.LATA0 = 1;			//	PORTAbits.RA0 = 1;
		LATAbits.LATA1 = 0; 		//	PORTAbits.RA1 = 0;
		for( i = 0 ; i < 20000 ; i++){};	
		LATAbits.LATA0 = 0;			//	PORTAbits.RA0 = 0;
		LATAbits.LATA1 = 0;			//	PORTAbits.RA1 = 0;
	}
	else{
		//move o motor para a direita
		LATAbits.LATA0 = 0;			//	PORTAbits.RA0 = 0;
		LATAbits.LATA1 = 1;			//	PORTAbits.RA1 = 1;
		for( i = 0 ; i < 20000 ; i++){};	
		LATAbits.LATA0 = 0;			//	PORTAbits.RA0 = 0;
		LATAbits.LATA1 = 0;			//	PORTAbits.RA1 = 0;
	}
}

void modo_bronzear(int sensor_esquerda, int sensor_direita){

	
	int i;
	
	if(sensor_esquerda > sensor_direita){
		//move motor para a direita
		LATAbits.LATA0 = 0;			//	PORTAbits.RA0 = 0;
		LATAbits.LATA1 = 1;			//	PORTAbits.RA1 = 1;
		for( i = 0 ; i < 20000 ; i++){};	
		LATAbits.LATA0 = 0;			//	PORTAbits.RA0 = 0;
		LATAbits.LATA1 = 0;			//	PORTAbits.RA1 = 0;
	}
	else{
		//move motor para a esquerda
		LATAbits.LATA0 = 1;			//	PORTAbits.RA0 = 1;
		LATAbits.LATA1 = 0; 		//	PORTAbits.RA1 = 0;
		for( i = 0 ; i < 20000 ; i++){};	
		LATAbits.LATA0 = 0;			//	PORTAbits.RA0 = 0;
		LATAbits.LATA1 = 0;			//	PORTAbits.RA1 = 0;
	}
}







int main(void)
{

int modo = 1;
char password[4];
char c, c_pass = 'a';
char str_sensor1[50], str_sensor2[50], str_temp[50], str_pot[50], str_estado[50];
int luminosidade_sensor_1, luminosidade_sensor_2, temperatura, velocidade_ventoinha, i;
int estado = 0, ventoinha_ON = 0;
int diferenca_LDRs_anterior = 51;

TRISDbits.TRISD7 = 1;   //input
TRISDbits.TRISD6 = 1;   //input
TRISAbits.TRISA0 = 0;   //LED A0
TRISAbits.TRISA1 = 0;   //LED A1
inicializar_adc();
inicializar_UART();

if(!PORTDbits.RD6){		// inicia o programa quando se carrega no botão RD6

	putStringUART("START\r\n");

	while (1) // repeat continuously
	{
		if(estado == 0){
			putStringUART("Inserir password:\r\n");			
			for(i=0; i<3; i++){
				c_pass = getCharUART();
				password[i] = c_pass;
			}
					
			password[3] = '\0';		
	
			if(strcmp("sad", password) == 0){
				estado = 1;
				putStringUART("Aceite!\r\n");
			}
			else{
				putStringUART("Password errada.\r\n");
				estado = 0;
			}
		}
		
		else 
		if(estado == 1){
			velocidade_ventoinha = valor_potenciometro();
			luminosidade_sensor_1 = valor_LDR1();
			luminosidade_sensor_2 = valor_LDR2();
			temperatura = sensor_temperatura();			
			
			
			if(IFS1bits.U2RXIF == 1){ //U1RXIF é uma flag que é acionada quando existe algo para ser lido, ou seja, quando ha o interrupt para se ler um caracter
				estado = 3;
			//	IFS1bits.U2RXIF = 0;
				c = 'a';
				
			}
	
			else {
		//	putStringUART("entrou no else\r\n");
			if(!PORTDbits.RD7){
				estado = 4;
			}
			
			else if( (luminosidade_sensor_1 - luminosidade_sensor_2 >= 50) || (luminosidade_sensor_1 - luminosidade_sensor_2 <= -50))
				{
					if(((luminosidade_sensor_1 - luminosidade_sensor_2 >= 50) && diferenca_LDRs_anterior <= -50)			//Não queremos que o chapeu gire quando já está no lado onde há mais luz
						|| ((luminosidade_sensor_1 - luminosidade_sensor_2 <= -50) && diferenca_LDRs_anterior >= 50))		//Só gira quando ultrupassa o threshold e o LDR com maior valor atual não é o mesmo LDR com o maior valor anterior
					{
						estado = 2;	
						diferenca_LDRs_anterior = luminosidade_sensor_1 - luminosidade_sensor_2;
					}
			
				}
	
			else if( temperatura / 10 >= 30 ) {		//para 30 graus, mudar aqui consoante a temperatura a que se efetua o teste
				estado = 5;
			}
	
			else if( temperatura / 10 < 30 ) {
				estado = 6;		
			}
	
			}
		}		
					
		//Estado 2 - Aciona o modo
		else if(estado == 2){
			if(modo == 1) //1-modo normal 2-modo bronzear
				modo_normal(valor_LDR1(), valor_LDR2());
			if(modo == 2)
				modo_bronzear(valor_LDR1(), valor_LDR2());
			
			estado = 1;
		}
	
		//Estado 3 - Imprime parametro
		else if(estado == 3){ 
			//quando recebe uma letra	
			c = getCharUART();
			if(c == 'T') {
				sprintf(str_temp, "Sensor temperatura : %d \r\n", sensor_temperatura());
				putStringUART(str_temp);
			}
			else if(c == 'P'){	
					if(ventoinha_ON == 1){
						sprintf(str_pot, "Velocidade da ventoinha: %d \r\n", valor_potenciometro());
						putStringUART(str_pot);
					}
					else
						putStringUART("A ventoinha encontra-se desligada\r\n");
			}	
			else if(c == 'L'){	
				sprintf(str_sensor1, "Sensor 1 LDR : %d \r\n", valor_LDR1());
				putStringUART(str_sensor1);
				sprintf(str_sensor2, "Sensor 2 LDR : %d \r\n", valor_LDR2());
				putStringUART(str_sensor2);
			}
	
			estado = 1;
		}
	
		//Estado 4 - botao B2 é carregado (altera o modo)
		else if ( estado == 4 ){
			if( modo == 1){
				modo = 2;
				putStringUART("Modo Bronzear: ON\r\n");
			}
			else{
				modo = 1; 
				putStringUART("Modo Normal: ON\r\n");
			}
			estado = 1;	
		}
	
		//Estado 5 - temperatura acima dos 30ºC, liga a ventoinha
		else if(estado == 5){ 
			if(	ventoinha_ON == 0){
				putStringUART("Temperatura acima dos 30ºC, ventoinha ligada\r\n");
				ventoinha_ON = 1;
			}
			estado = 1;
		}
	
		//Estado 6 - temperatura abaixo dos 30ºC, desliga a ventoinha
		else if(estado == 6){ 
			if(	ventoinha_ON == 1){
				putStringUART("Temperatura abaixo dos 30ºC, ventoinha desligada\r\n");
				ventoinha_ON = 0;
			}
			estado = 1;
		}	
		
	}
}
}
