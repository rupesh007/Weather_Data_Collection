#include "nRF24L01.h"



// set both RX_ADDR_P0 and TX_ADDR addresses of the nRF24L01
// the default SETUP_AW, which is 5 bytes, is left unchanged
void nRF24L01_setRxTxAddr(void){

	BYTE i;
	
	// define the addresses of RX_ADDR_P0 and TX_ADDR, LSBs first
	BYTE RxAddrP0[5] = { 0x01, 0x01, 0x05, 0x01, 0x01};
	BYTE TxAddr[5] = { 0x01, 0x01, 0x05, 0x01, 0x01};
	
	// set the RX_ADDR_P0, 5 bytes
	nRF24L01_sendPayload( W_REGISTER | RX_ADDR_P0, RxAddrP0, 5);
	
	// set the TX_ADDR, 5 bytes
	nRF24L01_sendPayload( W_REGISTER | TX_ADDR, TxAddr, 5);
		

}


// Sends one command word or byte to the nRF24L01
BYTE nRF24L01_sendOneByte(BYTE oneByte){
	
	BYTE status = 0x00;
	
	// nRF24_CSN off
	Port_0_Data_SHADE &= ~nRF24_CSN_MASK;
	PRT0DR = Port_0_Data_SHADE;
	
	while( ! (SPIM_bReadStatus() & SPIM_SPIM_TX_BUFFER_EMPTY ) );  
		SPIM_SendTxData( oneByte ); 
	
	while( SPIM_bReadStatus() & SPIM_SPIM_RX_BUFFER_FULL );
		status = SPIM_bReadRxData();
	
	// nRF24_CSN on
	Port_0_Data_SHADE |= nRF24_CSN_MASK;
	PRT0DR = Port_0_Data_SHADE;
	
	Delay50uTimes(20);
	
	return(status);
}


// Sends instruction commands to the nRF24L01 
BYTE nRF24L01_sendInstruction(BYTE instructionWord, BYTE mapAddr){

	BYTE status;

	// nRF24_CSN off
	Port_0_Data_SHADE &= ~nRF24_CSN_MASK;
	PRT0DR = Port_0_Data_SHADE;
	
	while( ! (SPIM_bReadStatus() & SPIM_SPIM_TX_BUFFER_EMPTY ) );  
		SPIM_SendTxData( instructionWord );
	
	while( SPIM_bReadStatus() & SPIM_SPIM_RX_BUFFER_FULL );
		status = SPIM_bReadRxData();
		
	while( ! (SPIM_bReadStatus() & SPIM_SPIM_TX_BUFFER_EMPTY ) );  
		SPIM_SendTxData( mapAddr );
		
	while( SPIM_bReadStatus() & SPIM_SPIM_RX_BUFFER_FULL );
		status = SPIM_bReadRxData();
	
	// nRF24_CSN on
	Port_0_Data_SHADE |= nRF24_CSN_MASK;
	PRT0DR = Port_0_Data_SHADE;

	Delay50uTimes(20);

	return(status);
}


// Sends multiple bytes, such as pipe addresses, to the nRF24L01 
// dataArray[] must exist
void nRF24L01_sendPayload(BYTE instructionWord, BYTE* dataArr, BYTE bytes) {

	BYTE i;

	// nRF24_CSN off
	Port_0_Data_SHADE &= ~nRF24_CSN_MASK;
	PRT0DR = Port_0_Data_SHADE;
	
	while( ! (SPIM_bReadStatus() & SPIM_SPIM_TX_BUFFER_EMPTY ) );  
		SPIM_SendTxData( instructionWord );
	
	while( SPIM_bReadStatus() & SPIM_SPIM_RX_BUFFER_FULL );
		SPIM_bReadRxData();
		
	for(i = 0 ; i < bytes ; i++) {

		while( ! (SPIM_bReadStatus() & SPIM_SPIM_TX_BUFFER_EMPTY ) ){};  
		SPIM_SendTxData( dataArr[i] );	
		
		while( SPIM_bReadStatus() & SPIM_SPIM_RX_BUFFER_FULL );
		SPIM_bReadRxData();
				
	}
	

	// nRF24_CSN on
	Port_0_Data_SHADE |= nRF24_CSN_MASK;
	PRT0DR = Port_0_Data_SHADE;
	
	Delay50uTimes(20);
}


// Transmits payload via a nRF24L01 to a receiving nRF24L01
// this function must be called after sendPayload()
void nRF24L01_transmitPayload(void){

    // nRF24_CE on
	Port_0_Data_SHADE &= ~nRF24_CE_MASK;
	PRT0DR = Port_0_Data_SHADE;

    Delay50uTimes(1);
	
    // nRF24_CE off
	Port_0_Data_SHADE |= nRF24_CE_MASK;
	PRT0DR = Port_0_Data_SHADE;
	
	Delay50uTimes(20);
}


