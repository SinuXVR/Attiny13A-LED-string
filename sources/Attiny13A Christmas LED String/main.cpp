/*
 * Attiny13A Christmas LED String.cpp
 *
 * Created: 01.12.2021 19:26:24
 * Author : SinuX
 */ 

#define EEPROM_SIZE 64	// In bytes

#include <avr/io.h>
#include "lib/eeprom.h"
#include "lib/animation.h"

#define MODE_AUTO			0
#define MODE_ALL			1
#define MODE_BLINK_ALL		2
#define MODE_BLINK			3
#define MODE_BLINK_POLICE	4
#define MODE_BLINK_SEQ_ON	5
#define MODE_BLINK_SEQ_OFF	6
#define MODE_FADE_ALL		7
#define MODE_FADE			8
#define MODE_FADE_SEQ_ON	9
#define MODE_FADE_SEQ_OFF	10

#define SPEED_SLOW		0
#define SPEED_NORMAL	1
#define SPEED_FAST		2

#define COUNTER_SLOW	15
#define COUNTER_NORMAL	5
#define COUNTER_FAST	1

#define DEFAULT_MODE	MODE_AUTO
#define DEFAULT_SPEED	SPEED_NORMAL
#define AUTO_MODE_SPEED	SPEED_FAST

#define portinit() DDRB = (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4); PORTB = 0

static inline uint8_t get_next_speed_and_mode(uint8_t speed_and_mode) {
	uint8_t speed = speed_and_mode >> 4;
	uint8_t mode = (speed_and_mode & 0xF) % 11;
	
	if (speed < 2 && mode > 1) {
		return (++speed) << 4 | mode;
	} else {
		return ++mode % 11;
	}
}

int main(void) {
	uint8_t eepos = 0;
	uint8_t saved_speed = DEFAULT_SPEED;
	uint8_t saved_mode = DEFAULT_MODE;
	
	// Get last saved speed and mode from EEPROM
	uint8_t saved_speed_and_mode;
	while (EECR & 2);
	while (((saved_speed_and_mode = eep_read(eepos)) == 0xFF) && (eepos < EEPROM_SIZE - 1)) eepos++;
	
	// Write next speed and mode to EEPROM
	eepos = eep_save(eepos, get_next_speed_and_mode(saved_speed_and_mode));
	if (saved_speed_and_mode != 0xFF) {
		saved_speed = (saved_speed_and_mode >> 4) % 3;
		saved_mode = (saved_speed_and_mode & 0xF) % 11;
	}
	
	const uint8_t div_counter_ovf = saved_mode == MODE_AUTO ? AUTO_MODE_SPEED : saved_speed == SPEED_SLOW ? COUNTER_SLOW : saved_speed == SPEED_FAST ? COUNTER_FAST : COUNTER_NORMAL;
	uint8_t div_counter = 0;
	uint8_t led_counter = 0;
	uint8_t led_counter_cycles = 0;
	uint8_t led_1_value = 0;
	uint8_t led_2_value = 0;
	uint8_t current_mode = saved_mode == MODE_AUTO ? saved_mode + 2 : saved_mode;
	uint8_t mode_saved = 0;
	portinit();
	
    while (1) {
		if (++div_counter > div_counter_ovf) {
			div_counter = 0;
			if (++led_counter == 0) {
				if (++led_counter_cycles % 32 == 0) {
					if (saved_mode == MODE_AUTO) {
						if (++current_mode > 10) current_mode = 2;
					}
					// Lock current speed and mode (after 15-30 seconds)
					if (mode_saved == 0) {
						eep_save(eepos, saved_speed << 4 | saved_mode);
						mode_saved = 1;
					}
				}
			}
			
			// Main routine
			switch (current_mode) {
				case MODE_ALL:
					animate_all(&led_1_value, &led_2_value);
					break;
				case MODE_BLINK_ALL:
					animate_blink_all(led_counter_cycles, &led_1_value, &led_2_value);
					break;
				case MODE_BLINK:
					animate_blink(led_counter_cycles, &led_1_value, &led_2_value);
					break;
				case MODE_BLINK_POLICE:
					animate_blink_police(led_counter_cycles, led_counter, &led_1_value, &led_2_value);
					break;
				case MODE_BLINK_SEQ_ON:
					animate_blink_seq_on(led_counter_cycles, &led_1_value, &led_2_value);
					break;
				case MODE_BLINK_SEQ_OFF:
					animate_blink_seq_off(led_counter_cycles, &led_1_value, &led_2_value);
					break;
				case MODE_FADE_ALL:
					animate_fade_all(led_counter_cycles, led_counter, &led_1_value, &led_2_value);
					break;
				case MODE_FADE:
					animate_fade(led_counter_cycles, led_counter, &led_1_value, &led_2_value);
					break;
				case MODE_FADE_SEQ_ON:
					animate_fade_seq_on(led_counter_cycles, led_counter, &led_1_value, &led_2_value);
					break;
				case MODE_FADE_SEQ_OFF:
					animate_fade_seq_off(led_counter_cycles, led_counter, &led_1_value, &led_2_value);
					break;
			}
		}
		
		charlieplex(led_1_value, led_2_value);
    }
}