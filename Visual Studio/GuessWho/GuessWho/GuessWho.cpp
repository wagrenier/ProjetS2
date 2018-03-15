// GuessWho.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Games.h"
#include "Demo.h"
#include "FPGA.h"

int main()
{
	
	FPGA test;
	test.readLoop();

	Games game;
	game.start();
	
	//Demo acts as a debug menu
	//Demo demo;
	//demo.start();

	
    return 0;
}

