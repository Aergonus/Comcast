#Comcast MakeFile
app : = Comcast.exe

	CXX : = g++
	CXXFLAGS : = -std = c++11
	LDFLAGS : = -c -Wall $(CXXFLAGS)

	all : $(app)

	$(app) : flow.o
	$(CXX) flow.o - o $(app)

	flow.o : flow.cpp net.h Node.h Link.h Flow.h Packet.h events/event_TO.h tcp.h util.h event.h
	$(CXX) $(LDFLAGS) flow.cpp

	events.o : events.cpp
	$(CXX) - o events.cpp events/event.h events/event_TO.h events/event_send_pak.h events/events_start_flow.h

	packet.o : packet.h util.h
	$(CXX) $(LDFLAGS) packet.h

	util.o : util.h
	$(CXX) $(LDFLAGS) util.h

	host.o : Host.h Packet.h Link.h Flow.h util.h node.h util.h 
	$(CXX) $(LDFLAGS) host.o

	link.o : Host.h Packet.h Link.h events/event_send_pak.h Flow.h util.h node.h util.h
	$(CXX) $(LDFLAGS) link.o

	main.o : rapidjson/document.h rapidjson/error/en.h  rapidjson/filereadstream.h rapidjson/stringbuffer.h rapidjson/writer.h net.h
	$(CXX) $(LDFLAGS) main.o
	
	net.o : net.h host.h router.h link.h flow.h node.h events/event_start_flow.h util.h
	$(CXX) $(LDFLAGS) net.o

	router.o : Router.h events/event.h events/event_manager.h
	$(CXX) $(LDFLAGS) router.o


	#debug :
	#	$(CXX) - Wall - std = gnu++11 - g - o ui.cpp dVec.cpp flow.cpp host.cpp router.cpp rtable.cpp link.cpp node.cpp packet.cpp

	clean :
rm - f *.exe *.o *.stackdump *~

backup :
	test - d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp makefile backups
