/**
 * ECE408 
 * util.h
 * Purpose: Provides a central organized location 
 * for constants used in the simulation
 * 
 * @author Kangqiao Lei
 * @version 0.2.0 04/21/16
 */
 
#ifndef UTIL_H
#define UTIL_H
// External Variables 
extern float simtime;
extern bool debug;
<<<<<<< HEAD
extern std::ostream debugSS;
extern std::ostream errorSS;
extern std::ostream outputSS;
=======

extern std::ostream *debugSS;
extern std::ostream *errorSS;
extern std::ostream *outputSS;
>>>>>>> 8b583dc3049d36ee176e40937169c7f1d9ee4fe5

/** Types of packets in this simulation. */
typedef enum{
	DATA,
	ACK,
	ROUTING
}packet_type;

/** Type of TCP implemented */
typedef enum{
	TAHOE,
	RENO
}TCP_type;

/** Default Max No of hops for Packets */
static const int KS_POISION_CONSTANT = 15; // BLEED_OUT_STEPS

/** Size of a flow packet MSS in bytes. */
static const long MAX_SEG_SIZE = 512;

/** Size of an ACK packet in bytes. */
static const long ACK_PACKET_SIZE = 64;

/** Size of a routing packet in bytes. */
static const long ROUTING_PACKET_SIZE = 64;

/** Size of a flow packet in bytes. */
static const float ALPHA_TIMEOUT = .125;

/** Size of a flow packet in bytes. */
static const float BETA_TIMEOUT = .25;

/** Conversion factor between bits and bytes. */
const int BITS_PER_BYTE = 1 << 3;

/** Conversion factor between bytes and kilobytes. */
const int BYTES_PER_KB = 1 << 10;

/** Conversion factor between kilobytes and megabytes. */
const int KB_PER_MB = 1 << 10;

/** Conversion factor between megabits to bytes. */
const int BYTES_PER_MEGABIT =
		BYTES_PER_KB * KB_PER_MB / BITS_PER_BYTE;

/** Milliseconds per second. */
const int MS_PER_SEC = 1000;

#endif // UTIL_H
