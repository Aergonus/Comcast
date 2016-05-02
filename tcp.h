/**
 * ECE408 
 * tcp.h
 * Purpose: 
 * 
 * @author Eric Nguyen, Kangqiao Lei, Jaeryung Song
 * @version 0.2.0 04/21/16
 */

#include "util.h"

class TCP {
	public:
		TCP(){};
		virtual ~TCP(){};
		virtual void tripCWND(int *currCWND) = 0;
		virtual void probeCWND(int *currCWND) = 0;
		virtual void slowCWND(int *currCWND) = 0;
		virtual void fastRetransmit(int *currCWND, int *currSSThresh) = 0;
		virtual void tripDups(int *currCWND) = 0;
		virtual void fastRecovery(int *currCWND, int *currSSThresh) = 0;
};

// TCP Tahoe Congestion Window
class TAHOE_TCP : public TCP {
	public:
		TAHOE_TCP(){};
		~TAHOE_TCP(){};
		void tripCWND(int *currCWND){
			*debugSS << "TCP_TAHOE," << simtime << ",tripCWND," << ",CWND," << *currCWND << ",newCWND," << "1" << std::endl;
			*currCWND = 1;}; // Triple duplicate acks CWND change
		void probeCWND(int *currCWND){
			*debugSS << "TCP_TAHOE," << simtime << ",probeCWND," << ",CWND," << *currCWND << ",newCWND," << *currCWND + 1 << std::endl;
			*currCWND = *currCWND + 1;}; // max probing cwnd
		void slowCWND(int *currCWND){
			*debugSS << "TCP_TAHOE," << simtime << ",slowCWND," << ",CWND," << *currCWND << ",newCWND," << *currCWND + 1 << std::endl;
			*currCWND = *currCWND + 1;}; // Slow start cwnd
		void fastRetransmit(int *currCWND, int *currSSThresh){
			*debugSS << "TCP_TAHOE," << simtime << ",fastRetransmit," << ",CWND," << *currCWND << ",newCWND," << 1 << std::endl;
			*currSSThresh = (*currCWND / 2) < 2 ? 2 : (*currCWND / 2);
			*currCWND = 1;}; //Triple duplicate acks 
		void tripDups(int *currCWND){
			*debugSS << "TCP_TAHOE," << simtime << ",tripDups," << ",CWND," << *currCWND << ",newCWND," << *currCWND << std::endl;
			}; // More than 3 Dup Acks Received, as bad as timeout
		void fastRecovery(int *currCWND, int *currSSThresh){
			*debugSS << "TCP_TAHOE," << simtime << ",fastRecovery," << ",CWND," << *currCWND << ",newCWND," << *currCWND + 1 << std::endl;
			*currCWND = *currCWND + 1;}; // Upon receiving new ack, treat as slow start
};

// TCP Reno Congestion Window
class RENO_TCP : public TCP {
	public:
		RENO_TCP(){};
		~RENO_TCP(){};
		void tripCWND(int *currCWND){
			*debugSS << "TCP_RENO," << simtime << ",tripCWND," << ",CWND," << *currCWND << ",newCWND," << *currCWND + 3 << std::endl;
			*currCWND = *currCWND + 3;}; // Triple duplicate acks CWND change
		void probeCWND(int *currCWND){
			*debugSS << "TCP_RENO," << simtime << ",probeCWND," << ",CWND," << *currCWND << ",newCWND," << *currCWND + 1 << std::endl;
			*currCWND = *currCWND + 1;}; // max probing cwnd
		void slowCWND(int *currCWND){
			*debugSS << "TCP_RENO," << simtime << ",slowCWND," << ",CWND," << *currCWND << ",newCWND," << *currCWND + 1 << std::endl;
			*currCWND = *currCWND + 1;}; // Slow start cwnd
		void fastRetransmit(int *currCWND, int *currSSThresh){
			*debugSS << "TCP_RENO," << simtime << ",fastRetransmit," << ",CWND," << *currCWND << ",newCWND," << *currCWND / 2 << std::endl;
			*currSSThresh = *currCWND = (*currCWND / 2) < 2 ? 2 : (*currCWND / 2); // Fast recovery http://www.faqs.org/rfcs/rfc2001.html
			tripCWND(currCWND);}; //Triple duplicate acks 
		void tripDups(int *currCWND){
			*debugSS << "TCP_RENO," << simtime << ",tripDups," << ",CWND," << *currCWND << ",newCWND," << *currCWND + 1 << std::endl;
			*currCWND = *currCWND + 1;}; // More than 3 Dup Acks Received 
		void fastRecovery(int *currCWND, int *currSSThresh){
			*debugSS << "TCP_RENO," << simtime << ",fastRecovery," << ",CWND," << *currCWND << ",newCWND," << *currSSThresh << std::endl;
			*currCWND = *currSSThresh;}; // Upon receiving new ack, account for all intermediate segments and go into congestion avoidance http://www.faqs.org/rfcs/rfc2001.html
};
