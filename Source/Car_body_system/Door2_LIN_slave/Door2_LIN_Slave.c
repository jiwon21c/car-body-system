/*
****************************************************************************
** car-body-system project/LIN-es ablakvez�rl�k
**
** Verzi� :     D�tum:         Szerz�:      Comment:
**    1.0       11.11.2009    B.L.          
****************************************************************************
*/


/*__________________________________________________________________________*/
/*_____ I N C L U D E S ____________________________________________________*/
/*__________________________________________________________________________*/
#include "config.h"
#if LIN_CONFIG == 1 //1:slave 0:master
#include "lib_mcu/lin_uart/slave_lin.h"
#else
#include "lib_mcu/lin_uart/master_lin.h"
#endif

#include "lib_mcu/lin_uart/lin_lib.h"
#include "avr/io.h"
#include <avr/interrupt.h>
#include "platform.h"
#include "mcu_avr_atmega128_api.h"
#include "dpy_trm_s01.h"

U8 ID;

//#define SLAVE1
#define SLAVE2

// K�t Slave van, melyeknek k�l�nb�z� az el�r�si c�me, �gy k�tf�lek�ppen ford�that� a k�d

#ifdef SLAVE1
  ID = 0x00;
#endif
#ifdef SLAVE2
  ID = 0x01;
#endif


U8 Buf_SET_SLAVE [4];
t_frame MESS_SET_SLAVE;
U8 edge;
U8 lock = 0x00;


int main (void) {
  
  U8 number_of_frame ;

  //Slave frame inicializ�l�sa
  MESS_SET_SLAVE.frame_id    = ID;
  MESS_SET_SLAVE.frame_size  = 4 ;
  MESS_SET_SLAVE.frame_type  = STANDART_LIN_FRAME_TYPE ;
  MESS_SET_SLAVE.frame_data  = Buf_SET_SLAVE;

  number_of_frame = 1;
  my_schedule.frame_message[0] = MESS_SET_SLAVE;
  my_schedule.number_of_frame = number_of_frame;

  //Kijelz�k�rtya inicializ�l�sa
  dpy_trm_s01__Init();
  
  //LIN Controller inicializ�l�sa
  lin_init();

  //Glob�lis interrupt enged�lyez�s
  sei();
  
  Buf_SET_SLAVE[0]=0;

  while(1) {
  
  //A laborban haszn�lt motort a LED-ek vez�rl�s�vel tudjuk forgatni
  //(ugyanarra a port-ra van k�tve a k�t jel) A LED3 enged�lyez� jel,
  //A LED1, illetve LED2-vel jobbra vagy balra tudjuk forgatni a motort.
  
  //Forgat�motor enged�lyez�se
	DPY_TRM_S01__LED_3_ON();
    
   //Ha az �zenet 4. bitje akt�v, jobbra forgatunk
	 if (Buf_SET_SLAVE[0] == 16) DPY_TRM_S01__LED_1_ON(); 
	 	else DPY_TRM_S01__LED_1_OFF();
	 //Ha az �zenet 5. bitje akt�v, balra forgatunk
	 if (Buf_SET_SLAVE[0] == 32) DPY_TRM_S01__LED_2_ON();
	 	else DPY_TRM_S01__LED_2_OFF();
	 
	 //A k�zponti z�r akkor v�lt st�tuszt, ha egy felfut� �let �rz�kel�nk
	 //az �zenet 6. bitj�n. Az �l�rz�kel�st az edge v�ltoz�ban t�roljuk,
	 //a z�r st�tusz�t a lock v�ltoz�ban (�s ezt jelzi a LED 4 �llapota is)
	 
	 edge = (edge & 0x01) << 1 + ((Buf_SET_SLAVE[0]) & 0x40) >> 6;
	 if ((edge & 0x03) == 1) lock = ~lock;
	 
	 if (lock != 0x00) DPY_TRM_S01__LED_4_ON();
	 	else DPY_TRM_S01__LED_4_OFF();  
	 	
	 //A vett LIN-es �zenet kijelz�se a h�tszegmenses kijelz�n  
	dpy_trm_s01__7seq_write_number(Buf_SET_SLAVE[0],0);
			 
  }
  
  return 0;
}




