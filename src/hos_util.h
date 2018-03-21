/*
 *	hos_util.h
 *
 *	Copyright (c) 2017 Picktek LLC
 *
 *	This file is part of Hyperios.
 *
 *	Hyperios is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Hyperios is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with Hyperios.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _HOS_UTIL_H
#define _HOS_UTIL_H

#include <stdio.h>
#include <string.h>
#include <avr/boot.h> 

#include "config.h"


#define _B2048          // boot size is 2048 words (4096 bytes) Don't change !

#define BOOTLOADER_LOCK_VALUE                        253
#define BOOTLOADER_EEPROM_LOAD_VALUE                 250
#define BOOTLOADER_EEPROM_LOAD_COMPLETED_VALUE       249
#define BOOTLOADER_EEPROM_RECOVERY_VALUE             248
#define BOOTLOADER_EEPROM_RECOVERY_COMPLETED_VALUE   247
#define BOOTLOADER_ESP8266_UART_LOAD_VALUE           246

#define BOOTLOADER_PORT                              23

#define BOOTLOADER_EUI64_LENGTH                      8
#define BOOTLOADER_SSID_LENGTH                       20
#define BOOTLOADER_KEY_LENGTH                        16

#define BOOTLOADER_VERSION_MAJOR                    '0'
#define BOOTLOADER_VERSION_MINOR                    '1'

#if !defined(BUTTON_INPUT_PORT) 
  #error "Please define `BUTTON_INPUT_*` directives" 
#endif 

typedef struct _hos_config_perm hos_config_perm;

struct _hos_config_perm
{
    uint8_t  mcusr;
    uint8_t  number_of_boot;
    uint8_t  flash_lock;
    
    uint16_t  eeprom_page_count;
    uint16_t  eeprom_page_offset;

    uint16_t  eeprom_recovery_page_count;
    uint16_t  eeprom_recovery_page_offset;

#if defined(ALLOW_WIFI)
    uint8_t   eui64[BOOTLOADER_EUI64_LENGTH];
    uint8_t   ssid[BOOTLOADER_SSID_LENGTH];
    uint8_t   key[BOOTLOADER_KEY_LENGTH];
#endif

} __attribute__ ((__packed__));


#if defined(LED_OUTPUT_PORT) 
  #define LED_ON()  LED_OUTPUT_PORT &= ~(_BV(LED_OUTPUT_PIN))
  #define LED_OFF() LED_OUTPUT_PORT |= (_BV(LED_OUTPUT_PIN))
  #define LED_ION() !((LED_OUTPUT_PORT & (_BV(LED_OUTPUT_PIN))) > 0)
#else 
  #define LED_ON()
  #define LED_OFF()
  #define LED_ION()
#endif

/**
 * IO register bit manipulation
 */
#define HosIoBitIsSet(sfr, bit)         (_SFR_BYTE(sfr) & _BV(bit))
#define HosIoBitIsClear(sfr, bit)       (!(_SFR_BYTE(sfr) & _BV(bit)))
#define HosIoBitSet(port, bit)          (port) |= (1 << (bit))
#define HosIoBitClear(port, bit)        (port) &= ~(1 << (bit))


void HosSerialInit(void);
void HosSerialTX(uint8_t b);
uint8_t HosSerialRX(void);
void HosLedBlink(uint8_t count);
void HosRebootCPU();

void HosEspEraseProgramSpace(void);
uint16_t HosEspWriteProgramPage(uint8_t *buffer, uint16_t address);
uint16_t HosUdsCRC16(uint8_t *data, uint16_t length, uint16_t crc);
void hexDump (char *desc, void *addr, int len);


// TODO: make use of RAMEND in the avr-libc io-files and 
// avoid a lot of by-device definitions here 

#if defined(__AVR_ATmega169__) 
  #define sig_byte3 0x1E 
  #define sig_byte2 0x94 
  #define sig_byte1 0x05 
  
  #define devtype 0x79       // Mega 169 device code 
  
//  #define PAGESIZE 128  // 2*64 Words = Size in Bytes 
  
  #ifdef _B128 
    #define APP_PAGES ((2*8192 / SPM_PAGESIZE)- (2*128 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif 
  #ifdef _B256 
    #define APP_PAGES ((2*8192 / SPM_PAGESIZE)- (2*256 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif 
  #ifdef _B512 
    #define APP_PAGES ((2*8192 / SPM_PAGESIZE)- (2*512 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif 
  #ifdef _B1024 
    #define APP_PAGES ((2*8192 / SPM_PAGESIZE)- (2*1024 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif 
  #ifdef _B2048 
    #error "_B2048 not suppoted on this device" 
  #endif    

#elif defined(__AVR_ATmega16__) 

  #define sig_byte3 0x1E 
  #define sig_byte2 0x94 
  #define sig_byte1 0x03 
  
  #define devtype 0x75       // Mega16 device code 
  
