/**
 * ECE408 
 * util.h
 * Purpose: Provides a central organized location 
 * for constants used in the simulation
 * 
 * @author Kangqiao Lei
 * @version 0.5.0 05/03/16
 */
 
#ifndef UTIL_H
#define UTIL_H

#include <fstream>
#include <iostream>

// External Variables 
extern float simtime;
extern int eventsHandled;
extern int eventsCreated;
extern bool debug;
extern std::ostream *debugSS;
extern std::ostream *errorSS;
extern std::ostream *outputSS;

/** Types of Packets in this simulation. */
enum packet_type{
	DATA,
	ACK,
	ROUTING
};

/** Type of TCP implemented */
enum TCP_type{
	TAHOE,
	RENO
};

/** "Small" value to offset from start of simulation */
static const int EPSILON = 1;

/** Default Max No of hops for Packets */
static const int KS_POISION_CONSTANT = 5; // BLEED_OUT_STEPS

/** Size of a Flow Packet MSS in bytes. */
static const long MAX_SEG_SIZE = 1024;

/** Size of an ACK Packet in bytes. */
static const long ACK_PACKET_SIZE = 64;

/** Size of a routing Packet in bytes. */
static const long ROUTING_PACKET_SIZE = 64;

/** Size of a Flow Packet in bytes. */
static const float ALPHA_TIMEOUT = .125;

/** Size of a Flow Packet in bytes. */
static const float BETA_TIMEOUT = .25;

/** Conversion factor between bits and bytes. */
const int BITS_PER_BYTE = 1<<3;

/** Conversion factor between bytes and kilobytes. */
const int BYTES_PER_KB = 1<<10;

/** Conversion factor between kilobytes and megabytes. */
const int KB_PER_MB = 1<<10;

/** Conversion factor between bytes and megabytes. */
const int BYTES_PER_MB = BYTES_PER_KB * KB_PER_MB;

/** Conversion factor between megabits to bytes. */
const int BYTES_PER_MEGABIT = 125000;
		// BYTES_PER_KB * KB_PER_MB / BITS_PER_BYTE;

/** Milliseconds per second. */
const int MS_PER_SEC = 1000;

/** Log throughput every interval */
static const float LOGGING_INTERVAL = 0.1;

/** Update Routing if older than this time */
static const float STALE_ROUTING_TIMEOUT = .3;

#endif // UTIL_H