#include <avr/io.h>
#include <avr/interrupt.h>
#include "ir_wrapper.h"
#include "irmp.h"
#include "irmpconfig.h"
#include "control.h"
#include "config.h"
#include "fadingengine.h"

#ifdef USART_DEBUG
#include "usart.h"
#endif

void ir_init() {
	// Init Timer
	//OCR0A   =  (F_CPU / F_INTERRUPTS) - 1;                                    // compare value: 1/10000 of CPU frequency
	OCR0A   =  (F_CPU / F_INTERRUPTS) / 8;                                    // compare value: 1/10000 of CPU frequency
	//TCCR0B  = (1 << WGM02) | (1 << CS00);                                     // switch CTC Mode on, set prescaler to 1
	TCCR0B  = (1 << WGM02) | (1 << CS01);                                     // switch CTC Mode on, set prescaler to 8

	TIMSK0  = 1 << OCIE0A;                                                    // OCIE1A: Interrupt by timer compare
	
	irmp_init(); 
}

ISR(TIMER0_COMPA_vect, ISR_NOBLOCK)
{
  (void) irmp_ISR();                                                        // call irmp ISR
  // call other timer interrupt routines...
}


void ir_handler() {
	IRMP_DATA irmp_data;
	
	if (irmp_get_data (&irmp_data)) {

#ifdef USART_DEBUG
		usart0_puts("Received IR data\n\r");
#endif
		if ( irmp_data.protocol == IRMP_NEC_PROTOCOL ) {   // China RGB Remote
			switch ( irmp_data.command ) {
				case RC_CHINARGB_RED:
					control_cmd = CTRL_CMD_SET_COLOR;
					control_param = CTRL_COLOR_HUE_0;
					usart0_puts("RC_CHINARGB_RED\n\r");
					break;
				case RC_CHINARGB_RED1:
					control_cmd = CTRL_CMD_SET_COLOR;
					control_param = CTRL_COLOR_HUE_24;
					usart0_puts("RC_CHINARGB_RED1\n\r");
					break;
				case RC_CHINARGB_RED2:
					control_cmd = CTRL_CMD_SET_COLOR;
					control_param = CTRL_COLOR_HUE_48;
					usart0_puts("RC_CHINARGB_RED2\n\r");
					break;
				case RC_CHINARGB_RED3:
					control_cmd = CTRL_CMD_SET_COLOR;
					control_param = CTRL_COLOR_HUE_72;
					usart0_puts("RC_CHINARGB_RED3\n\r");
					break;
				case RC_CHINARGB_RED4:
					control_cmd = CTRL_CMD_SET_COLOR;
					control_param = CTRL_COLOR_HUE_96;
					usart0_puts("RC_CHINARGB_RED4\n\r");
					break;
				case RC_CHINARGB_GREEN:
					control_cmd = CTRL_CMD_SET_COLOR;
					control_param = CTRL_COLOR_HUE_120;
					usart0_puts("RC_CHINARGB_GREEN\n\r");
					break;
				case RC_CHINARGB_GREEN1:
					control_cmd = CTRL_CMD_SET_COLOR;
					control_param = CTRL_COLOR_HUE_144;
					usart0_puts("RC_CHINARGB_GREEN1\n\r");
					break;
				case RC_CHINARGB_GREEN2:
					control_cmd = CTRL_CMD_SET_COLOR;
					control_param = CTRL_COLOR_HUE_168;
					usart0_puts("RC_CHINARGB_GREEN2\n\r");
					break;
				case RC_CHINARGB_GREEN3:
					control_cmd = CTRL_CMD_SET_COLOR;
					control_param = CTRL_COLOR_HUE_192;
					usart0_puts("RC_CHINARGB_GREEN3\n\r");
					break;
				case RC_CHINARGB_GREEN4:
					control_cmd = CTRL_CMD_SET_COLOR;
					control_param = CTRL_COLOR_HUE_222;
					usart0_puts("RC_CHINARGB_GREEN4\n\r");
					break;
				case RC_CHINARGB_BLUE:
					control_cmd = CTRL_CMD_SET_COLOR;
					control_param = CTRL_COLOR_HUE_246;
					usart0_puts("RC_CHINARGB_BLUE\n\r");
					break;
				case RC_CHINARGB_BLUE1:
					control_cmd = CTRL_CMD_SET_COLOR;
					control_param = CTRL_COLOR_HUE_270;
					usart0_puts("RC_CHINARGB_BLUE1\n\r");
					break;
				case RC_CHINARGB_BLUE2:
					control_cmd = CTRL_CMD_SET_COLOR;
					control_param = CTRL_COLOR_HUE_294;
					usart0_puts("RC_CHINARGB_BLUE2\n\r");
					break;
				case RC_CHINARGB_BLUE3:
					control_cmd = CTRL_CMD_SET_COLOR;
					control_param = CTRL_COLOR_HUE_318;
					usart0_puts("RC_CHINARGB_BLUE3\n\r");
					break;
				case RC_CHINARGB_BLUE4:
					control_cmd = CTRL_CMD_SET_COLOR;
					control_param = CTRL_COLOR_HUE_342;
					usart0_puts("RC_CHINARGB_BLUE4\n\r");
					break;
				case RC_CHINARGB_WHITE:
					control_cmd = CTRL_CMD_SET_COLOR;
					control_param = CTRL_COLOR_WHITE;
					break;
				case RC_CHINARGB_SMOOTH:
					control_cmd = CTRL_CMD_RUN_FADING;
					control_param = FADING_MODE_SMOOTH;
					break;
				case RC_CHINARGB_FADE:
					control_cmd = CTRL_CMD_RUN_FADING;
					control_param = FADING_MODE_FADE;
					break;
				case RC_CHINARGB_FLASH:
					control_cmd = CTRL_CMD_RUN_FADING;
					control_param = FADING_MODE_FLASH;
					break;
				case RC_CHINARGB_STROBE:
					control_cmd = CTRL_CMD_RUN_FADING;
					control_param = FADING_MODE_STROBE;
					break;
				case RC_CHINARGB_OFF:
					control_cmd = CTRL_CMD_STANDBY;
					break;
				case RC_CHINARGB_ON:
					control_cmd = CTRL_CMD_POWERON;
					break;
				case RC_CHINARGB_BRIGHTER:
					control_cmd = CTRL_CMD_SPEED_UP;
					break;
				case RC_CHINARGB_DARKER:
					control_cmd = CTRL_CMD_SPEED_DOWN;
					break;
				default:
					usart0_puts("Uknown code\n\r");
				
			}
		} else if ( irmp_data.protocol == IRMP_RC5_PROTOCOL ) {   // RC5 Remote
			switch ( irmp_data.command ) {
				case RC_RC5_STANDBY:
					control_cmd = CTRL_CMD_STANDBY;
					break;
				case RC_RC5_NUM1:
					control_cmd = CTRL_CMD_RUN_FADING;
					control_param = FADING_MODE_SMOOTH;
					break;
				case RC_RC5_NUM2:
					control_cmd = CTRL_CMD_RUN_FADING;
					control_param = FADING_MODE_FADE;
					break;
				case RC_RC5_NUM3:
					control_cmd = CTRL_CMD_RUN_FADING;
					control_param = FADING_MODE_STROBE;
					break;
				case RC_RC5_NUM4:
					control_cmd = CTRL_CMD_RUN_FADING;
					control_param = FADING_MODE_FLASH;
					break;
				case RC_RC5_RED:
					control_cmd = CTRL_CMD_SET_COLOR;
					control_param = CTRL_COLOR_RED;
					break;
				case RC_RC5_GREEN:
					control_cmd = CTRL_CMD_SET_COLOR;
					control_param = CTRL_COLOR_GREEN;
					break;
				case RC_RC5_YELLOW:
					control_cmd = CTRL_CMD_SET_COLOR;
					control_param = CTRL_COLOR_YELLOW;
					break;
				case RC_RC5_BLUE:
					control_cmd = CTRL_CMD_SET_COLOR;
					control_param = CTRL_COLOR_BLUE;
					break;
				case RC_RC5_AV:
					control_cmd = CTRL_CMD_SET_COLOR;
					control_param = CTRL_COLOR_WHITE;
					break;
				case RC_RC5_VOLUP:
					control_cmd = CTRL_CMD_SPEED_UP;
					break;
				case RC_RC5_VOLDOWN:
					control_cmd = CTRL_CMD_SPEED_DOWN;
					break;
			}
		}
	}
}