//  #define PAGESIZE 128       // Size in Bytes 
  
  #ifdef _B128 
    #define APP_PAGES ((2*8192 / SPM_PAGESIZE)- (2*128 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif 
  #ifdef _B256 
    #define APP_PAGES ((2*8192 / SPM_PAGESIZE)- (2*256 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif 
  #ifdef _B512 
    #define APP_PAGES ((2*8192 / SPM_PAGESIZE)- (2*512 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif 
   #ifdef _B1024 
    #define APP_PAGES ((2*8192 / SPM_PAGESIZE)- (2*1024 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif  
  #ifdef _B2048 
    #error "_B2048 not suppoted on this device" 
  #endif    
  
#elif defined(__AVR_ATmega328P__) 

 // #define sig_byte3 0x1E 
 // #define sig_byte2 0x94 
 // #define sig_byte1 0x03 
  
  //#define devtype 0x75       // Mega16 device code 
  
//  #define PAGESIZE 128       // Size in Bytes 

  #ifdef _B128 
    #define APP_PAGES ((2*16384UL / SPM_PAGESIZE)- (2*128 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif 
  #ifdef _B256 
    #define APP_PAGES ((2*16384UL / SPM_PAGESIZE)- (2*256 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif 
  #ifdef _B512 
    #define APP_PAGES ((2*16384UL / SPM_PAGESIZE)- (2*512 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif 
   #ifdef _B1024 
    #define APP_PAGES ((2*16384UL / SPM_PAGESIZE)- (2*1024 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif  
  #ifdef _B2048 
    #define APP_PAGES ((2*16384UL / SPM_PAGESIZE)- (2*2048 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif  

  
#elif defined(__AVR_ATmega168__) 

  #define sig_byte3 0x1E 
  #define sig_byte2 0x94 
  #define sig_byte1 0x03 
  
  #define devtype 0x75       // Mega16 device code 
  
//  #define PAGESIZE 128       // Size in Bytes 
  
  #ifdef _B128 
    #define APP_PAGES ((2*8192 / SPM_PAGESIZE)- (2*128 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif 
  #ifdef _B256 
    #define APP_PAGES ((2*8192 / SPM_PAGESIZE)- (2*256 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif 
  #ifdef _B512 
    #define APP_PAGES ((2*8192 / SPM_PAGESIZE)- (2*512 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif 
   #ifdef _B1024 
    #define APP_PAGES ((2*8192 / SPM_PAGESIZE)- (2*1024 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif  
  #ifdef _B2048 
    #error "_B2048 not suppoted on this device" 
  #endif    

#elif defined(__AVR_ATmega8__) 

  #define sig_byte3 0x1E 
  #define sig_byte2 0x93 
  #define sig_byte1 0x07 
  
  #define devtype 0x77       // Mega8 boot device code 
  
//  #define PAGESIZE 64        // Size in Bytes 
  
  #ifdef _B128 
    #define APP_PAGES ((2*4096 / SPM_PAGESIZE)- (2*128 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif 
  #ifdef _B256 
    #define APP_PAGES ((2*4096 / SPM_PAGESIZE)- (2*256 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif 
  #ifdef _B512 
    #define APP_PAGES ((2*4096 / SPM_PAGESIZE)- (2*512 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif 
   #ifdef _B1024 
    #define APP_PAGES ((2*4096 / SPM_PAGESIZE)- (2*1024 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif  
  #ifdef _B2048 
    #error "_B2048 not suppoted on this device" 
  #endif    
#elif defined(__AVR_ATmega162__) 

  #define sig_byte3 0x1E 
  #define sig_byte2 0x94 
  #define sig_byte1 0x04 
  
// #define devtype 0x??       // Mega162 boot device code 
  
//  #define PAGESIZE 128        // Size in Bytes 
  
  #ifdef _B128 
    #define APP_PAGES ((2*8192 / SPM_PAGESIZE)- (2*128 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif 
  #ifdef _B256 
    #define APP_PAGES ((2*8192 / SPM_PAGESIZE)- (2*256 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif 
  #ifdef _B512 
    #define APP_PAGES ((2*8192 / SPM_PAGESIZE)- (2*512 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif 
   #ifdef _B1024 
    #define APP_PAGES ((2*8192 / SPM_PAGESIZE)- (2*1024 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif  
  #ifdef _B2048 
    #error "_B2048 not suppoted on this device" 
  #endif    

#elif defined(__AVR_ATmega32__) 

  #define sig_byte3 0x1E 
  #define sig_byte2 0x95 
  #define sig_byte1 0x02 
  
  #define devtype 0x73       // Mega32 device code 
  
//  #define PAGESIZE 128       // Size in Bytes 
  
  #ifdef _B128 
    #define APP_PAGES ((2*16384UL / SPM_PAGESIZE)- (2*128 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif 
  #ifdef _B256 
    #define APP_PAGES ((2*16384UL / SPM_PAGESIZE)- (2*256 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif 
  #ifdef _B512 
    #define APP_PAGES ((2*16384UL / SPM_PAGESIZE)- (2*512 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif 
   #ifdef _B1024 
    #define APP_PAGES ((2*16384UL / SPM_PAGESIZE)- (2*1024 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif  
  #ifdef _B2048 
    #define APP_PAGES ((2*16384UL / SPM_PAGESIZE)- (2*2048 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif  

