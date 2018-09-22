all:
	g++ DBHandler.cpp Session.cpp HybridServer.cpp server.cpp \
		-lssl -lcrypto -lpthread -lpqxx -lpq -ljsoncpp \
		-lboost_system -lboost_thread -lboost_serialization\
		 -o HServer