// Reads multiple bytes. Note: first byte is STATUS register of the nRF24L01
// dataArray[] must exist
void nRF24L01_readPayload( BYTE instructionWord, BYTE* dataArr, BYTE bytes ) {

	BYTE i;

	// nRF24_CSN off
	Port_0_Data_SHADE &= ~nRF24_CSN_MASK;
	PRT0DR = Port_0_Data_SHADE;
	
	while( ! (SPIM_bReadStatus() & SPIM_SPIM_TX_BUFFER_EMPTY ) );  
		SPIM_SendTxData( instructionWord );
		
	while( SPIM_bReadStatus() & SPIM_SPIM_RX_BUFFER_FULL ); 
	dataArr[0] = SPIM_bReadRxData(); 
		
	for(i = 0 ; i < bytes ; i++) {

		while( ! (SPIM_bReadStatus() & SPIM_SPIM_TX_BUFFER_EMPTY ) );  
		SPIM_SendTxData( DUMMYDATA );
		
		while( SPIM_bReadStatus() & SPIM_SPIM_RX_BUFFER_FULL ); 
		dataArr[i] = SPIM_bReadRxData();  
			
	}

	// nRF24_CSN on
	Port_0_Data_SHADE |= nRF24_CSN_MASK;
	PRT0DR = Port_0_Data_SHADE;
	
	Delay50uTimes(20);
	
}


void nRF24L01_commonConfiguration(void){
	
	// set CRC to 2 bytes in the CONFIG and disable interrupts on IRQ
	nRF24L01_sendInstruction( W_REGISTER | CONFIG, 0x7C );
	
	// disable EN_AA for all pipes
	//nRF24L01_sendInstruction( W_REGISTER | EN_AA, 0x00 );
	
	// ARC set to 15  
	//nRF24L01_sendInstruction( W_REGISTER | SETUP_RETR, 0x0F );
	
	// set data rate to 1Mbps
	nRF24L01_sendInstruction( W_REGISTER | RF_SETUP, 0x07 );

	// set addresses for RX_ADDR_P0 and TX_ADDR, change values in function
	nRF24L01_setRxTxAddr();

	// set RX_PW_P0 to 2 bytes 
	nRF24L01_sendInstruction( W_REGISTER | RX_PW_P0, 0x02 );

}


// set as transmiter and power up the nRF24L01
void nRF24L01_SetAsTransmiter( void ){

	BYTE presentConfig;
	
	// nRF24_CE off
	Port_0_Data_SHADE &= ~nRF24_CE_MASK;
	PRT0DR = Port_0_Data_SHADE;
	
	presentConfig = nRF24L01_sendInstruction(R_REGISTER | CONFIG, DUMMYDATA);	 
	nRF24L01_sendInstruction( W_REGISTER | CONFIG, presentConfig & ~0x01 );
	presentConfig = nRF24L01_sendInstruction(R_REGISTER | CONFIG, DUMMYDATA);
	nRF24L01_sendInstruction( W_REGISTER | CONFIG, presentConfig | 0x02 );
	
	// nRF24_CE on
	//Port_0_Data_SHADE |= nRF24_CE_MASK;
	//PRT0DR = Port_0_Data_SHADE;
}

// set as reciever and power up the nRF24L01
void nRF24L01_SetAsReciever( void ){

	BYTE presentConfig;
	
	// nRF24_CE off
	Port_0_Data_SHADE &= ~nRF24_CE_MASK;
	PRT0DR = Port_0_Data_SHADE;
	
	presentConfig = nRF24L01_sendInstruction(R_REGISTER | CONFIG, DUMMYDATA);
	nRF24L01_sendInstruction( W_REGISTER | CONFIG, presentConfig | 0x03 );
	
	// nRF24_CE on
	Port_0_Data_SHADE |= nRF24_CE_MASK;
	PRT0DR = Port_0_Data_SHADE;
	
}


// checks if payload's been received
BYTE nRF24L01_is_RX_DR( void ){

	BYTE tmp;

	// check the RX_DR of the STATUS register
	if( nRF24L01_sendOneByte( NOP ) & RX_DR_MASK ) 
	tmp = 1;
	
	else 
		tmp = 0;

	return tmp;
}

// checks if payload was successfully sent
BYTE nRF24L01_is_TX_DS( void ){

	BYTE tmp;

	// check the TX_DS of the STATUS register
	if( nRF24L01_sendOneByte( NOP ) & TX_DS_MASK )
		tmp = 1;
	
	else 
		tmp = 0;

	return tmp;

}

// checks if MAX_RT is set
BYTE nRF24L01_is_MAX_RT( void ){

	BYTE tmp;

	// check the TX_DS of the STATUS register
	if( nRF24L01_sendOneByte( NOP ) & MASK_MAX_RT )
		tmp = 1;
	
	else 
		tmp = 0;

	return tmp;

}


// clears RX_DR interrupt in STATUS
void nRF24L01_clear_RX_DR( void ){

	BYTE presentStatus;
	presentStatus = nRF24L01_sendOneByte( NOP );
	nRF24L01_sendInstruction( W_REGISTER | STATUS, presentStatus |= MASK_RX_DR );
	
}

// clears TX_DS interrupt in STATUS
void nRF24L01_clear_TX_DS( void ){

	BYTE presentStatus;
	presentStatus = nRF24L01_sendOneByte( NOP );
	nRF24L01_sendInstruction( W_REGISTER | STATUS, presentStatus |= MASK_TX_DS );
	
}

// clears MAX_RT interrupt in STATUS
void nRF24L01_clear_MAX_RT( void ){

	BYTE presentStatus;
	presentStatus = nRF24L01_sendOneByte( NOP );
	nRF24L01_sendInstruction( W_REGISTER | STATUS, presentStatus |= MASK_MAX_RT );
	
}