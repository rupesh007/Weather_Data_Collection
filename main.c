//----------------------------------------------------------------------------
// C main line
//----------------------------------------------------------------------------

#include <m8c.h>
#include "PSoCAPI.h"
#include"PSoCGPIOINT.h"

//byte sent_data;
BYTE data;
void main()
{ char *key_input;

//int i;
M8C_EnableGInt;
UART_Start(UART_PARITY_NONE); //Starts d UART enabeling null parity
UART_EnableInt();
UART_IntCntl(UART_ENABLE_RX_INT);
UART_IntCntl(UART_ENABLE_TX_INT);

while(1)
{
//data=UART_bReadRxData(); 
//data=UART_iReadChar();
//UART_SendData(data);
//UART_PutSHexByte(data);

*key_input = UART_cGetChar(); //Reads character from the Rx port
//if(UART_TX_BUFFER_EMPTY){
UART_PutChar(*key_input); //Sends a null terminated (RAM)string i.e variable to Tx
UART_bReadTxStatus();
UART_PutCRLF();
//}
}

}
