GEN_SRC = KeyValueStore.cpp Administrator.cpp Proposer.cpp keyvaluestore_constants.cpp keyvaluestore_types.cpp helper.cpp
GEN_OBJ = $(patsubst %.cpp,%.o, $(GEN_SRC))

THRIFT_DIR = /usr/local/include/thrift
BOOST_DIR = /usr/local/include/boost

INC = -I$(THRIFT_DIR) -I$(BOOST_DIR)

.PHONY: all clean

all: Server Admin Client

%.o: %.cpp
	$(CXX) -std=c++11 -Wall $(INC) -c $< -o $@

Server: Server.o $(GEN_OBJ)
	$(CXX) -std=c++11 $^ -o $@ -L/usr/local/lib -lthrift -lboost_thread -lboost_system

Admin: Admin.o $(GEN_OBJ)
	$(CXX) -std=c++11 $^ -o $@ -L/usr/local/lib -lthrift

Client: Client.o $(GEN_OBJ)
	$(CXX) -std=c++11 $^ -o $@ -L/usr/local/lib -lthrift

clean:
	$(RM) *.o Server Admin Client