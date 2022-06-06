
#include "tb.h"
#include <esc.h>		// for the latency logging functions
#include <string>
#include <iostream>

void tb::source()
{
	dout.reset();		// reset the outputs and cycle the design's reset
	rst.write( 0 );		// assert reset (active low)
	wait( 2 );		// wait 2 cycles
	rst.write( 1 );		// deassert reset
	wait(1);
	// Open the stimulus file
	char stim_file[256] = "stimulus.dat";
	infp = fopen( stim_file, "r" );
	if( infp == NULL )
	{
		cout << "Couldn't open stimulus.dat for reading." << endl;
		exit(0);
	}
	total_start_time = sc_time_stamp();
	int num = 2;
	// Read stimulus values from file and send to DUT
	for( int r = 0 ; r < NumData ; r++){
		for( int i = 0; i < condition + 1; i++ )
		{
			if(i<condition){
				fscanf(infp, "%d\n", &TestData[r][i]);
				dout.put( (input_t)TestData[r][i]);
				cout << TestData[r][i];
			}
			else
				dout.put( (input_t)num);
			
		}
		cout << endl;
	}
	cout << endl;
	// Guard condition: after 100000 cycles the sink() thread should have ended the simulation.
	// If we're still here, timeout and print error message.
	wait( 100000 );
	cout << "cow";
    fclose( infp );
	cout << "Error! TB source thread timed out!" << endl;
	esc_stop();
}

// The sink thread reads all the expected values from the design
void tb::sink()
{
	din.reset();
	wait(2);     // to synchronize with reset
	wait(1);
	// Extract clock period from clk port
	sc_clock * clk_p = DCAST < sc_clock * >( clk.get_interface() );
	sc_time clock_period = clk_p->period();

	// Open the simulation results file
	char output_file[256];
	sprintf( output_file, "%s/response.dat", getenv("BDW_SIM_CONFIG_DIR") );
	outfp = fopen(output_file, "wb");
	if (outfp == NULL)
	{
		cout << "Couldn't open output.dat for writing." << endl;
		exit(0);
	}

	// Read outputs from DUT
	
	for( int r = 0 ; r < NumData ; r ++){
			output_t inVal = din.get();
			cout << inVal << endl;
			if((int)inVal == 5){
				fprintf( outfp, "No.%d data :",r+1);
				fprintf( outfp, "Accept \n");
				cout << "Accept" << endl;
			}
			else if((int)inVal == 6){
				fprintf( outfp, "No.%d data :",r+1);
				fprintf( outfp, "Reject \n");
				cout << "Reject" << endl;
			}
			else{
				fprintf( outfp, "No.%d data",r+1);
				fprintf( outfp, "None \n");
				cout << "None" << endl;
			}
		// write value to response file
		total_latency = sc_time_stamp() - total_start_time ;
		cout << "Total latency " << total_latency << "." << endl;
		fprintf( outfp, "\n");
	}
	total_latency = sc_time_stamp() - total_start_time ;
	cout << "Total latency " << total_latency << "." << endl;
	// Close the results file and end the simulation
	fclose( outfp );
	esc_stop();
}
