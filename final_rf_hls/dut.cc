#include "dut.h"
#include "stratus_hls.h"
// The thread function for the design
void dut::thread1()
{
	// Reset the interfaces
	// Also initialize any variables that are part of the module class here
	{
		HLS_DEFINE_PROTOCOL( "reset" );
		din.reset();
		dout.reset();
		wait();
	}
	//unsigned int i,j,k,m;
	unsigned int n;
	unsigned int m;
	int NumTree;
	for(int a = 0 ; a < TreeSize ; a++){
		tree1[a] = 0;
		tree2[a] = 0;
		tree3[a] = 0;
		tree4[a] = 0;
	}
	tree1[0]=1;tree1[1]=4;tree1[2]=5;tree1[3]=5;tree1[4]=6;
	tree2[0]=2;tree2[1]=4;tree2[2]=3;tree2[3]=5;tree2[4]=6;tree2[5]=6;tree2[6]=5;
	tree3[0]=2;tree3[1]=1;tree3[2]=6;tree3[3]=5;tree3[4]=6;
	tree4[0]=3;tree4[1]=1;tree4[2]=6;tree4[3]=5;tree4[4]=6;

	while( true )
	{
		unsigned int count = 0;
		
		//for (m = 0 ; m < NumData + 1 ; m++){
			//for( n = 0 ; n < condition ; n++){
			for( n = 0 ; n < condition + 1 ; n++){
			HLS_DEFINE_PROTOCOL("input");
			input_t value = din.get();
			count++;
			//test[m][n] = (int)value;
			test[n] = (int)value;
			wait(1);	
			}
		//}
		if(n==condition+1)
		{
			//NumTree = test[NumData][condition-1];
			NumTree = test[condition];
			/*for(m = 0 ; m < NumData + 1 ; m++){
				Prediction[m] = 0;
			}*/
			Prediction = 0;
			switch (NumTree)
			{
				case 1:
					//for(int i = 0 ; i < NumData ; i++)
					//{
						for(int j = 0 ; j < TreeSize ;)
						{
							if((tree1[j] != 5)&&(tree1[j] != 6))
							{
								//if(test[i][tree1[j]-1])
								if(test[tree1[j]-1])
									j = j + j + 1;
								else
									j = j + j + 2;
							}
							else
							{
								//Prediction[i] = tree1[j];
								Prediction = tree1[j];
								break;
							}
						}
					//}
					break;
				case 2:
					//for(int i = 0 ; i < NumData ; i++)
					//{
						for(int j = 0 ; j < TreeSize ;)
						{
							if((tree2[j] != 5)&&(tree2[j] != 6))
							{
								//if(test[i][tree2[j]-1])
								if(test[tree2[j]-1])
									j = j + j + 1;
								else
									j = j + j + 2;
							}
							else
							{
								//Prediction[i] = tree2[j];
								Prediction = tree2[j];
								break;
							}
						}
					//}
					break;
				case 3:
					//for(int i = 0 ; i < NumData ; i++)
					//{
						for(int j = 0 ; j < TreeSize ;)
						{
							if((tree3[j] != 5)&&(tree3[j] != 6))
							{
								//if(test[i][tree3[j]-1])
								if(test[tree3[j]-1])
									j = j + j + 1;
								else
									j = j + j + 2;
							}
							else
							{
								//Prediction[i] = tree3[j];
								Prediction = tree3[j];
								break;
							}
						}
					//}
					break;
				case 4:
					//for(int i = 0 ; i < NumData ; i++)
					//{
						for(int j = 0 ; j < TreeSize ;)
						{
							if((tree4[j] != 5)&&(tree4[j] != 6))
							{
								//if(test[i][tree4[j]-1])
								if(test[tree4[j]-1])
									j = j + j + 1;
								else
									j = j + j + 2;
							}
							else
							{
								//Prediction[i] = tree4[j];
								Prediction = tree4[j];
								break;
							}
						}
					//}
					break;
				default:
					//for(int i = 0 ; i < NumData ; i++){
						//Prediction[i] = 0;
						Prediction = 0;
					//}
					break;
			}
			//for(m = 0 ; m < NumData ; m++)
			{
				HLS_DEFINE_PROTOCOL("output");
				//output_t result = (output_t)Prediction[m];
				output_t result = (output_t)Prediction;
				wait();
				dout.put(result); 
			}
		}
		
		

	}	
}

