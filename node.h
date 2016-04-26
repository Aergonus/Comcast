/**
 * ECE408 
 * node.h
 * Purpose: Nodes are points of the network, i.e. hosts and routers
 * 
 * @author Jaeryung Song, Kangqiao Lei
 * @version 0.2.0 04/19/16
 */

#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>
#include "util.h"
#include "packet.h"

class link;

class node {
<<<<<<< HEAD
	protected:
		std::string name; //node id
		std::vector<link *> links; // list of links connected to node
=======
	private:
		std::string name;
		std::vector<link*> links; 
>>>>>>> 8b583dc3049d36ee176e40937169c7f1d9ee4fe5

	public: 
		// Constructor
		node(const std::string name):name(name){};
		
<<<<<<< HEAD
                bool operator == (const node& n) const{
                  return (name == n.name);
                }
		std::string getName(){return name;} // Gets address of the node
		int addLink(link *l); // Add link to the node
		std::vector<link *> getLinks(){return links;}; // Gets list of links connected to the node
		virtual void receive_packet(packet *p) = 0;
		link* getConnectedLink(node *connection); // Gets the link connected to the given node
		// Debug
		std::string print();
=======
		std::string getName(){return name;}; // Gets address of the node
		int addLink(link*); // Add link to the node
		std::vector<link*> getLinks(){return links;}; // Gets list of links connected to the node
		virtual int receive_packet(packet*) = 0;
		node* getConnectedNode(link*); // Gets the link connected to the given node
		void print();
>>>>>>> 8b583dc3049d36ee176e40937169c7f1d9ee4fe5
};
#endif
