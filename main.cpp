/*eNB整理成class*/
/*UE 回傳NAK後就不要再吵，除非自己的MCSmax變了
  eNB有個counter計算已經收到幾個NAK了，當counter < g 時MCS繼續上升
  counter=g時MCS不變
  counter>g時MCS下降，counter--*/
/*gg在eNodeB的constructor*/
/*
6/13:
加上unlucky
outage扣掉SNR<-9.4的
改throughput 計算方法(考慮unlucky)
*/
#include "eNB.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h> //exit (0)
#include <string>
using namespace std;


#define iteration 600
#define move 10

#ifndef FILENAME
#define FILENAME
string Filename;
#endif
/* 
  總共模擬iteration個iteration
  每move個iteration, UE就移動一次*/

void printLabel();
double calculateThroughput (UE* UEarray[], int eNB_MCS);

int main (int argc, char *argv[])
{
	UE::createSNRtoMCSarray();
	UE::createMbitRBarray();
	eNodeB::BLER;
	//printf ("%d\n", *argv[1]-'0');
	//exit (0);
	int eNB_variation, UE_variation;
	eNB_variation = *argv[1]-'0'; //5/15
	UE_variation = *argv[2] -'0'; //5/15
	eNodeB::BLER = *argv[4] - '0'; //1: 有考慮10%NAK，0:沒考慮10%NAK
	printf ("%d, %d\n", eNB_variation, UE_variation);
	/*printf ("What is the variation of the eNB?\n"
		"0: method 5\n"
		"1: when eNB_MCS==1, counter歸零\n");
	scanf ("%d", &eNB_variation); 

	printf ("What is the variation of the UE?\n"
		"0: NAK過之後就shut up, 從NAK變成ACK或null就回傳ACK\n"
		"1: 自我感覺良好的UE如果連續5個iteration都無法解，就回傳NAK\n"
		"2: 1 channel\n");
	scanf ("%d", &UE_variation);*/ //5/15

	//printf ("variation: %d\n", eNB_variation);
	
	Filename = "UE"+to_string(eNB_variation)+"gg"+to_string(*argv[3] -'0')+"#"+to_string(UEnum)+".csv";
	cout <<Filename<<endl;
	//exit (0);

	//////======create UEs, give them snr======//////
	UE* UEarray[UEnum]; //創造出an array of UE* s
	FILE *f;
	if ((f = fopen ("SNRDB/snrdb1", "r"))==NULL) //snrdb1
	{
		printf ("Cannot open snrdb1\n");
		exit (0);
	}
	int snr_num;
	double snr;
	for (snr_num=0; snr_num<UEnum && fscanf(f, "%lf", &snr); snr_num++)
	{
		UEarray[snr_num] = new UE(snr);
	}
	if (snr_num<UEnum)
	{
		printf ("not enough snr\n");
		exit (0);
	}
	fclose(f);
	//////======create UEs, give them snr======//////
	for (int i=0; i<UEnum; i++)
	{
		UEarray[i]->setVariation (UE_variation);
	}

	printLabel();

	eNodeB eNB;
	eNB.gg = *argv[3] -'0'; //5/15
	eNB.Filename = Filename;
	eNB.setVariation (eNB_variation);

	for (int i=0; i<iteration; i++)
	{
		//=====================UE 移動=========================//
		if (i!=0&&i%move==0)
		{
			string a = to_string(i/move+1);
			cout <<"a is "<<a<<endl;
			char b[5];
			strcpy (b,a.c_str());
			//printf ("%s\n", b);
			char filename[100];
			strcpy (filename, "SNRDB/snrdb");
			cout<<strcat (filename, b)<<endl;
			
			if ((f = fopen (filename, "r"))==NULL) //strcat ("snrdb", to_string(i/10+1).c_str())
			{
				printf ("Cannot open SNR\n");
				exit (0);
			}
			int snr_num;
			double snr;
			for (snr_num=0; snr_num<UEnum && fscanf(f, "%lf", &snr); snr_num++)
			{
				UEarray[snr_num]->setSNR(snr);
			}
			if (snr_num<UEnum)
			{
				exit (0);
				printf ("not enough snr\n");
			}
			fclose(f);
		}
		//=====================UE 移動=========================//

		double throughput;
		int currentMCS = eNB.eNB_MCS;
		eNB.execute (UEarray);
		//eNB.broadcastMCS(UEarray);
		throughput = calculateThroughput (UEarray, currentMCS);
		eNB.printData (i, throughput);
	}

	return (0);
}

double calculateThroughput (UE* UEarray[], int eNB_MCS)
{
	for (int i=0; i<UEnum; i++)
	{
		UEarray[i]->calculateThroughput(eNB_MCS);
	}
	double totalThroughput=0;
	for (int i=0; i<UEnum; i++)
	{
		totalThroughput+=UEarray[i]->throughput;
	}
	return (totalThroughput);
}

void printLabel()
{
	FILE *g;
	//g = fopen ("UEbehavior.csv", "a");
	g = fopen (Filename.c_str(), "a"); //5/15
	if (g==NULL)
	{
		printf ("Can't open UEbehavior.csv\n");
	}
	else
	{
		fprintf (g, "Channelnum,# of null,# of ACK,# of NAK, Current MCS, New MCS, counter, Throughput, giveup\n");
	}
	fclose (g);
}