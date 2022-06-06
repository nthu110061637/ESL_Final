#ifndef _DEFINES_H_
#define _DEFINES_H_

#define CLOCK_PERIOD 5
const int NumData = 20;
const int condition = 4;
const int TreeSize = condition * condition -1;
const int DMA_TRANS = 64;
union word {
  int sint;
  unsigned int uint;
  unsigned char uc[4];
};
// Used between blocking_transport() & do_filter()
const int RANDOM_FOREST_R_ADDR = 0x00000000;
const int RANDOM_FOREST_RESULT_ADDR = 0x00000004;

const int RANDOM_FOREST_RS_R_ADDR   = 0x00000000;
const int RANDOM_FOREST_RS_W_WIDTH  = 0x00000004;
const int RANDOM_FOREST_RS_W_HEIGHT = 0x00000008;
const int RANDOM_FOREST_RS_W_DATA   = 0x0000000C;
const int RANDOM_FOREST_RS_RESULT_ADDR = 0x00800000;

#endif
