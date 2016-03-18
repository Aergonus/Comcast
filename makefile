
link.o: link.cc link.h packet.o host.h
	g++ -c link.cc
flow.o: flow.cc flow.h
	g++ -c flow.cc
packet.o: packet.cc packet.h
	g++ -c packet.cc