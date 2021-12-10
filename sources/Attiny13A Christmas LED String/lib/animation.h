/*
 * animation.h
 *
 * Created: 02.12.2021 20:01:09
 *  Author: SinuX
 */ 


#ifndef ANIMATION_H_
#define ANIMATION_H_

#define LED_MAX_VALUE	255

#define enable_led_1() PORTB = (1 << PB2) | (1 << PB3)
#define enable_led_2() PORTB = (1 << PB1) | (1 << PB4)
#define disable_leds() PORTB = 0

static inline void animate_all(uint8_t* led_1_value, uint8_t* led_2_value) {
	*led_1_value = LED_MAX_VALUE;
	*led_2_value = LED_MAX_VALUE;
}

static inline void animate_blink_all(uint8_t cycles, uint8_t* led_1_value, uint8_t* led_2_value) {
	*led_1_value = cycles % 2 ? 0 : LED_MAX_VALUE;
	*led_2_value = cycles % 2 ? 0 : LED_MAX_VALUE;
}

static inline void animate_fade_all(uint8_t cycles, uint8_t counter, uint8_t* led_1_value, uint8_t* led_2_value) {
	*led_1_value = cycles % 2 ? counter : 255 - counter;
	*led_2_value = cycles % 2 ? counter : 255 - counter;
}

static inline void animate_blink(uint8_t cycles, uint8_t* led_1_value, uint8_t* led_2_value) {
	*led_1_value = cycles % 2 ? LED_MAX_VALUE : 0;
	*led_2_value = cycles % 2 ? 0 : LED_MAX_VALUE;
}

static inline void animate_fade(uint8_t cycles, uint8_t counter, uint8_t* led_1_value, uint8_t* led_2_value) {
	*led_1_value = cycles % 2 ? counter : 255 - counter;
	*led_2_value = cycles % 2 ? 255 - counter : counter;
}

static inline void animate_blink_police(uint8_t cycles, uint8_t counter, uint8_t* led_1_value, uint8_t* led_2_value) {
	*led_1_value = cycles % 2 ? (counter & 32) ? LED_MAX_VALUE : 0 : 0;
	*led_2_value = cycles % 2 ? 0 : (counter & 32) ? LED_MAX_VALUE : 0;
}

static inline void animate_blink_seq_on(uint8_t cycles, uint8_t* led_1_value, uint8_t* led_2_value) {
	switch (cycles % 4) {
		case 0:
			*led_1_value = LED_MAX_VALUE;
			*led_2_value = 0;
			break;
		case 1:
			*led_1_value = LED_MAX_VALUE;
			*led_2_value = LED_MAX_VALUE;
			break;
		case 2:
			*led_1_value = 0;
			*led_2_value = LED_MAX_VALUE;
			break;
		case 3:
			*led_1_value = LED_MAX_VALUE;
			*led_2_value = LED_MAX_VALUE;
			break;
	}
}

static inline void animate_blink_seq_off(uint8_t cycles, uint8_t* led_1_value, uint8_t* led_2_value) {
	switch (cycles % 4) {
		case 0:
			*led_1_value = LED_MAX_VALUE;
			*led_2_value = 0;
			break;
		case 1:
			*led_1_value = 0;
			*led_2_value = 0;
			break;
		case 2:
			*led_1_value = 0;
			*led_2_value = LED_MAX_VALUE;
			break;
		case 3:
			*led_1_value = 0;
			*led_2_value = 0;
			break;
	}
}

static inline void animate_fade_seq_on(uint8_t cycles, uint8_t counter, uint8_t* led_1_value, uint8_t* led_2_value) {
	switch (cycles % 4) {
		case 0:
			*led_1_value = LED_MAX_VALUE;
			*led_2_value = counter;
			break;
		case 1:
			*led_1_value = 255 - counter;
			*led_2_value = LED_MAX_VALUE;
			break;
		case 2:
			*led_1_value = counter;
			*led_2_value = LED_MAX_VALUE;
			break;
		case 3:
			*led_1_value = LED_MAX_VALUE;
			*led_2_value = 255 - counter;
			break;
	}
}

static inline void animate_fade_seq_off(uint8_t cycles, uint8_t counter, uint8_t* led_1_value, uint8_t* led_2_value) {
	switch (cycles % 4) {
		case 0:
			*led_1_value = counter;
			*led_2_value = 0;
			break;
		case 1:
			*led_1_value = 255 - counter;
			*led_2_value = 0;
			break;
		case 2:
			*led_1_value = 0;
			*led_2_value = counter;
			break;
		case 3:
			*led_1_value = 0;
			*led_2_value = 255 - counter;
			break;
	}
}

static inline void charlieplex(uint8_t led_1_value, uint8_t led_2_value) {
	if (led_1_value > 0) enable_led_1();
	for (uint8_t i = 0; i < LED_MAX_VALUE; i++) {
		if (i == led_1_value) disable_leds();
	}
	
	if (led_2_value > 0) enable_led_2();
	for (uint8_t i = 0; i < LED_MAX_VALUE; i++) {
		if (i == led_2_value) disable_leds();
	}
}

#endif /* ANIMATION_H_ */