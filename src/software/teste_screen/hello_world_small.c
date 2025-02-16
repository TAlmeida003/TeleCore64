

#include "sys/alt_stdio.h"
#include "lib_graphic_processor/graphic_processor.h"
#include "lib_JOYSTICK/joystick_io.h"
#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "stdio.h"

#define YMAX 98


void clear_block(){
	volatile int *fifo_full = WRFULL_BASE;

	for (int i = 0; i < 60; i++) {
		for (int j = 0; j < 80; j++) {
			while (*fifo_full){}
			set_background_block(j , i, 0, 0, 0);
		}
	}
}

void baseBeckgraud(){
	volatile int *fifo_full = WRFULL_BASE;

	for (int i = 5; i < 55; i++) {
		for (int j = 20; j < 70; j++) {
			while (*fifo_full){}
			set_background_block(j , i, 1, 1, 1);
		}
	}


}

void setbase(int *intrucoes1, int *intrucoes2, int *intrucoes3, int *intrucoes4){
	for (int j = 0; j < 50; j++){
		intrucoes1[j] = 0x010101;
		intrucoes2[j] = 0x010101;
		intrucoes3[j] = 0x010101;
		intrucoes4[j] = 0x010101;
	}
}

void printLaterais(int *instrucoesLeft, int *instrucoesRight, int *instrucoesUp, int *instrucoesDown){
	for (int i = 0; i < 2; i++){
		for (int j = 0; j < 50; j++){
			while (IORD(WRFULL_BASE,0) == 1){}
			set_background_block(20+j , i + 53,
								instrucoesDown[j] & 0xFF, instrucoesDown[j] >> 8 & 0xFF,  instrucoesDown[j] >> 16 & 0xFF);
			set_background_block(i + 20 , j+5,
					instrucoesLeft[j] & 0xFF, instrucoesLeft[j] >> 8 & 0xFF,  instrucoesLeft[j] >> 16 & 0xFF);
			set_background_block(20 + j , i+5,
								instrucoesUp[j] & 0xFF, instrucoesUp[j] >> 8 & 0xFF,  instrucoesUp[j] >> 16 & 0xFF);
			set_background_block(i + 68 , j+5,
					instrucoesRight[j] & 0xFF, instrucoesRight[j] >> 8 & 0xFF,  instrucoesRight[j] >> 16 & 0xFF);
		}
	}
}


void scroolLombadaVerticalLeft(int *instrucoesLeft, int  x, int y){
	if ((y <= 98 && x >= 26) || x < - 96){
		for (int i = 0; i < 50; i += 2) {
			if ((!(y > 50 && i < y - 50 && x > -96) && y + 50 + 49 > i) || x < - 96){
				if ((i % 4 == 0 && y % 4 == 0) || (i % 4 != 0 && y % 4 != 0)){
					instrucoesLeft[i] = 5;
					instrucoesLeft[i+1] = 5;
				} else {
					instrucoesLeft[i] = 0x070707;
				    instrucoesLeft[i + 1] = 0x070707;
				}
			}
		}
	}
}


/*
 *
 */

void scroolLombadaVerticalRight(int *instrucoesRight, int  x, int y){
	if ((x >= 26 && y <= 98) || x < -96){
		for (int i = 0; i < 50; i += 2) {
			if ((y - 98 + 48 <= i && x < -96 && y + 100 > i) || x >= 26){
	    		if ((i % 4 == 0 && y % 4 == 0) || (i % 4 != 0 && y % 4 != 0)){  // Altera��o para que dois elementos seguidos recebam o mesmo valor
	        		instrucoesRight[i] = 5;
	        		instrucoesRight[i+1] = 5;
	        	} else {
	        		instrucoesRight[i] = 0x070707;
	        		instrucoesRight[i + 1] = 0x070707;
	        	}
	    	}
		}
	}
}


