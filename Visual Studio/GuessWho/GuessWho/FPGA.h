#pragma once
#include "CommunicationFPGA.h"
#include <windows.h>

#define STATUS_LOCAL 0
#define STATUS_CONNECTED 1
#define READINGS_PER_BURST 10

class FPGA
{

private:
	CommunicationFPGA fpgaCard;
	int cardMode; //Local or connected
	bool cardStatus;
	int readingDelay; //Delay between each reading in a burst to average the 4 channels (~10ms)
	int burstDelay; //Delay between each burst (minimum time between 2 phonemes)
	int rawData[READINGS_PER_BURST][4];
	int phonemeThreshold[4][4][2]; //Threshold for each phoneme x[phoneme][channel][min/max]
	bool phonemeDetected;
	void printRead();

	// numeros de registres correspondants pour les echanges FPGA <-> PC  ...
	unsigned const int nreg_lect_stat_btn = 0;  // fpga -> PC  Statut et BTN lus FPGA -> PC
	unsigned const int nreg_lect_swt = 1;       // fpga -> PC  SWT lus FPGA -> PC
	unsigned const int nreg_lect_cmpt_t = 2;    // fpga -> PC  compteur temps FPGA -> PC 
	unsigned const int nreg_lect_can0 = 3;      // fpga -> PC  canal 0 lus FPGA -> PC
	unsigned const int nreg_lect_can1 = 4;      // fpga -> PC  canal 1 lus FPGA -> PC
	unsigned const int nreg_lect_can2 = 5;      // fpga -> PC  canal 2 lus FPGA -> PC
	unsigned const int nreg_lect_can3 = 6;      // fpga -> PC  canal 3 lus FPGA -> PC
	unsigned const int nreg_ecri_aff7sg0 = 7;   // PC -> fpga (octet 0  aff.7 seg.)
	unsigned const int nreg_ecri_aff7sg1 = 8;   // PC -> fpga (octet 1  aff.7 seg.)
	unsigned const int nreg_ecri_aff7dot = 9;   // PC -> fpga (donnees dot-points)
	unsigned const int nreg_ecri_led = 10;      // PC -> fpga (donnees leds)

public:
	bool switchToConnected();
	bool readData();
	int convertDataToPhoneme();
	void readLoop();

	FPGA(int delay=500);
	~FPGA();
};
