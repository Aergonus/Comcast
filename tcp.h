/**
 * ECE408 
 * tcp.h
 * Purpose: 
 * 
 * @author Eric Nguyen, Kangqiao Lei, Jaeryung Song
 * @version 0.1.8 04/21/16
 */

class TCP {
	public:
		virtual int tripCWND(int currCWND);
}

class TAHOE_TCP {
	public:
		TAHOE_TCP();
		int tripCWMD(int currCWND){return 1;};
		int tripSS(int currCWND){return currCWND/2);
		int probeCWND(int currCWND){return (currCWND++);}
		int slowCWND(int currCWND){return (currCWND++);}
		int fastCWND(int currCWND){return (currCWND++);}
}

class RENO_TCP {
	public:
		RENO_TCP();
		int tripCWMD(int currCWND){return ((currCWND/2) + 3);};
		int tripSS(int currCWND){return currCWND/2);
		int probeCWND(int currCWND){return (currCWND++);}
		int slowCWND(int currCWND){return (currCWND++);}
		int fastCWND(int currCWND){return (currCWND++);}
}
