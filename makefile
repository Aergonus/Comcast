#Comcast MakeFile
app := Comcast.exe

CXX := g++
CXXFLAGS := -std=c++11
LDFLAGS := -c -Wall $(CXXFLAGS)

all: $(app)

$(app): flow.o
	$(CXX) flow.o -o $(app)

flow.o : flow.cpp flow.h tcp.h events/event_TO.h packet.h util.h
	$(CXX) $(LDFLAGS) flow.cpp 

tcp.o : tcp.h
	$(CXX) $(LDFLAGS) tcp.h

events.o : events.cpp
	$(CXX) -o events.cpp events/event.h events/event_TO.h events/event_send_pak.h events/events_start_flow.h
	
packet.o: packet.h util.h
	$(CXX) $(LDFLAGS) packet.h 

util.o: util.h
	$(CXX) $(LDFLAGS) util.h

#debug:
#	$(CXX) -Wall -std=gnu++11 -g -o ui.cpp dVec.cpp flow.cpp host.cpp router.cpp rtable.cpp link.cpp node.cpp packet.cpp

clean:
	rm -f *.exe *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp makefile backups
