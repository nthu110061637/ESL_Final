#ifndef RANDOM_FOREST_H_
#define RANDOM_FOREST_H_
#include <systemc>
#include <cmath>
#include <iomanip>
#include "defines.h"
using namespace sc_core;

#include <tlm>
#include <tlm_utils/simple_target_socket.h>

#include "defines.h"

struct RandomForest : public sc_module {
  tlm_utils::simple_target_socket<RandomForest> tsock;

  sc_fifo<unsigned char> ToBeTest;
  sc_fifo<unsigned char> Predict;

  unsigned int base_offset;
  SC_HAS_PROCESS(RandomForest);

  RandomForest(sc_module_name n): 
    sc_module(n), 
    tsock("t_skt"), 
    base_offset(0) 
  {
    tsock.register_b_transport(this, &RandomForest::blocking_transport);
    SC_THREAD(traverse);
  }

  ~RandomForest() {
	}

  void traverse(){
    { wait(CLOCK_PERIOD, SC_NS); }
    int Prediction;
    int test[condition+1];
    unsigned int n;
    int NumTree;
    int tree1[TreeSize];
    int tree2[TreeSize];
    int tree3[TreeSize];
    int tree4[TreeSize];
    for(int a = 0 ; a < TreeSize ; a++){
      tree1[a] = 0;
      tree2[a] = 0;
      tree3[a] = 0;
      tree4[a] = 0;
      wait(CLOCK_PERIOD, SC_NS);
    }
    tree1[0]=1;tree1[1]=4;tree1[2]=5;tree1[3]=5;tree1[4]=6;
    tree2[0]=2;tree2[1]=4;tree2[2]=3;tree2[3]=5;tree2[4]=6;tree2[5]=6;tree2[6]=5;
    tree3[0]=2;tree3[1]=1;tree3[2]=6;tree3[3]=5;tree3[4]=6;
    tree4[0]=3;tree4[1]=6;tree4[2]=1;tree4[5]=6;tree4[6]=5;

    while( true )
    {
        for( n = 0 ; n < condition + 1 ; n++){
          test[n] = ToBeTest.read();
          wait(CLOCK_PERIOD, SC_NS);
        }
        if(n==condition+1)
        {
          NumTree = test[condition];
          Prediction = 0;
          switch (NumTree+1)
          {
            case 1:
                for(int j = 0 ; j < TreeSize ;)
                {
                  if((tree1[j] != 5)&&(tree1[j] != 6))
                  {
                    if(test[tree1[j]-1])
                      j = j + j + 1;
                    else
                      j = j + j + 2;
                  }
                  else
                  {
                    Prediction = tree1[j];
                    break;
                  }
                  wait(CLOCK_PERIOD, SC_NS);
                }
                break;
            case 2:
                for(int j = 0 ; j < TreeSize ;)
                {
                  if((tree2[j] != 5)&&(tree2[j] != 6))
                  {
                    if(test[tree2[j]-1])
                      j = j + j + 1;
                    else
                      j = j + j + 2;
                  }
                  else
                  {
                    Prediction = tree2[j];
                    break;
                  }
                  wait(CLOCK_PERIOD, SC_NS);
                }
                break;
            case 3:
                for(int j = 0 ; j < TreeSize ;)
                {
                  if((tree3[j] != 5)&&(tree3[j] != 6))
                  {
                    if(test[tree3[j]-1])
                      j = j + j + 1;
                    else
                      j = j + j + 2;
                  }
                  else
                  {
                    Prediction = tree3[j];
                    break;
                  }
                  wait(CLOCK_PERIOD, SC_NS);
                }
                break;
            case 4:
                for(int j = 0 ; j < TreeSize ;)
                {
                  if((tree4[j] != 5)&&(tree4[j] != 6))
                  {
                    if(test[tree4[j]-1])
                      j = j + j + 1;
                    else
                      j = j + j + 2;
                  }
                  else
                  {
                    Prediction = tree4[j];
                    break;
                  }
                  wait(CLOCK_PERIOD, SC_NS);
                }
                break;
            default:
                Prediction = 0;
                wait(CLOCK_PERIOD, SC_NS);
                break;
          }
            unsigned char answer = (unsigned char)Prediction;
            Predict.write(answer); 
        }
    }	
  }


  void blocking_transport(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay){
    wait(delay);
    // unsigned char *mask_ptr = payload.get_byte_enable_ptr();
    // auto len = payload.get_data_length();
    tlm::tlm_command cmd = payload.get_command();
    sc_dt::uint64 addr = payload.get_address();
    unsigned char *data_ptr = payload.get_data_ptr();

    addr -= base_offset;


    // cout << (int)data_ptr[0] << endl;
    // cout << (int)data_ptr[1] << endl;
    // cout << (int)data_ptr[2] << endl;
    word buffer;

    switch (cmd) {
      case tlm::TLM_READ_COMMAND:
        // cout << "READ" << endl;
        switch (addr) {
          case RANDOM_FOREST_RESULT_ADDR:
            buffer.uc[0] = Predict.read();
            break;
          default:
            std::cerr << "READ Error! RandomForest::blocking_transport: address 0x"
                      << std::setfill('0') << std::setw(8) << std::hex << addr
                      << std::dec << " is not valid" << std::endl;
          }
        data_ptr[0] = buffer.uc[0];
        data_ptr[1] = buffer.uc[1];
        data_ptr[2] = buffer.uc[2];
        data_ptr[3] = buffer.uc[3];
        break;
      case tlm::TLM_WRITE_COMMAND:
        // cout << "WRITE" << endl;
        switch (addr) {
          case RANDOM_FOREST_R_ADDR:
            ToBeTest.write(data_ptr[0]);
            break;
          default:
            std::cerr << "WRITE Error! RandomForest::blocking_transport: address 0x"
                      << std::setfill('0') << std::setw(8) << std::hex << addr
                      << std::dec << " is not valid" << std::endl;
        }
        break;
      case tlm::TLM_IGNORE_COMMAND:
        payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
        return;
      default:
        payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
        return;
      }
      payload.set_response_status(tlm::TLM_OK_RESPONSE); // Always OK
  }
};
#endif
