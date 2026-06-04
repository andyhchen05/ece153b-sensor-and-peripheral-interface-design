#include "SPI.h"
#include "SysTimer.h"
#include "accelerometer.h"

void accWrite(uint8_t addr, uint8_t val){
	uint8_t cmd = 0x00 | (addr & 0x3F);
	uint16_t packet = ((uint16_t)cmd << 8) | val;
	SPI_Transfer_Data(packet);
}

uint8_t accRead(uint8_t addr){
	uint8_t cmd = 0x80 | (addr & 0x3F);
	uint16_t packet = ((uint16_t)cmd << 8) | 0x00;
	uint16_t result = SPI_Transfer_Data(packet);
	return (uint8_t)(result & 0xFF);
}

void initAcc(void){
	// set full range mode
	accWrite(0x31, 0x08);
	// enable measurement
	accWrite(0x2D, 0x08);
}

void readValues(double* x, double* y, double* z){
	// find scaler from data sheet
	const double scale = 0.0039;
	// read values into x,y,z using accRead
	int16_t rawX = (int16_t)(accRead(0x33) << 8 | accRead(0x32));
	int16_t rawY = (int16_t)(accRead(0x35) << 8 | accRead(0x34));
	int16_t rawZ = (int16_t)(accRead(0x37) << 8 | accRead(0x36));
	
	*x = rawX * scale;
	*y = rawY * scale;
	*z = rawZ * scale;
}
