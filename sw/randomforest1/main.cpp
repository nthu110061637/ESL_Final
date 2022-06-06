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
word data;
int Result [NumData][4];
int TestData[NumData][condition];
int num = 0;
FILE *infp;
FILE *outfp;
// Gassian Filter ACC
static char* const RandomForest_START_ADDR = reinterpret_cast<char* const>(0x73000000);
static char* const RandomForest_READ_ADDR  = reinterpret_cast<char* const>(0x73000004);

// DMA 
static volatile uint32_t * const DMA_SRC_ADDR  = (uint32_t * const)0x70000000;
static volatile uint32_t * const DMA_DST_ADDR  = (uint32_t * const)0x70000004;
static volatile uint32_t * const DMA_LEN_ADDR  = (uint32_t * const)0x70000008;
static volatile uint32_t * const DMA_OP_ADDR   = (uint32_t * const)0x7000000C;
static volatile uint32_t * const DMA_STAT_ADDR = (uint32_t * const)0x70000010;
static const uint32_t DMA_OP_MEMCPY = 1;

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

void write_data_to_ACC(char* ADDR, unsigned char* buffer, int len){
  if(_is_using_dma){  
    // Using DMA 
    *DMA_SRC_ADDR = (uint32_t)(buffer);
    *DMA_DST_ADDR = (uint32_t)(ADDR);
    *DMA_LEN_ADDR = len;
    *DMA_OP_ADDR  = DMA_OP_MEMCPY;
  }else{
    // Directly Send
    memcpy(ADDR, buffer, sizeof(unsigned char)*len);
  }
}
void read_data_from_ACC(char* ADDR, unsigned char* buffer, int len){
  if(_is_using_dma){
    // Using DMA 
    *DMA_SRC_ADDR = (uint32_t)(ADDR);
    *DMA_DST_ADDR = (uint32_t)(buffer);
    *DMA_LEN_ADDR = len;
    *DMA_OP_ADDR  = DMA_OP_MEMCPY;
  }else{
    // Directly Read
    memcpy(buffer, ADDR, sizeof(unsigned char)*len);
  }
}

int main() {
  read_data("stimulus.txt");
  unsigned char buffer[4];
  printf("Start processing...\n");
  for(int t = 0 ; t < 2 ; t++){
    for( int r = 0 ; r < NumData ; r++){
      for( int i = 0; i < condition + 1; i++ )
      {
        if(i<condition){
          buffer[0] = (unsigned char)TestData[r][i];
          //printf("%d\n",TestData[r][i]);
          write_data_to_ACC(RandomForest_START_ADDR, buffer, 4);
        }
        else{
          num = t + 1;
          buffer[0] = (unsigned char)num;
          write_data_to_ACC(RandomForest_START_ADDR, buffer, 4);
        }
      }
      read_data_from_ACC(RandomForest_READ_ADDR, buffer, 4);
      memcpy(data.uc, buffer, 4);
      Result[r][t] = (int)data.uc[0];
    }
  }
  num = 0;
  for(int t = 2 ; t < 4 ; t++){
    for( int r = 0 ; r < NumData ; r++){
      for( int i = 0; i < condition + 1; i++ )
      {
        if(i<condition){
          buffer[0] = (unsigned char)TestData[r][i];
          //printf("%d\n",TestData[r][i]);
          write_data_to_ACC(RandomForest_START_ADDR, buffer, 4);
        }
        else{
          num = t + 1;
          buffer[0] = (unsigned char)num;
          write_data_to_ACC(RandomForest_START_ADDR, buffer, 4);
        }
      }
      read_data_from_ACC(RandomForest_READ_ADDR, buffer, 4);
      memcpy(data.uc, buffer, 4);
      Result[r][t] = (int)data.uc[0];
    }
  }
  write_data("response.txt");
}
