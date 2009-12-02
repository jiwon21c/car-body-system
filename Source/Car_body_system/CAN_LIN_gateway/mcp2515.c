#include "mcp2515.h"
#include "platform.h"

// definition of SPI chip select pin
#define MCP2515_CS_DIR_OUTPUT() 	GPIO_13_DIR_OUTPUT()	
#define MCP2515_CS_CLEAR() 			GPIO_13_CLEAR()	
#define MCP2515_CS_SET()			GPIO_13_SET()

// definition of SPI chip select pin
#define MCP2515_INT_DIR_INPUT() 	GPIO_12_DIR_INPUT()	
#define MCP2515_INT_GET_VALUE() 			GPIO_12_GET_VALUE()	



void mcp2515_init(void)
{

MCP2515_CS_DIR_OUTPUT();
MCP2515_INT_DIR_INPUT();

#if ARM_LPC2106	
	SPI_Init(0,0,0,20);
#elif AVR_ATMEGA128
	SPI_Init(0,0,0,4);
#endif


// Reseting the CAN controller
	mcp2515_reset();
	
// Disabling not used bits
	mcp2515_write( BFPCTRL, 0 ); // Deactivating RXnBF Pins
	mcp2515_write( TXRTSCTRL, 0 ); // Setting TXnRTS Bits Input
		
	mcp2515_write(CNF1,R_CNF1_500kbps);// Baudrate Prescaler setup
	mcp2515_write(CNF2,R_CNF2_500kbps);// Propagation Segment and phase 1 setup
	mcp2515_write(CNF3,R_CNF3_500kbps);// Phase 2 setup
	
	mcp2515_write( RXB0CTRL, 0 ); // Receives messages enabled by filter
	mcp2515_write( RXB1CTRL, 0 ); // Receives messages enabled by filter
	// Setting every 
	mcp2515_write( RXM1SIDH, 0 );
	mcp2515_write( RXM1SIDL, 0 );
	mcp2515_write( RXM1EID8, 0 );
	mcp2515_write( RXM1EID0, 0 );
	
	mcp2515_write( CANINTE, (1<<RX0IE));	// Setting IT control
	
	mcp2515_write( CANCTRL,(1 << OSM));  // Normal mode, One shot enabled
	
}



void can_receive_message(CAN_message *p_message)
{
	unsigned char data,i;
	while(MCP2515_INT_GET_VALUE());
	
	MCP2515_CS_CLEAR();
		SPI_byte(SPI_READ_RX,0);	// Read Buffer Command

		// Standard ID kiolvas�sa
		SPI_byte(0,&data);			
		p_message->id =  ((unsigned int) data) << 3;
		SPI_byte(0,&data);			
		p_message->id |= ((unsigned int) data) >> 5;
		
		SPI_byte(0,&data);			
		SPI_byte(0,&data);			

		// Length
		SPI_byte(0,&data);
		p_message->length = data&0xf;

		// Adatok kiolvas�sa
		for (i=0;i<p_message->length;i++) {
			SPI_byte(0,&data);
			p_message->data[i] = data;
		}


	mcp2515_bit_modify(CANINTF, (1<<RX0IF), 0);
}


void can_send_standard_message(CAN_message *p_message)
{
     unsigned char length = p_message->length;
     unsigned char i;

     // Setting the ID:
     mcp2515_write(TXB0SIDH, (unsigned char)((p_message->id >> 3)&0xFF));
     mcp2515_write(TXB0SIDL, (unsigned char)((p_message->id &0x7)<<5));
     // If the message is RTR
     if (p_message->rtr)
     {
          // RTR message has no data
          mcp2515_write(TXB0DLC, (1<<RTR) | length);
     }
     else
     {
          // Setting data length parameter
          mcp2515_write(TXB0DLC, length);
          // Copying data
          for (i=0;i<length;i++) {
               mcp2515_write(TXB0D0 + i, p_message->data[i]);
          }
     }

     // Sending CAN Message
	 mcp2515_TxBuffern_ready_to_send(0);
     
}


void mcp2515_TxBuffern_ready_to_send(unsigned short buffer)
{
	if(buffer<3)
	{
		MCP2515_CS_CLEAR();
			SPI_byte(SPI_RTS | (1<<buffer),0);	// Ready to Send command
		MCP2515_CS_SET();
	}
}

