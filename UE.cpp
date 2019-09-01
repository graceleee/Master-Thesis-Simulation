#include "UE.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
UE::UE()
{
	//cout <<"x";
}
UE::UE(double snr)
{
	SNR = snr;
	SNRtoMCS();
	RB=10;
	shutup = false;
	replyACK = false;
	variation = 0;
	maxVariation = 3;
	NAKcounter = 0;
}
void UE::setSNR (int snr)
{
	//int oldMCS = MCS;
	SNR = snr;
	SNRtoMCS();
	/*if (oldMCS < MCS)
	{
		replyACK = true;
	}*/
}
double UE::MCS_SNR[MCSsize]; //without this one, the static double MCS_SNR[MCSsize] will cause LNK2001
double UE::MCS_MbitRB[MCSsize];
void UE::createSNRtoMCSarray()
{
	FILE *f;
	if ((f = fopen("MCS_SNR.txt", "r"))==NULL)
	{
		printf ("Cannot open file\n");
		exit (0);
	}
	double SNR_value;
	for (int i=0; i<MCSsize && fscanf(f, "%lf", &SNR_value); i++)
	{
		MCS_SNR[i]=SNR_value;
	}
	fclose (f);
	MCS_SNR[MCSsize-1]=MCSmax;
}
void UE::SNRtoMCS ()//conver SNR to MCS by looking up the double MCS_SNR
{
	int low = 0;
	int high = MCSsize-1;
	while (low < high)
	{
		int mid = (low+high)/2;
		if (SNR <=MCS_SNR[mid])
		{
			high = mid;
		}
		else
		{
			low = mid+1;
		}
	}
	MCS = high-1;
	return;
}
void UE::createMbitRBarray()
{
	FILE *f;
	if ((f = fopen("MbitRB.txt", "r"))==NULL)
	{
		printf ("Cannot open file\n");
		exit (0);
	}
	double MbitRB_value;
	for (int i=0; i<MCSsize && fscanf(f, "%lf", &MbitRB_value); i++)
	{
		MCS_MbitRB[i]=MbitRB_value;
	}
	fclose (f);
}
double UE::MCStoMbitRB(int currentMCS)
{
	if (currentMCS<MCSsize&&currentMCS>=0)
	{
		return (MCS_MbitRB[currentMCS]);
	}
	else
	{
		printf ("MCS>MCSsize\n");
		exit(0);
	}
}
void UE::calculateThroughput(int currentMCS)
{
	double x;
	if (currentMCS>MCS || feedback==unlucky)
		x=0;
	else
		x = MCS_MbitRB[currentMCS]; //TBS
	throughput = x/1000.0; // Mbit/sec
}

enum Feedback UE::getFeedback()
{
	switch (variation)
	{
	case 2:
		if (feedback == NAK && shutup == false)
		{
			shutup = true;
			return NAK;
		}
		else
		{
			return null;
		}
		break;
	case 1:
		if (feedback == NAK)
		{
			if (MCS > 10)
			{
				NAKcounter++;
			}
			if (shutup == false)
			{
				shutup = true;
				return (NAK);
			}
			if (MCS > 10 && NAKcounter > 5)
			{
				printf ("f\n");
				NAKcounter = 0;
				return (NAK);
			}
			else
			{
				return (null);
			}
		}//end of if (feedback == NAK)
		else if (replyACK)
		{
			replyACK = false;
			return (ACK);
		}
		else
		{
			return (null);
		}

		break;
	case 0:
	default:
		if (feedback == NAK && shutup == false)
		{
			shutup = true;
			return NAK;
		}
		else if (replyACK)
		{
			//printf ("ACK\n");
			replyACK = false;
			return ACK;	
		}
		else
		{
			return null;
		}
		break;

	}
}
enum Feedback UE::getRealFeedback()
{
	return (feedback);
}

void UE::setFeedback(enum Feedback fb)
{
	if (feedback==NAK && fb != NAK)//原本NAK，後來變成不是NAK
	{
		replyACK = true;
	}
	if (fb != NAK)
	{
		shutup = false;
	}

	feedback = fb;
}
void UE::setVariation (int v)
{
	if (v<maxVariation && v>=0)
	{
		variation = v;
	}
	else
	{
		printf ("the input UE variation is illegal, set variation to 0\n");
		variation = 0;
	}
	return;
}