#elif defined(__AVR_ATmega128__) 

  #define sig_byte3 0x1E 
  #define sig_byte2 0x97 
  #define sig_byte1 0x02 
  
  #define devtype 0x44      // Mega128 device code 
  
//  #define PAGESIZE 128       // Size in Bytes 
  
  #ifdef _B512 
    #define APP_PAGES ((2*65536UL / SPM_PAGESIZE)- (2*512 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif 
  #ifdef _B1024 
    #define APP_PAGES ((2*65536UL / SPM_PAGESIZE)- (2*1024 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif  
  #ifdef _B2048 
    #define APP_PAGES ((2*65536UL / SPM_PAGESIZE)- (2*2048 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif 
  #ifdef _B4096 
    #define APP_PAGES ((2*65536UL / SPM_PAGESIZE)- (2*4096 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif  

#elif defined(__AVR_ATmega1284P__) 

/*
  #define sig_byte3 0x1E 
  #define sig_byte2 0x97 
  #define sig_byte1 0x02 
  
  #define devtype 0x44      // Mega128 device code 
*/
  
//  #define PAGESIZE 128       // Size in Bytes 
  
  #ifdef _B512 
    #define APP_PAGES ((2*65536UL / SPM_PAGESIZE)- (2*512 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif 
  #ifdef _B1024 
    #define APP_PAGES ((2*65536UL / SPM_PAGESIZE)- (2*1024 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif  
  #ifdef _B2048 
    #define APP_PAGES ((2*65536UL / SPM_PAGESIZE)- (2*2048 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif 
  #ifdef _B4096 
    #define APP_PAGES ((2*65536UL / SPM_PAGESIZE)- (2*4096 / SPM_PAGESIZE )) 
    #define APP_END APP_PAGES * SPM_PAGESIZE 
  #endif  

#else 
   #error "no definition for MCU available in chipdef.h" 
#endif 

#ifdef __AVR_ATmega8__
#define myIVSELREG GICR
#define myUSART_ReceiveCompleteVect USART_RXC_vect
#define myRXCIE RXCIE
#define myUDR   UDR
#define myUBRRH UBRRH
#define myUBRRL UBRRL
#define myUCSRA UCSRA
#define myUCSRB UCSRB
#define myUCSRC UCSRC
#define myUDRE  UDRE
#define myRXC   RXC
#define myRXEN  RXEN
#define myTXEN  TXEN
#define myUCSZ0 UCSZ0
#define myUCSZ1 UCSZ1
#define myURSEL _BV(URSEL) // bit must be set to one on certain MCU types
#define myCOUNTERTYPE uint8_t
#endif

#ifdef __AVR_ATmega328P__
#define myIVSELREG GICR
#define myUSART_ReceiveCompleteVect USART_RX_vect
#define myRXCIE RXCIE0
#define myUDR   UDR0
#define myUBRRH UBRR0H
#define myUBRRL UBRR0L
#define myUCSRA UCSR0A
#define myUCSRB UCSR0B
#define myUCSRC UCSR0C
#define myUDRE  UDRE0
#define myRXC   RXC0
#define myRXEN  RXEN0
#define myTXEN  TXEN0
#define myUCSZ0 UCSZ00
#define myUCSZ1 UCSZ01
#define myURSEL  0 // bit must be set to one on certain MCU types
#define myCOUNTERTYPE uint8_t
#endif

#ifdef __AVR_ATmega32__
#define myIVSELREG GICR
#define myUSART_ReceiveCompleteVect USART_RXC_vect
#define myRXCIE RXCIE
#define myUDR   UDR
#define myUBRRH UBRRH
#define myUBRRL UBRRL
#define myUCSRA UCSRA
#define myUCSRB UCSRB
#define myUCSRC UCSRC
#define myUDRE  UDRE
#define myRXC   RXC
#define myRXEN  RXEN
#define myTXEN  TXEN
#define myUCSZ0 UCSZ0
#define myUCSZ1 UCSZ1
#define myURSEL _BV(URSEL) // bit must be set to one on certain MCU types
#define myCOUNTERTYPE uint8_t
#endif

#ifdef __AVR_ATmega1284P__
#define myIVSELREG GICR
#define myUSART_ReceiveCompleteVect USART0_RX_vect
#define myRXCIE RXCIE0
#define myUDR   UDR0
#define myUBRRH UBRR0H
#define myUBRRL UBRR0L
#define myUCSRA UCSR0A
#define myUCSRB UCSR0B
#define myUCSRC UCSR0C
#define myUDRE  UDRE0
#define myRXC   RXC0
#define myRXEN  RXEN0
#define myTXEN  TXEN0
#define myUCSZ0 UCSZ00
#define myUCSZ1 UCSZ01
#define myURSEL 0 // bit must be set to one on certain MCU types
#define myCOUNTERTYPE uint8_t
#endif


#endif
