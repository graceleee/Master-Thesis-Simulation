/*eNB��z��class*/
/*UE �^��NAK��N���n�A�n�A���D�ۤv��MCSmax�ܤF
  eNB����counter�p��w�g����X��NAK�F�A��counter < g ��MCS�~��W��
  counter=g��MCS����
  counter>g��MCS�U���Acounter--*/
/*gg�beNodeB��constructor*/
/*
6/13:
�[�Wunlucky
outage����SNR<-9.4��
��throughput �p���k(�Ҽ{unlucky)
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
  �`�@����iteration��iteration
  �Cmove��iteration, UE�N���ʤ@��*/

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
	eNodeB::BLER = *argv[4] - '0'; //1: ���Ҽ{10%NAK�A0:�S�Ҽ{10%NAK
	printf ("%d, %d\n", eNB_variation, UE_variation);
	/*printf ("What is the variation of the eNB?\n"
		"0: method 5\n"
		"1: when eNB_MCS==1, counter�k�s\n");
	scanf ("%d", &eNB_variation); 

	printf ("What is the variation of the UE?\n"
		"0: NAK�L����Nshut up, �qNAK�ܦ�ACK��null�N�^��ACK\n"
		"1: �ۧڷPı�}�n��UE�p�G�s��5��iteration���L�k�ѡA�N�^��NAK\n"
		"2: 1 channel\n");
	scanf ("%d", &UE_variation);*/ //5/15

	//printf ("variation: %d\n", eNB_variation);
	
	Filename = "UE"+to_string(eNB_variation)+"gg"+to_string(*argv[3] -'0')+"#"+to_string(UEnum)+".csv";
	cout <<Filename<<endl;
	//exit (0);

	//////======create UEs, give them snr======//////
	UE* UEarray[UEnum]; //�гy�Xan array of UE* s
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
		//=====================UE ����=========================//
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
		//=====================UE ����=========================//

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