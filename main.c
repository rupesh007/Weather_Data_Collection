//----------------------------------------------------------------------------
// C main line
//----------------------------------------------------------------------------

#include <m8c.h>        // part specific constants and macros
#include "PSoCAPI.h"    // PSoC API definitions for all User Modules
#include "nRF24L01.h"

#define PAYLOAD_ARRAY_MAX 2
char thedata[25];
char theStr[] = "tmpr=";
char theStr1[] = "clear";
int i;

char *PbStrPtr = theStr;
char the[] = "NOP";
char * strPtr = thedata;

void main(void)

{	
	BYTE payloadArray[PAYLOAD_ARRAY_MAX] = {0x00,0x00};
//	for (i=0;i<=15;i++){ 
//	payloadArray[i]=0x00;
//	}   
    
	
	M8C_EnableGInt ; // Uncomment this line to enable Global Interrupts
	
	LCD_Start(); //Initialize the LCD
	
	SPIM_Start(SPIM_SPIM_MODE_0 | SPIM_SPIM_MSB_FIRST);
	
	nRF24L01_commonConfiguration();
	nRF24L01_SetAsTransmiter();
	
	
	UART_EnableInt();
	
	 // Parameter pointer
	UART_CmdReset(); // Initialize receiver/cmd
	// buffer
	UART_IntCntl(UART_ENABLE_RX_INT); // Enable RX interrupts

	UART_Start(UART_PARITY_NONE); // Enable UART
	
	UART_CPutString("\r\nwelcome to the uart\r\n");
	
	
	while(1){
	 for(i=0;i<26;i++){
		payloadArray[0] = thedata[i];
//		payloadArray[1]--;



		
					if(UART_bCmdCheck()) { // Wait for command
					
					if(strPtr = UART_szGetParam()) { // More than delimiter"
					
					//UART_CPutString("Found valid command\r\nCommand =>");
					
					UART_PutString(strPtr); // Print out command
					UART_CPutString(" ");
					while(strPtr = UART_szGetParam()) { // loop on each parameter
					UART_CPutString(" ");
					UART_PutString(strPtr); // Print each parameter
					UART_CPutString(" ");
					}
					}
					UART_CmdReset(); // Reset command buffer
					UART_CPutString("\r\n");
					}
				
		
		
		
			//memcpy (payloadArray,&strPtr,strlen(strPtr)+1);
	
    
	  while( nRF24L01_is_TX_DS() != 1 ){//unless the payload is successfully sent
	  
		nRF24L01_sendPayload(W_TX_PAYLOAD, payloadArray, 2);
		nRF24L01_transmitPayload();
		LCD_Position(1,6);
		LCD_PrString(theStr);
		Delay50uTimes(1);
		
		
		if(nRF24L01_is_MAX_RT()){
		nRF24L01_clear_MAX_RT();		
		
	    LCD_Position(1,0);
		LCD_PrString(theStr1);	
		break;
		}		
		}
		// Clear the interrupt status
			nRF24L01_clear_TX_DS();
		
		Delay10msTimes(100);
		LCD_Position(0,0);
		LCD_PrString(payloadArray);
			
		
		
	}	
	}	
	
}