void scroolLombadaHorizontalUp(int *screen, int xcar, int ycar) {
    if (ycar >= 98 || ycar <= -98) {
        for (int i = 0; i < 50; i += 2) {
			if ((i < -xcar + 27 && !(xcar < -50 && i < -xcar - 50) ) || ycar >= 98){
        		if ((i % 4 == 0 && xcar % 4 == 0) || (i % 4 != 0 && xcar % 4 != 0)){
        			screen[i] = 5;
       				screen[i+1] = 5;
       			} else {
       				screen[i] = 0x070707;
       				screen[i + 1] = 0x070707;
       			}
        	}
    	}
	}
}

void scroolLombadaHorizontalDown(int *screen, int xcar, int ycar) {
	if (ycar >= 98 || ycar <= -98) {
	        for (int i = 0; i < 50; i += 2) {
				if ((i < -xcar + 27 && !(xcar < -50 && i < -xcar - 50)) || ycar == -98){
	        		if ((i % 4 == 0 && xcar % 4 == 0) || (i % 4 != 0 && xcar % 4 != 0)){
	        			screen[i] = 5;
	       				screen[i+1] = 5;
	       			} else {
	       				screen[i] = 0x070707;
	       				screen[i + 1] = 0x070707;
	       			}
	        	}
	    	}
		}
}

void printCar(){
	volatile int *fifo_full = WRFULL_BASE;

	for (int i = 30; i < 35; i++) {
		for (int j = 42; j < 47; j++) {
			while (*fifo_full){}
			set_background_block(j , i, 1, 0, 0);
		}
	}


}

int main()
{
	initialize_joystick();
	clear_block();
	baseBeckgraud();
	printCar();

	int instrucoesLeft[50] = {};
	int instrucoesRight[50] = {};
	int instrucoesUp[50] = {};
	int instrucoesDown[50] = {};

	int y = 0;
	int x = 26;
	int telas = 0;



	while (1){
		setbase(instrucoesLeft, instrucoesRight, instrucoesUp, instrucoesDown);

		scroolLombadaVerticalLeft(instrucoesLeft,x, y);
		scroolLombadaVerticalRight(instrucoesRight,x, y);
		scroolLombadaHorizontalUp(instrucoesUp, x, y);
		scroolLombadaHorizontalDown(instrucoesDown, x, y);

		if (instrucoesLeft[0] != 0x010101){
			instrucoesUp[0] = instrucoesLeft[0];
			instrucoesUp[1] = instrucoesLeft[1];
		} else if (instrucoesUp[0] != 0x010101){
			instrucoesLeft[0] = instrucoesUp[0];
			instrucoesLeft[1] = instrucoesUp[1];
		}

		if (instrucoesRight[0] != 0x010101){
			instrucoesUp[48] = instrucoesRight[0];
			instrucoesUp[49] = instrucoesRight[1];
		} else if (instrucoesUp[48] != 0x010101){
			instrucoesRight[0] = instrucoesUp[48];
			instrucoesRight[1] = instrucoesUp[49];
		}

		if (instrucoesLeft[48] != 0x010101){
			instrucoesDown[0] = instrucoesLeft[48];
			instrucoesDown[1] = instrucoesLeft[49];
		} else if (instrucoesDown[0] != 0x010101){
			instrucoesLeft[48] = instrucoesDown[0];
			instrucoesLeft[49] = instrucoesDown[1];
		}
		if (instrucoesRight[48] != 0x010101){
			instrucoesDown[48] = instrucoesRight[48];
			instrucoesDown[49] = instrucoesRight[49];
		} else if (instrucoesDown[48] != 0x010101){
			instrucoesRight[48] = instrucoesDown[48];
			instrucoesRight[49] = instrucoesDown[49];
		}



		printLaterais(instrucoesLeft, instrucoesRight, instrucoesUp, instrucoesDown);

			if (read_JOYSTICK(UP_DIR, LEVEL) &&  y < 98){
				y +=2;
			} else if (read_JOYSTICK(DOWN_DIR, LEVEL)&& y > -98){
				y-=2;
			}

			if (read_JOYSTICK(RIGHT_DIR, LEVEL) && x < 26){
				x +=2;
			} else if (read_JOYSTICK(LEFT_DIR, LEVEL) && x >= -96){
				x-=2;
			}
	}
    return 0;
}
