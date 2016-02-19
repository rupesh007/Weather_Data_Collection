//----------------------------------------------------------------------------
// C main line
//----------------------------------------------------------------------------

#include <m8c.h>
#include "PSoCAPI.h"

void initialisation();

BYTE data_received;



void main()

{
		
		initialisation();
		
	while(1) {
		
				if(UART_RX_REG_FULL){//to make sure if rx register has unread data
					data_received=UART_bReadRxData(); //reads rx register and returns the data					
					
						UART_SendData(data_received); //initiates data transmission loading tx buffer
					    UART_PutSHexByte(data_received);
						
						}
				
				
			
}
}
void initialisation(){
       // Counter8_Start();
		//Counter8_EnableInt;
		//Counter8_WritePeriod(155); // Set up baud rate generator
        //Counter8_WriteCompareValue(77);
		UART_IntCntl(UART_ENABLE_RX_INT);
		UART_IntCntl(UART_ENABLE_TX_INT);
	
	
		UART_Start(UART_PARITY_NONE); // Enable UART
		
		M8C_EnableGInt ; // Turn on interrupts
}