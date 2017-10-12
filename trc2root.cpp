#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream> 
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <math.h>
#include <cstring>
#include <string.h>
#include <sstream>
#include <iomanip>
#include <stdlib.h>
#include <ctime>
#include <dirent.h>


#include "TRint.h"
#include "TROOT.h"
#include "TFile.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TTree.h"

#include  <read_waveform.h>

int main(int argc, char *argv[]) {// -a/-b ./folder/filename number_of_events number_of_channels
	
	bool binary_mode = true;
	char data_type;
	int number_of_events = atof(argv[3]);
	//int number_of_voltages = atof(argv[3]);
		
	// here we check the type of data
	data_type = *argv[1];
	switch (data_type) {
		case 'a':
			binary_mode = false;
		break;
		case 'b':
			binary_mode = true;
		break;
		default:
		fprintf(stderr,"\nUnknown file type: %c\n",data_type);
		exit(1);
	}
	
	// here we obtain the name of file
	char *filename_in = argv[2];
	int l0 = strlen(filename_in)-9;
	int l1 = l0 - 23;
	
	// here we obtain the number of channels
	const int number_of_channels = argc-4;
	int channels[number_of_channels];
	for (int i=0;i<number_of_channels;i++) channels[i] = atof(argv[4+i]);
	
	// here we obtain the start event
	char n_start_str[6];
	for (int i=0;i<5;i++) n_start_str[i]=*(filename_in+l0+i);
	int n_start=atof(n_start_str);
	
	// here we obtain the info about the waveform		
	int32_t number_of_points = readinfo_b(filename_in);
	Float_t time[number_of_points][NUMCHS],amplitude[number_of_points][NUMCHS],points[number_of_points][NUMCHS];
	Float_t bias_voltage;
	Int_t npoint,nevent,temperature,number_of_chans;
	
	//printf("\nname: %s\n",filename_in);
	
	// here we create output file
	char filename_out[l1];
	int bias_folder_name = 17;
	for (int i=0;i<bias_folder_name;i++) filename_out[i] = *(filename_in+2+i);
	for (int i=bias_folder_name;i<l1+bias_folder_name;i++) filename_out[i] = *(filename_in+5+i);
	TString filename_out_t = filename_out;
	filename_out_t += "_output.root";
	TFile *file_out = new TFile(filename_out_t,"RECREATE");
	
	//printf("\nname: %s\n",filename_out);
	
	// here we create the Tree
	TTree *tree = new TTree("Events","Events");	
	tree->Branch("number_of_samples",&npoint,"npoint/I");
	tree->Branch("number_of_events",&nevent,"nevent/I");
	tree->Branch("number_of_channels",&number_of_chans,"number_of_chans/I");
	tree->Branch("bias_voltage",&bias_voltage,"bias_voltage/F");
	tree->Branch("temperature",&temperature,"temperature/I");
	tree->Branch("time",time,TString::Format("time[npoint][%d]/F",number_of_channels));
	tree->Branch("amplitude",amplitude,TString::Format("amplitude[npoint][%d]/F",number_of_channels));
	tree->Branch("points",points,TString::Format("points[npoint][%d]/F",number_of_channels));
	
	// here we obtain voltage first step
	//char name_start_voltage = *(filename_in+2);
	//int voltage_start_step = name_start_voltage-'0';
	//printf("voltage step is: %d",voltage_start_step);
	
	char filename_voltage[5],filename_temp[2];
	int error;
	
	// here we convert each waveform in Tree
	//for (int k=voltage_start_step;k<=voltage_start_step+number_of_voltages;k++) {
		
		//obtain bias voltage and temperature from folder name
		for (int i=0;i<2;i++) filename_voltage[i] = *(filename_in+8+i);
		filename_voltage[2] = '.';
		for (int i=3;i<5;i++) filename_voltage[i] = *(filename_in+8+i);
		for (int i=0;i<2;i++) filename_temp[i] = *(filename_in+4+i);
		
		// add these values in TTree
		bias_voltage = atof(filename_voltage);
		temperature = atof(filename_temp);
		npoint = number_of_points;
		nevent = number_of_events;
		number_of_chans = number_of_channels;
		
		
		for (int j=n_start;j<n_start+number_of_events;j++) {
									
			*(filename_in+l0+4) = '0' + j%10;
			*(filename_in+l0+3) = '0' + (j/10)%10;
			*(filename_in+l0+2) = '0' + (j/100)%10;
			*(filename_in+l0+1) = '0' + (j/1000)%10;
			*(filename_in+l0)   = '0' + (j/10000)%10;
			
					
			for (int i=0;i<number_of_channels;i++) {
				
			
				*(filename_in+21) = channels[i] + '0';
				
				error = readwfm_b(i,number_of_channels,*time,*amplitude,*points,filename_in);
				//for (int t=0;t<5002;t++) {
				//	printf("\n point: %d, channel: %d, data: %f",t,i,amplitude[t][i]);
				//}
			}
						
			if (error) continue;
			tree->Fill();
				
		}

	//}
	
	
	// here we save data and close the files
	tree->Write();
	file_out->Write();
	file_out->Close();

	return 0;

}
