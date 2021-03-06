#include <avr/io.h>
#include <avr/eeprom.h>
#include "config.h"
#include "control.h"
#include "pwm.h"
#include "usart.h"
#include "fadingengine.h"
#include <stdio.h>

#ifdef USART_DEBUG
#include "usart.h"
#endif

uint8_t current_mode EEMEM = CTRL_CMD_NONE;
uint8_t current_mode_param EEMEM = 0;

void control_init(void) {
	uint8_t temp = CTRL_CMD_NONE;
	temp = eeprom_read_byte(&current_mode);
	switch (temp) {
		case CTRL_CMD_SET_COLOR:
			control_cmd = CTRL_CMD_SET_COLOR;
			control_param = eeprom_read_byte(&current_mode_param);
			break;
		case CTRL_CMD_RUN_FADING:
			control_cmd = CTRL_CMD_RUN_FADING;
			control_param = eeprom_read_byte(&current_mode_param);
			break;
		default:
			control_cmd = CTRL_CMD_RUN_FADING;
			control_param = FADING_MODE_SMOOTH;
			fe_speed = 512;
	}
	
}

void control_handler(void) {

	if (control_cmd != CTRL_CMD_NONE) {

#ifdef USART_DEBUG
		usart0_puts("INCMD\n\r");
#endif

		switch (control_cmd) {
			case CTRL_CMD_STANDBY:
	#ifdef USART_DEBUG
			usart0_puts("CTRL_CMD_STANDBY\n\r");
	#endif
					control_setColorRGB(0x00, 0x00, 0x00);
					fe_mode = FADING_MODE_DISABLED;
				break;
			case CTRL_CMD_POWERON:
	#ifdef USART_DEBUG
			usart0_puts("CTRL_CMD_POWERON\n\r");
	#endif
					control_init();
					control_handler();

				break;
			case CTRL_CMD_SET_COLOR:
#ifdef USART_DEBUG
		usart0_puts("CTRL_CMD_SET_COLOR\n\r");
#endif
				control_setColor(control_param);
				eeprom_write_byte(&current_mode, CTRL_CMD_SET_COLOR);
				eeprom_write_byte(&current_mode_param, control_param);
				break;
			case CTRL_CMD_RUN_FADING:
#ifdef USART_DEBUG
		usart0_puts("CTRL_CMD_RUN_FADING\n\r");
#endif
				if (fe_mode != control_param) {
					fe_mode = control_param;
					fe_start();
					eeprom_write_byte(&current_mode, CTRL_CMD_RUN_FADING);
					eeprom_write_byte(&current_mode_param, control_param);
				}
				break;	
/*			case CTRL_CMD_PAUSE_TOGGLE:
				global.flags.paused = (1 - global.flags.paused);
				break;*/
			case CTRL_CMD_SPEED_UP:
#ifdef USART_DEBUG
		usart0_puts("CTRL_CMD_SPEED_UP\n\r");
#endif
				if (fe_speed < 0xf00)
					fe_speed = fe_speed * 2;

#ifdef USART_DEBUG
		sprintf(msgbuf, "SPEED IS NOW %d\n\r", fe_speed);
		usart0_puts(msgbuf);
#endif

				break;
			case CTRL_CMD_SPEED_DOWN:
#ifdef USART_DEBUG
		usart0_puts("CTRL_CMD_SPEED_DOWN\n\r");
#endif
				if (fe_speed > 0x10)
					fe_speed = fe_speed / 2;

#ifdef USART_DEBUG
		sprintf(msgbuf, "SPEED IS NOW %d\n\r", fe_speed);
		usart0_puts(msgbuf);
#endif
				break;
/*			case CTRL_CMD_PAUSE_ON:
				global.flags.paused = 1;
				break;
			case CTRL_CMD_PAUSE_OFF:
				global.flags.paused = 1;
				break;*/
		}
		
		control_cmd = CTRL_CMD_NONE;
	}
};

void control_setColor(uint8_t color) {
	if (color == CTRL_COLOR_RED) 
		control_setColorRGB(0xff, 0x00, 0x00);
	else if (color == CTRL_COLOR_GREEN)
		control_setColorRGB(0x00, 0xff, 0x00);
	else if (color == CTRL_COLOR_BLUE)
		control_setColorRGB(0x00, 0x00, 0xff);
	else if (color == CTRL_COLOR_YELLOW)
		control_setColorRGB(0xff,0xff,0x00);
	else if (color == CTRL_COLOR_VIOLET)
		control_setColorRGB(0xff,0x00,0xff);
	else if (color == CTRL_COLOR_WHITE)
		control_setColorRGB(0xff,0xff,0xff);
	else if (color == CTRL_COLOR_ORANGE)
		control_setColorRGB(0xff,0x7f,0x00);
	else if (color == CTRL_COLOR_LIGHTGREEN)
		control_setColorRGB(0x7f,0xff,0x7f);
	else if (color == CTRL_COLOR_LIGHTBLUE)
		control_setColorRGB(0x7f,0x7f,0xff);
	else if (color == CTRL_COLOR_HUE_0)
		control_setColorRGB(0xff,0x00,0x00);
	else if (color == CTRL_COLOR_HUE_24)
		control_setColorRGB(0xff,0x66,0x00);
	else if (color == CTRL_COLOR_HUE_48)
		control_setColorRGB(0xff,0xcc,0x00);
	else if (color == CTRL_COLOR_HUE_72)
		control_setColorRGB(0xcc,0xff,0x00);
	else if (color == CTRL_COLOR_HUE_96)
		control_setColorRGB(0x66,0xff,0x00);
	else if (color == CTRL_COLOR_HUE_120)
		control_setColorRGB(0x00,0xff,0x00);
	else if (color == CTRL_COLOR_HUE_144)
		control_setColorRGB(0x00,0xff,0x66);
	else if (color == CTRL_COLOR_HUE_168)
		control_setColorRGB(0x00,0xff,0xcc);
	else if (color == CTRL_COLOR_HUE_192)
		control_setColorRGB(0x00,0xcc,0xff);
	else if (color == CTRL_COLOR_HUE_222)
		control_setColorRGB(0x00,0x4c,0xff);
	else if (color == CTRL_COLOR_HUE_246)
		control_setColorRGB(0x00,0x00,0xff);
	else if (color == CTRL_COLOR_HUE_270)
		control_setColorRGB(0x80,0x00,0xff);
	else if (color == CTRL_COLOR_HUE_294)
		control_setColorRGB(0xe6,0x00,0xff);
	else if (color == CTRL_COLOR_HUE_318)
		control_setColorRGB(0xff,0x00,0xb3);
	else if (color == CTRL_COLOR_HUE_342)
		control_setColorRGB(0xff,0x04,0x4d);
	fe_mode = FADING_MODE_DISABLED;
}

void control_setColorRGB(uint8_t red, uint8_t green, uint8_t blue) {
//	script_threads[0].flags.disabled = 1;	   
    global_pwm.channels[CHANNEL_RED].flags.target_reached = 0;
	global_pwm.channels[CHANNEL_RED].speed = 0x600;
	global_pwm.channels[CHANNEL_RED].target_brightness = red;
	
	global_pwm.channels[CHANNEL_GREEN].flags.target_reached = 0;
	global_pwm.channels[CHANNEL_GREEN].speed = 0x600;
	global_pwm.channels[CHANNEL_GREEN].target_brightness = green;
	
	global_pwm.channels[CHANNEL_BLUE].flags.target_reached = 0;
	global_pwm.channels[CHANNEL_BLUE].speed = 0x600;
	global_pwm.channels[CHANNEL_BLUE].target_brightness = blue;
}
