#ifndef UE_H
#define UE_H

#define MCSsize 16
#define MCSmax 1000
#define UEnum 36

enum Feedback {ACK, NAK, null, unlucky};

class UE
{
public:
	enum Feedback getFeedback();
	enum Feedback getRealFeedback();
	void setFeedback(enum Feedback fb);
	int MCS;//0~31
	UE ();
	UE (double snr);
	double getSnr () {return (SNR);}
	static void createSNRtoMCSarray();
	static void createMbitRBarray();
	//static int UEnum;
	void setSNR (int snr);
	double throughput;
	void calculateThroughput (int currentMCS);
	bool shutup; //0沒傳過NAK, 1傳過NAK
	void setVariation (int v);
private:
	enum Feedback feedback;//0:ACK, 1:NAK, 2:null 3:unlucky
	double SNR;
	int RB;
	static double MCS_SNR[MCSsize]; //MCS SNR
	static double MCS_MbitRB[MCSsize]; //Mbit per RB
	void SNRtoMCS();
	double MCStoMbitRB(int currentMCS);
	bool replyACK;
	int variation;
	int maxVariation;
	int NAKcounter;
};

#endif
