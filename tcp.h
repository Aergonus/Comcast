/**
 * ECE408 
 * tcp.h
 * Purpose: 
 * 
 * @author Eric Nguyen, Kangqiao Lei, Jaeryung Song
 * @version 0.2.0 04/21/16
 */

class TCP {
	public:
		TCP(){};
		virtual ~TCP(){};
		virtual void tripCWND(int *currCWND) = 0;
		virtual void tripSS(int *currCWND, int *currSSThresh) = 0;
		virtual void probeCWND(int *currCWND) = 0;
		virtual void slowCWND(int *currCWND) = 0;
		virtual void fastCWND(int *currCWND) = 0;
};

// TCP Tahoe Congestion Window
class TAHOE_TCP : public TCP {
	public:
		TAHOE_TCP(){};
		~TAHOE_TCP(){};
		void tripCWND(int *currCWND){ *currCWND = 1;}; // Triple duplicate acks CWND change
		void tripSS(int *currCWND, int *currSSThresh){ *currSSThresh = *currCWND = *currCWND / 2;}; //Triple duplicate acks SSthresh change
		void probeCWND(int *currCWND){ *currCWND = *currCWND + 1;}; // max probing cwnd
		void slowCWND(int *currCWND){ *currCWND = *currCWND + 1;}; // Slow start cwnd
		void fastCWND(int *currCWND){ *currCWND = *currCWND + 1;}; // ?? cwnd (never even gets called...)
};

// TCP Reno Congestion Window
class RENO_TCP : public TCP {
	public:
		RENO_TCP(){};
		~RENO_TCP(){};
		void tripCWND(int *currCWND){ *currCWND = *currCWND + 3;}; //Fast retransmit
		void tripSS(int *currCWND, int *currSSThresh){ *currSSThresh = *currCWND = *currCWND / 2;}; //Triple duplicate acks SSthresh change
		void probeCWND(int *currCWND){ *currCWND = *currCWND + 1;}; // max probing cwnd
		void slowCWND(int *currCWND){ *currCWND = *currCWND + 1;}; // Slow start cwnd
		void fastCWND(int *currCWND){ *currCWND = *currCWND + 1;}; // ?? cwnd (never even gets called...)
};
