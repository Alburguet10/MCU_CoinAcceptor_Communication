/*
 * Comunicaci�n_Aceptador.c
 *
 * Created: 23/11/2021 04:53:26 p. m.
 * Author: Eva Beltr�n, Emiliano Vel�zquez, Alan Burguete
 */

#include <io.h>
#include <delay.h>
#include <stdio.h> 

//Intrucci�n Coin Type
unsigned char coinType[11]           = {0x50,0x06,0x01,0x7A,0x14,0x0C,0xFF,0xFF,0x00,0x00,0x11};
unsigned respuestaCoinType[17];
//Instrucci�n Poll
unsigned char poll[7]                = {0x50,0x02,0x01,0x7A,0x14,0x0B,0x14};
unsigned char respuestaPoll[15];
//Instrucci�n Dispense
unsigned char dispenseUno[8]         = {0x50,0x03,0x01,0x7A,0x14,0x0D,0x13,0xFE};
unsigned char respuestaDispense[14];
//Instrucci�n Tube Status
unsigned char tubeStatus[7]           = {0x50,0x02,0x01,0x7A,0x14,0x0A,0x15};
unsigned char respuestaTubeStatus [32];

int i;
unsigned int contador = 0;

void main(void)
{
    // USART initialization
    // Communication Parameters: 8 Data, 1 Stop, No Parity
    // USART Receiver: On
    // USART Transmitter: On
    // USART0 Mode: Asynchronous
    // USART Baud Rate: 9600 (Double Speed Mode)
    UCSR0A=(0<<RXC0) | (0<<TXC0) | (0<<UDRE0) | (0<<FE0) | (0<<DOR0) | (0<<UPE0) | (1<<U2X0) | (0<<MPCM0);
    UCSR0B=(0<<RXCIE0) | (0<<TXCIE0) | (0<<UDRIE0) | (1<<RXEN0) | (1<<TXEN0) | (0<<UCSZ02) | (0<<RXB80) | (0<<TXB80);
    UCSR0C=(0<<UMSEL01) | (0<<UMSEL00) | (0<<UPM01) | (0<<UPM00) | (0<<USBS0) | (1<<UCSZ01) | (1<<UCSZ00) | (0<<UCPOL0);
    UBRR0H=0x00;
    UBRR0L=0x0C;       
    DDRD.6 = 1;
    DDRD.7 = 1;
    
    //Enviar datos de Coin Type
     for(i = 0; i < 11; i++)
       putchar(coinType[i]); 
           
     // Se espera que la respuesta sea recibida
     while (!( UCSR0A & (1<<UDRE0)));     
     for(i = 0;  i < 17; i++)
        respuestaCoinType[i] = getchar();   
      
while (1){ 
      //Enviar datos de Poll   
      for(i = 0; i < 7; i++)
        putchar(poll[i]);
                             
      // Se espera que la respuesta sea recibida
      while (!( UCSR0A & (1<<UDRE0)));     
      for(i = 0; i < 13; i++)
           respuestaPoll[i] = getchar();    
           
      //Esperar un tiempo para mandar siguiente comando
      delay_ms(5000);     
           
      //Enviar datos de Tube Status   
      for(i = 0; i < 7; i++)
        putchar(tubeStatus[i]);
                                
      // Se espera que la respuesta sea recibida
      while (!( UCSR0A & (1<<UDRE0)));     
      for(i = 0; i < 31; i++)
           respuestaTubeStatus[i] = getchar();

      //Esperar un tiempo para mandar siguiente comando
      delay_ms(2000);   
           
     //Checar el byte donde est� la cantidad de monedas de $1 instertadas
      contador = respuestaTubeStatus[17];
              
      //Los LEDs indican en binario la cantidad de monedas que hay en el aceptador
      if(contador == 1){
      
        PORTD.7 = 1;
        delay_ms(1000);
        PORTD.7 = 0;
        delay_ms(1000);
      
      } else if(contador == 2){
      
        PORTD.6 = 1;
        delay_ms(1000);
        PORTD.6 = 0;
        delay_ms(1000);  
        
    } else if(contador == 3){
      
        PORTD.6 = 1;  
        PORTD.7 = 1;
        delay_ms(1000);
        PORTD.6 = 0;  
        PORTD.7 = 0;
        delay_ms(1000);
    }else{
    
        PORTD.6 = 1;  
        PORTD.7 = 1;
        delay_ms(100);
        PORTD.6 = 0;  
        PORTD.7 = 0;
        delay_ms(100);
    
    }
             
    //Mandar comando para que el aceptador vaya soltando las monedas hasta que no quede ninguna
    if(contador > 0){
    //Enviar datos de Dispense  
      for(i = 0; i < 8; i++)
        putchar(dispenseUno[i]);
                                
      // Se espera que la respuesta sea recibida
      while (!( UCSR0A & (1<<UDRE0)));     
      for(i = 0; i < 14; i++)
           respuestaDispense[i] = getchar();
           
      contador--;
    }
      
      delay_ms(2000); 
  }
}