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

#include <fstream>
#include <iostream>

// External Variables 
extern float simtime;
extern bool debug;
extern std::ostream *debugSS;
extern std::ostream *errorSS;
extern std::ostream *outputSS;

/** Types of Packets in this simulation. */
enum packet_type {
	DATA,
	ACK,
	ROUTING
};

/** Type of TCP implemented */
enum TCP_type {
	TAHOE,
	RENO
};

/** Default Max No of hops for Packets */
static const int KS_POISION_CONSTANT = 15; // BLEED_OUT_STEPS

/** Size of a Flow Packet MSS in bytes. */
static const long MAX_SEG_SIZE = 512;

/** Size of an ACK Packet in bytes. */
static const long ACK_PACKET_SIZE = 64;

/** Size of a routing Packet in bytes. */
static const long ROUTING_PACKET_SIZE = 64;

/** Size of a Flow Packet in bytes. */
static const float ALPHA_TIMEOUT = .125;

/** Size of a Flow Packet in bytes. */
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
