#ifndef ENB_H
#define ENB_H
#include "UE.h"
#include <string>
using namespace std;
#define giveUp 0
//#ifndef FILENAME
//#define FILENAME
//string Filename;
//#endif
class eNodeB
{
public:
	eNodeB ();
	int counter; //≠p∫‚NAK¥X¶∏
	int eNB_MCS;
	void execute (UE* UEarray[]);
	void printData (int iteration, double totalThroughput);
	void setVariation (int v);
	int gg; //5/15
	string Filename;
	static int BLER;
private:
	void broadcastMCS (UE* UEarray[]); //broadcast the MCS to the array of UEs
	//int gg;
	int Null, Ack, Nak;
	int realNull, realAck, realNak;
	int old_eNB_MCS;
	void eNB (UE* UEarray[], char channelnum);
	void count (UE* UEarray[]); //count the number of Acks
	void realcount (UE* UEarray[]);
	int variation;
	int maxVariation;
};

#endif