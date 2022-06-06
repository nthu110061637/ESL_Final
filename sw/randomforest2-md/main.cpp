#include "string"
#include "string.h"
#include "cassert"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

using namespace std;
const int NumData = 20;
const int condition = 4;
const int TreeSize = condition * condition -1;
union word {
  int sint;
  unsigned int uint;
  unsigned char uc[4];
};
word data0;
word data1;
int Result [NumData][4];
int TestData[NumData][condition];
int num0 = 0;
int num1 = 0;

FILE *infp;
FILE *outfp;
// Gassian Filter ACC
static char* const RandomForest0_START_ADDR = reinterpret_cast<char* const>(0x73000000);
static char* const RandomForest0_READ_ADDR  = reinterpret_cast<char* const>(0x73000004);
static char* const RandomForest1_START_ADDR = reinterpret_cast<char* const>(0x74000000);
static char* const RandomForest1_READ_ADDR  = reinterpret_cast<char* const>(0x74000004);
// DMA 
static volatile uint32_t * const DMA0_SRC_ADDR  = (uint32_t * const)0x70000000;
static volatile uint32_t * const DMA0_DST_ADDR  = (uint32_t * const)0x70000004;
static volatile uint32_t * const DMA0_LEN_ADDR  = (uint32_t * const)0x70000008;
static volatile uint32_t * const DMA0_OP_ADDR   = (uint32_t * const)0x7000000C;
static volatile uint32_t * const DMA0_STAT_ADDR = (uint32_t * const)0x70000010;
static const uint32_t DMA0_OP_MEMCPY = 1;
static volatile uint32_t * const DMA1_SRC_ADDR  = (uint32_t * const)0x75000000;
static volatile uint32_t * const DMA1_DST_ADDR  = (uint32_t * const)0x75000004;
static volatile uint32_t * const DMA1_LEN_ADDR  = (uint32_t * const)0x75000008;
static volatile uint32_t * const DMA1_OP_ADDR   = (uint32_t * const)0x7500000C;
static volatile uint32_t * const DMA1_STAT_ADDR = (uint32_t * const)0x75000010;
static const uint32_t DMA1_OP_MEMCPY = 1;


bool _is_using_dma = true;
//bool _is_using_dma = false;

void read_data(string infile_name){
    infp = NULL;
    infp = fopen(infile_name.c_str(),"r");
    if (infp == NULL) {
      printf("fopen %s error\n", infile_name.c_str());
    }
    for(int j = 0 ; j < NumData ; j++){
      for(int i = 0 ; i < condition ; i++){
        fscanf(infp, "%d", &TestData[j][i]);
      }
    }
    fclose(infp);
}


void write_data(string outfile_name){
  int Acount[NumData] = {0};
  int Rcount[NumData] = {0};
  outfp = NULL;
	outfp = fopen(outfile_name.c_str(), "wb");
	if (outfp == NULL)
	{
		printf("Couldn't open response.txt for writing.");
		exit(0);
	}
  for(int r = 0 ; r < NumData ; r++){
    for(int t = 0 ; t < condition ; t++){
      // write value to response file
      if(Result[r][t] == 5){
        Acount[r]++;
      }
      else if(Result[r][t] == 6){
        Rcount[r]++; 
      } 
    }
  }
  for(int r = 0 ; r < NumData ; r++){
    if(Rcount[r]>Acount[r]){
      fprintf( outfp, "No.%d data : ",r+1);
      fprintf( outfp, "Reject \n");
      printf("Reject \n");
    }
    else if(Rcount[r]<Acount[r]){
      fprintf( outfp, "No.%d data : ",r+1);
      fprintf( outfp, "Accept \n");
      printf("Accept \n");
    }
    else{
      fprintf( outfp, "No.%d data : ",r+1);
      fprintf( outfp, "None \n");
      printf("None \n");
    }
  }
  fprintf( outfp, "\n");
	// Close the results file and end the simulation
	fclose( outfp );
}

void write_data_to_ACC0(char* ADDR, unsigned char* buffer, int len){
  if(_is_using_dma){  
    // Using DMA 
    *DMA0_SRC_ADDR = (uint32_t)(buffer);
    *DMA0_DST_ADDR = (uint32_t)(ADDR);
    *DMA0_LEN_ADDR = len;
    *DMA0_OP_ADDR  = DMA0_OP_MEMCPY;
  }else{
    // Directly Send
    memcpy(ADDR, buffer, sizeof(unsigned char)*len);
  }
}
void read_data_from_ACC0(char* ADDR, unsigned char* buffer, int len){
  if(_is_using_dma){
    // Using DMA 
    *DMA0_SRC_ADDR = (uint32_t)(ADDR);
    *DMA0_DST_ADDR = (uint32_t)(buffer);
    *DMA0_LEN_ADDR = len;
    *DMA0_OP_ADDR  = DMA0_OP_MEMCPY;
  }else{
    // Directly Read
    memcpy(buffer, ADDR, sizeof(unsigned char)*len);
  }
}
void write_data_to_ACC1(char* ADDR, unsigned char* buffer, int len){
  if(_is_using_dma){  
    // Using DMA 
    *DMA1_SRC_ADDR = (uint32_t)(buffer);
    *DMA1_DST_ADDR = (uint32_t)(ADDR);
    *DMA1_LEN_ADDR = len;
    *DMA1_OP_ADDR  = DMA1_OP_MEMCPY;
  }else{
    // Directly Send
    memcpy(ADDR, buffer, sizeof(unsigned char)*len);
  }
}
void read_data_from_ACC1(char* ADDR, unsigned char* buffer, int len){
  if(_is_using_dma){
    // Using DMA 
    *DMA1_SRC_ADDR = (uint32_t)(ADDR);
    *DMA1_DST_ADDR = (uint32_t)(buffer);
    *DMA1_LEN_ADDR = len;
    *DMA1_OP_ADDR  = DMA1_OP_MEMCPY;
  }else{
    // Directly Read
    memcpy(buffer, ADDR, sizeof(unsigned char)*len);
  }
}
int main() {
  read_data("stimulus.txt");
  unsigned char buffer0[4];
  unsigned char buffer1[4];
  printf("Start processing...\n");
  for(int t = 0 ; t < 2 ; t++){
    for( int r = 0 ; r < NumData ; r++){
      for( int i = 0; i < condition + 1; i++ )
      {
        if(i<condition){
          buffer0[0] = (unsigned char)TestData[r][i];
          buffer1[0] = (unsigned char)TestData[r][i];
          write_data_to_ACC0(RandomForest0_START_ADDR, buffer0, 4);
          write_data_to_ACC1(RandomForest1_START_ADDR, buffer1, 4);
        }
        else{
          num0 = 2 * t + 1;
          num1 = 2 * t + 1 + 1;
          buffer0[0] = (unsigned char)num0;
          buffer1[0] = (unsigned char)num1;
          write_data_to_ACC0(RandomForest0_START_ADDR, buffer0, 4);
          write_data_to_ACC1(RandomForest1_START_ADDR, buffer1, 4);
        }
      }
      read_data_from_ACC0(RandomForest0_READ_ADDR, buffer0, 4);
      read_data_from_ACC1(RandomForest1_READ_ADDR, buffer1, 4);
      memcpy(data0.uc, buffer0, 4);
      memcpy(data1.uc, buffer1, 4);
      Result[r][2*t] = (int)data0.uc[0];
      Result[r][2*t+1] = (int)data1.uc[0];
    }
  }
  write_data("response.txt");
}


