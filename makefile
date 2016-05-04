#Comcast MakeFile
app:= Comcast.exe

CXX:= g++
CXXFLAGS:= -std=c++11
LDFLAGS:= -c -Wall $(CXXFLAGS)

all:$(app)

$(app):main.o net.o node.o host.o router.o flow.o link.o
	$(CXX) main.o net.o node.o host.o router.o flow.o link.o -o $(app)

main.o:main.cpp rapidjson/document.h rapidjson/error/en.h  rapidjson/filereadstream.h rapidjson/stringbuffer.h rapidjson/writer.h net.h
	$(CXX) $(LDFLAGS) main.cpp
	
net.o:net.cpp net.h node.h host.h router.h link.h flow.h node.h events/event_start_flow.h events/event_init_rt.h events/event_log.h util.h
	$(CXX) $(LDFLAGS) net.cpp

flow.o:flow.cpp flow.h tcp.h util.h net.h link.h packet.h events/event.h
	$(CXX) $(LDFLAGS) flow.cpp

link.o:link.cpp link.h util.h net.h packet.h node.h host.h events/event_send_pak.h
	$(CXX) $(LDFLAGS) link.cpp
	
router.o:router.cpp router.h node.h link.h
	$(CXX) $(LDFLAGS) router.cpp

host.o:host.cpp host.h node.h util.h link.h flow.h packet.h
	$(CXX) $(LDFLAGS) host.cpp

node.o:node.cpp node.h
	$(CXX) $(LDFLAGS) node.cpp
	
debug:
	$(CXX) -Wall -std=c++11 -g -o debug.out main.cpp net.cpp flow.cpp router.cpp link.cpp host.cpp node.cpp

clean:
	rm -f *.exe *.o *.stackdump *~ *.out
# Added *.out for output txt files

backup:
	test - d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp makefile backups