void mcp2515_Set_standard_mask_Rx0(unsigned short mask)
{
	mcp2515_write( RXM0SIDH, (unsigned char)((mask >> 3)&0xFF) );
	mcp2515_write( RXM0SIDL, (unsigned char)((mask&0x7)<<5) );

}

void mcp2515_Set_standard_mask_Rx1(unsigned short mask)
{
	mcp2515_write( RXM1SIDH, (unsigned char)((mask >> 3)&0xFF) );
	mcp2515_write( RXM1SIDL, (unsigned char)((mask&0x7)<<5) );

}

void mcp2515_Set_standard_filter_RxF0(unsigned short filter)
{
	mcp2515_write( RXF0SIDH, (unsigned char)((filter >> 3)&0xFF) );
	mcp2515_write( RXF0SIDL, (unsigned char)((filter&0x7)<<5) );

}

void mcp2515_Set_standard_filter_RxF1(unsigned short filter)
{
	mcp2515_write( RXF1SIDH, (unsigned char)((filter >> 3)&0xFF) );
	mcp2515_write( RXF1SIDL, (unsigned char)((filter&0x7)<<5) );

}

void mcp2515_Set_standard_filter_RxF2(unsigned short filter)
{
	mcp2515_write( RXF2SIDH, (unsigned char)((filter >> 3)&0xFF) );
	mcp2515_write( RXF2SIDL, (unsigned char)((filter&0x7)<<5) );

}

void mcp2515_Set_standard_filter_RxF3(unsigned short filter)
{
	mcp2515_write( RXF3SIDH, (unsigned char)((filter >> 3)&0xFF) );
	mcp2515_write( RXF3SIDL, (unsigned char)((filter&0x7)<<5) );

}

void mcp2515_Set_standard_filter_RxF4(unsigned short filter)
{
	mcp2515_write( RXF4SIDH, (unsigned char)((filter >> 3)&0xFF) );
	mcp2515_write( RXF4SIDL, (unsigned char)((filter&0x7)<<5) );

}

void mcp2515_Set_standard_filter_RxF5(unsigned short filter)
{
	mcp2515_write( RXF5SIDH, (unsigned char)((filter >> 3)&0xFF) );
	mcp2515_write( RXF5SIDL, (unsigned char)((filter&0x7)<<5) );

}


void mcp2515_reset(void)
{
	MCP2515_CS_CLEAR(); 
	SPI_byte(SPI_RESET,0);
	MCP2515_CS_SET();
}

void mcp2515_write(unsigned char adr,unsigned char data)
{
		MCP2515_CS_CLEAR();
			SPI_byte(SPI_WRITE,0);	// Writing Command
			SPI_byte(adr,0);		// Register Address
			SPI_byte(data,0);		// Data
		MCP2515_CS_SET();
}

unsigned char mcp2515_read(unsigned char adr)
{
		unsigned char read;
		
		MCP2515_CS_CLEAR();		
			SPI_byte(SPI_READ,0);	// Read command
			SPI_byte(adr,0);		// Register Address
			SPI_byte(0,&read);		// Data
		MCP2515_CS_SET();	
		return read;
}

unsigned char mcp2515_read_status(void)
{

	unsigned char read;
		
		MCP2515_CS_CLEAR();		
			SPI_byte(SPI_READ_STATUS,0);	// Read command
			SPI_byte(0,&read);			// Status
			SPI_byte(0,&read);			// Status repeated
		MCP2515_CS_SET();	

	return read;
	
}

unsigned char mcp2515_read_rx_status(void)
{
	
	unsigned char read;
		
		MCP2515_CS_CLEAR();		
			SPI_byte(SPI_RX_STATUS,0);	// Read command
			SPI_byte(0,&read);			// Status
			SPI_byte(0,&read);			// Status repeated
		MCP2515_CS_SET();	

	return read;
	
}


void mcp2515_bit_modify(unsigned char adr, unsigned char mask, unsigned char data)
{

		MCP2515_CS_CLEAR();		
			SPI_byte(SPI_READ,0);	// Read command
			SPI_byte(adr,0);		// Register Address
			SPI_byte(mask,0);		// Mask for bit modification
			SPI_byte(data,0);		// data
		MCP2515_CS_SET();	
	
}

