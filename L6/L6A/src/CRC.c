/*
 * ECE 153B
 *
 * Name(s): Andy Chen, Karl Tizon
 * Section: Monday 6PM
 * Lab: 6A
 */
 
#include "CRC.h"

/**
  * @brief  Algorithm implementation of the CRC
  * @param  Crc: specifies the previous Crc data
  * @param  Data: specifies the input data
  * @retval Crc: the CRC result of the input data
  */

uint32_t CrcSoftwareFunc(uint32_t Initial_Crc, uint32_t Input_Data, uint32_t POLY)
{
  uint8_t bindex = 0;
  uint32_t Crc = 0;

  /* Initial XOR operation with the previous Crc value */
	Crc = Initial_Crc ^ Input_Data;

  /* The CRC algorithm routine */
  for (bindex = 0; bindex < 32; bindex++){
		if (Crc & 0x80000000U) {
			Crc = (Crc << 1) ^ POLY;
		} else {
			Crc = Crc << 1;
		}
	}
	
  return Crc;
}

