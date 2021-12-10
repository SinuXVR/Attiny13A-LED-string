/*
 * eeprom.h
 *
 * Created: 02.12.2021 20:24:18
 *  Author: SinuX
 */ 

#ifndef EEPROM_H_
#define EEPROM_H_

static inline uint8_t eep_read(uint8_t addr) {
	EEARL = addr; EECR = 1;
	return EEDR;
}

/* Save current mode to EEPROM with wear leveling */
static inline uint8_t eep_save(uint8_t address, uint8_t speed_and_mode) {
	uint8_t old_address = address;
	address = (uint8_t)(address + 1) % EEPROM_SIZE;
	
	while (EECR & 2); // Wait for completion
	EEARL = address; EEDR = speed_and_mode; EECR = 32 + 4; EECR = 32 + 4 + 2; // 32:write only (no erase) 4:enable  2:go
	while (EECR & 2); // Wait for completion
	EEARL = old_address; EECR = 16 + 4; EECR = 16 + 4 + 2; // 16:erase only (no write) 4:enable  2:go
	
	return address;
}

#endif /* EEPROM_H_ */