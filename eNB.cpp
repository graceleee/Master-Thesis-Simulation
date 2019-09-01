#include "eNB.h"
#include <stdio.h>
#include <stdlib.h>
eNodeB::eNodeB()
{
	counter=0; //一開始還沒有NAK
	eNB_MCS=1;
	//gg=6;
	Null=0, Ack=0, Nak=0;
	variation = 0;
	maxVariation = 2;
}
int eNodeB::BLER;
void eNodeB::broadcastMCS (UE* UEarray[])
{
	//呼叫每個UE來決定feedback
	//what this function does is to let each UE update its feedback
	for (int i=0; i<UEnum; i++)
	{
		if (eNB_MCS==UEarray[i]->MCS)
		{
			if (BLER == 0)
				UEarray[i]->setFeedback(ACK);
			else
			{
				if (rand()/(double)RAND_MAX>=0.1)
					UEarray[i]->setFeedback(ACK);
				else
					UEarray[i]->setFeedback(unlucky);
			}
		}
		else if (eNB_MCS>UEarray[i]->MCS)
		{
			UEarray[i]->setFeedback(NAK);
		}
		else
		{
			UEarray[i]->setFeedback(null);
		}
	}
}
void eNodeB::execute (UE* UEarray[])
{
	broadcastMCS (UEarray);
	count (UEarray);
	eNB (UEarray, 2);
	realcount (UEarray);
}
void eNodeB::eNB (UE* UEarray[], char channelnum)
{
	old_eNB_MCS = eNB_MCS;
	if (variation == 1)
	{
		if (eNB_MCS==1)
		{
			counter = 0;
		}
	}
	//若同時收到Nak和Ack，就當作情況沒變=>do nothing。 若只收到NAK沒收到ACK，counter++。若只收到ACK沒收到NAK，counter--
	if (Nak>0 && Ack <=0)
	{
		if (counter <=gg+1)//從<改成<= (4/2)
			counter++;
	}
	else if (Ack >0 && Nak <=0)
	{
		if (counter>0)
		{
			if (counter <=gg+1)
				counter--;
			else
				counter-=2;
		}
	}
	//else if (variation==1) //如果沒有NAK的話，1 channel 當作是ACK，所以counter--
	//{
	//	counter--;
	//}

	//以下開始判斷counter和gg的關係

	if (counter > gg+1) //(double)Nak/UEnum > giveUp
	{
		if (eNB_MCS > 1)
		{
			--eNB_MCS;
			if (variation==1) //如果是1 channel的話，MCS下降時counter要--
			{
				counter--;
			}
		}
	}
	else if (counter <gg && eNB_MCS < MCSsize) //counter<gg
	{
		eNB_MCS++;
		//return eNB_MCS;
	}
}

void eNodeB::printData (int iteration, double totalThroughput)
{
	FILE *f;
	//f = fopen ("UEbehavior.csv", "a");
	f = fopen (Filename.c_str(), "a"); //5/15
	if (f==NULL)
	{
		printf ("Cannot open UEbehavior\n");
		exit(0);
	}
	else
	{
		//"iteration index, # of null,# of ACK,# of NAK, High or Low, Current MCS, New MCS, Throughput\n"
		fprintf (f, "%d,%d,%d,%d,%d,%d,%d,%lf,%d,%d,%d\n",iteration, Null, Ack, Nak, old_eNB_MCS, eNB_MCS, counter, totalThroughput, realNak, realAck, realNull);
	}
	fclose (f);
}

void eNodeB::count (UE* UEarray[])
{
	Null = 0, Ack = 0, Nak = 0;
	for (int i=0; i<UEnum; i++)
	{
		enum Feedback feedback = UEarray[i]->getFeedback();
		if (feedback == NAK) // NAK
		{
			Nak++;
			//nak = true;
		}
		else if (feedback == ACK)
		{
			//ack = true;// ACK
			Ack++;
		}
		else if (feedback ==null)
		{
			Null++;
		}
		else
		{
			printf ("dam\n");
		}
	}
}
void eNodeB::realcount (UE* UEarray[])
{
	realNak=0, realAck=0, realNull=0;
	for (int i=0; i<UEnum; i++)
	{
		if (UEarray[i]->getRealFeedback()==NAK)
		{
			if (UEarray[i]->getSnr()>=-9.478)
				realNak++;
		}
		else if (UEarray[i]->getRealFeedback()==ACK)
		{
			realAck++;
		}
		else if (UEarray[i]->getRealFeedback()==null)
		{
			realNull++;
		}
	}
}

void eNodeB::setVariation (int v)
{
	printf ("eNB variation: %d\n", v);
	if (v<maxVariation && v>=0)
	{
		variation = v;
	}
	else
	{
		printf ("the input eNB variation is illegal, set variation to 0\n");
		variation = 0;
	}
	return;
}