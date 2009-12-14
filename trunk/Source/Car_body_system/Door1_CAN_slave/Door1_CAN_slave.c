/** \file Door1_CAN_slave.c
* \brief Controller of doors connected to CAN bus
* \author Software Developer Team
* \version 1.0
* \date 2009-12-06
*/


//$WCDATE$
//$WCREV$
// ver: 1.0 (nem tal�ltam a megfelel� kulcssz�t, hogy automatikusan t�ltse ki a verzi�sz�mot)

#include "config.h"
#include "platform.h"
#include "dpy_trm_s01.h"
#include "mcp2515.h"
#include "mcu_avr_atmega128_api.h"

CAN_message received,sent;
U8 active, openclose;
/* ************************************************************************** */
/*
** Description :	The main C function.  Program execution starts here.
**
** ------------------------------------------------------------------------------
*/

/** \fn int main (void)
*   \brief main function
*   \param nothing
*	\return nothing
*/
int main (void) {
  // Initialize
  sei(); 		/* Interrupts globally enabled */
  mcp2515_init(); // CAN initialization
  dpy_trm_s01__Init(); // Display card initialization

  sent.id = 0x0100;        //Message ID = 0x100;
  sent.rtr = 0;
  sent.length = 8;

  // nyitva van az ajt�
  openclose = 0;

  while(1) {

  	// Ablakemel�s az ajt�n l�v� gombbal
	if(DPY_TRM_S01__BUTTON_1_GET_STATE()){
		DPY_TRM_S01__LED_3_ON();
		DPY_TRM_S01__LED_1_ON();
	}
	else{
		DPY_TRM_S01__LED_3_OFF();
		DPY_TRM_S01__LED_1_OFF();
	}
	// Ablak leenged�s az ajt�n l�v� gombbal
	if(DPY_TRM_S01__BUTTON_2_GET_STATE()){
		DPY_TRM_S01__LED_3_ON();
		DPY_TRM_S01__LED_2_ON();
	}
	else{
		
		DPY_TRM_S01__LED_3_OFF();
		DPY_TRM_S01__LED_2_OFF();
	}
	// K�zponti z�r CAN �zenetet kell k�ldeni a t�bbi ajt� vez�rl� modulj�nak is
	if(DPY_TRM_S01__BUTTON_3_GET_STATE()){
		if(openclose = 1) openclose = 0;
		else openclose = 1;
		DPY_TRM_S01__LED_4_ON();
		sent.data[4] = 64;
		can_send_standard_message(&sent);
	}
	else{
		DPY_TRM_S01__LED_4_OFF();
	}


    // CAN �zenet v�tele
	can_receive_message(&received);
	// A CAN �zenet a korm�nyt�l �rkezett-e
	if( received.id == 0x100){
		// k�zponti z�r
		if (received.data[4] == 64){
			if(openclose = 1) openclose = 0;
			else openclose = 1;
			DPY_TRM_S01__LED_4_ON();
		}
		else
			DPY_TRM_S01__LED_4_OFF();
		// Az 1-es ajt�vez�rl�nek sz�l-e az �zenet
		if(received.data[4] == 1) {
			// motor enged�lyez�se
			active = 1;
			DPY_TRM_S01__LED_3_ON();
			dpy_trm_s01__7seq_write_number(received.data[4],0);
		}
		else if(received.data[4] != 16 || received.data[4] != 32){
			// motor tilt�sa
			active = 0;
			DPY_TRM_S01__LED_3_OFF();
		}
		if(active == 1){
			// Ablakemel�s
			if (received.data[4] == 16)
				DPY_TRM_S01__LED_1_ON(); 
		 	else 
				DPY_TRM_S01__LED_1_OFF();
			// Ablak leenged�se
		 	if (received.data[4] == 32)
				DPY_TRM_S01__LED_2_ON();
		 	else
				DPY_TRM_S01__LED_2_OFF();
		}
		_delay_ms(10);
	}
  }//while(1)
  return 0;
}//main
