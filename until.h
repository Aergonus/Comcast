/**
 * ECE408 
 * util.h
 * Purpose: Provides a central organized location 
 * for constants used in the simulation
 * 
 * @author Kangqiao Lei
 * @version 0.1 03/13/16
 */
 
#ifndef UTIL_H
#define UTIL_H

/** Types of packets in this simulation. */
enum packet_type {
	FLOW,
	ACK,
	ROUTING
};

/** Size of a flow packet in bytes. */
static const long FLOW_PACKET_SIZE = 512;

/** Size of an ACK packet in bytes. */
static const long ACK_PACKET_SIZE = 64;

/** Size of a routing packet in bytes. */
static const long ROUTING_PACKET_SIZE = 64;

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