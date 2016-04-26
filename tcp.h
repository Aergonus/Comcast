/**
 * ECE408 
 * tcp.h
 * Purpose: 
 * 
 * @author Eric Nguyen, Kangqiao Lei, Jaeryung Song
 * @version 0.2.0 04/21/16
 */
#ifndef TCP_H
#define TCP_H
class TCP {
	public:
		virtual int tripCWND(int currCWND);
};

<<<<<<< HEAD
// TCP Tahoe Congestion Window
class TAHOE_TCP: public TCP {
	public:
		TAHOE_TCP();
		int tripCWMD(int currCWND){return 1;}; // Triple duplicate acks CWND change
		int tripSS(int currCWND){return currCWND/2;}; //Triple duplicate acks SSthresh change
		int probeCWND(int currCWND){return (currCWND++);};// max probing cwnd
		int slowCWND(int currCWND){return (currCWND++);}; // Slow start cwnd
		int fastCWND(int currCWND){return (currCWND++);}; // ?? cwnd (never even gets called...)
};

// TCP Reno Congestion Window
class RENO_TCP : public TCP{
	public:
		RENO_TCP();
		int tripCWMD(int currCWND){return ((currCWND/2) + 3);}; //Fast retransmit
		int tripSS(int currCWND){return currCWND/2;}; //Triple duplicate acks SSthresh change
		int probeCWND(int currCWND){return (currCWND++);}; //max probing cwnd
		int slowCWND(int currCWND){return (currCWND++);}; // slow start cwnd
		int fastCWND(int currCWND){return (currCWND++);}; // ?? cwnd (never gets called)
};
#endif
=======
class TAHOE_TCP : public TCP{
	public:
		TAHOE_TCP();
		int tripCWMD(int currCWND){return 1;};
		int tripSS(int currCWND){return currCWND/2;};
		int probeCWND(int currCWND){return (currCWND++);}
		int slowCWND(int currCWND){return (currCWND++);}
		int fastCWND(int currCWND){return (currCWND++);}
};

class RENO_TCP : public TCP{
	public:
		RENO_TCP();
		int tripCWMD(int currCWND){return ((currCWND/2) + 3);};
		int tripSS(int currCWND){return currCWND/2;};
		int probeCWND(int currCWND){return (currCWND++);}
		int slowCWND(int currCWND){return (currCWND++);}
		int fastCWND(int currCWND){return (currCWND++);}
};
#endif //TCP_H
>>>>>>> 8b583dc3049d36ee176e40937169c7f1d9ee4fe5
