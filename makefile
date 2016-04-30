#Comcast MakeFile
app := Comcast.exe

CXX := g++
CXXFLAGS := -std=c++11
LDFLAGS := -c -Wall $(CXXFLAGS)

all : $(app)

$(app) : main.o flow.o net.o router.o link.o host.o
	$(CXX) main.o flow.o net.o router.o link.o host.o -o $(app)

main.o : main.cpp rapidjson/document.h rapidjson/error/en.h  rapidjson/filereadstream.h rapidjson/stringbuffer.h rapidjson/writer.h net.h
	$(CXX) $(LDFLAGS) main.cpp
	
net.o : net.cpp net.h host.h router.h link.h flow.h node.h events/event_start_flow.h util.h
	$(CXX) $(LDFLAGS) net.cpp

flow.o : flow.cpp flow.h tcp.h util.h net.h link.h packet.h events/event.h
	$(CXX) $(LDFLAGS) flow.cpp

router.o : router.cpp router.h node.h link.h
	$(CXX) $(LDFLAGS) router.cpp

link.o : link.cpp link.h util.h net.h packet.h host.h events/event_send_pak.h
	$(CXX) $(LDFLAGS) link.cpp

host.o : host.cpp host.h node.h util.h link.h flow.h packet.h
	$(CXX) $(LDFLAGS) host.cpp
	
debug :
	$(CXX) -Wall -std=c++11 -g -o $(app) main.cpp net.cpp flow.cpp router.cpp link.cpp host.cpp

clean :
	rm -f *.exe *.cpp *.stackdump *~

backup :
	test - d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp makefile backups