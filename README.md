# Master-Thesis-Simulation
*An Efficient Multicast Scheme for Mobile Networks with Reduced Feedback (Simulation)*

This program simulates the algorithms proposed in this [master's thesis](https://ndltd.ncl.edu.tw/cgi-bin/gs32/gsweb.cgi?o=dnclcdr&s=id=%22106NCTU5435071%22.&searchmode=basic&switchlang=en). The goal is to improve the throughput of [SC-PTM](https://www.3gpp.org/technologies/keywords-acronyms/1763-sc_ptm) while UEs transmit minimal feedback.

This thesis is the foundation of Grace Wensan Lee, Ting-Yu Lin, Tsern-Huei Lee, Alex C.-C. Hsu, "Simple Reduced Feedback Multicast Schemes for Mobile Networks" in *2018 International Symposium on Intelligent
Signal Processing and Communication Systems, ISPACS 2018, November 27-30, Ishigaki Island, Okinawa, Japan* (https://ieeexplore.ieee.org/document/8923560)

## Preliminary
* Usually in SC-PTM, either there is no feedback, or all UEs transmit their Channel Quality Indicator (CQI) to the eNodeB.
* The eNodeB, upon receiving the CQI’s from the UEs, determines which modulation and coding scheme (MCS) the broadcast transmission will use.
* The maximal MCS that each UE can correctly decode depends on the UE’s SNR.
* The higher the MCS, the higher throughput each receiving UE can have, but the fewer receiving UEs there would be.

## Proposed Algorithms
1. 1_FBC (1 feedback channel for NAK)
2. 2_FBC (2 feedback channels, 1 feedback channel for ACK and 1 feedback channel for NAK)

## Basic idea of the proposed algorithms
* The UEs informs the eNodeB of any changes in whether the UEs can receive with or without errors
* Each UE records its state with a Boolean variable called mute, and replies NAK and/or ACK when needed (these replies are called feedback)
* The eNodeB records the feedback with an integer variable called counter, and the MCS of the previous iteration with an integer variable called MCS_Index
* Based on the feedback, the eNodeB adjusts the MCS of the next iteration.

## Simulation
The signal to noise ratio (SNR) of each UE is specified in the folder called SNR. Each UE calculates its own MCS in the constructor (MbitRB.txt and MCS_SNR.txt are the input files for this).

The first and second inline arguments specify the eNodeB's behavior, the UEs' behavior (1_FBC or 2_FBC).

The eNodeB simulates transmitting by telling each UE the MCS of the (supposedly) transmission, and the UEs, upon receiving the MCS, would return ACK and/or NAK.

The output of this program is a file called UEbehavior.csv, which includes the number of UEs, the throughput in each iteration, the number of receiving UEs.

## Author's note
The explanation of the algorithms in this README is grossly incomplete, lacking what exactly 1_FBC and 2_FBC are, how the UEs behave, how the eNodeBs behave, and how they exactly tie together. I apologize for this. The master's thesis is not going to be available to the public until 2023, so I'm not exactly sure if I would violate any copyright laws by explaining the entire algorithm here. However, you can take a look at the [draft of the powerpoint slides](https://github.com/graceleee/Master-Thesis-Simulation/blob/master/20180612(draft).pptx) that I used for my defense. Also feel free to contact me if you want to read the draft of the master's thesis (due to complicated reasons the only copy of the final edition of the thesis is a hardcopy in National Chiao Tung University Library, 1001 University Road, Hsinchu, Taiwan).
