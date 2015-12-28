/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TMultiplexedProtocol.h>
#include <thrift/processor/TMultiplexedProcessor.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/TToString.h>

#include <boost/make_shared.hpp>
#include <boost/thread/mutex.hpp>
#include <mutex>
#include <boost/thread/lock_guard.hpp>

#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>
#include <thread>
#include <atomic>
#include <random>

#include "KeyValueStore.h"
#include "Administrator.h"
#include "Proposer.h"
#include "helper.hpp"

#define AWAITING_COMMIT_TIMEOUT_SECS 5

using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

using namespace keyvaluestore;
using namespace std;

//**********************************************************
// error
//
// logs errors and quits
//**********************************************************
void error(const string msg)
{
  fprintf(stderr, "%s ERROR: %s\n", systemTime().c_str(), msg.c_str());
  exit(1);
}

// A struct to hold a client and transport
struct ProposerClientTransport {
  ProposerClient client;
  boost::shared_ptr<TTransport> transport;

  ProposerClientTransport(ProposerClient pClient, boost::shared_ptr<TTransport> pTransport)  : client(pClient) {
    transport = pTransport;
  }

};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// KeyValueStoreServer
// all local services share these values
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class KeyValueStoreServer {
private:
  int maxRound;
  std::mutex storelog_mutex;

public:
  int serverID;
  ProposalNumber minProposal;
  ProposalNumber acceptedProposal;
  Request acceptedValue;
  atomic<bool> isPreparing, isAccepting;

  ServerAddress serverAddress;
  map<string, string> hashmap;
  map<ServerAddress, ProposerClientTransport*> connectedServers;

public:
  // Constructor
  KeyValueStoreServer(int port, string hostname) {
    serverAddress.port = port;
    serverAddress.hostname = hostname;
    maxRound = 0;
  }

  // Turn a request into a string
  string request_toString(const Request &request) {
    string operation, what;
    if (request.op == Operation::PUT) operation = "PUT";
    else if (request.op == Operation::GET) operation = "GET";
    else if (request.op == Operation::ERASE) operation = "DEL";

    what += " " + operation + " " + request.key;
    if (request.value != "") what += " " + request.value;

    return what;
  }

  // Log a string
  void log(string what) {
    ofstream server_log;
    string serverName = serverAddress.hostname + "_" + to_string(serverAddress.port);
    string logName = "../Logs/status/server_" + serverName + "_log.txt";
    server_log.open(logName, ios_base::app);

    server_log << systemTime() << " " << what << "\n";
  }

  // Log an event with a server
  void log(string what, const ServerAddress &serverAddress) {
    log(what + " " + serverAddress.hostname + ":" + to_string(serverAddress.port));
  }

  // Log a request
  void log(string what, const Request &request) {
    what += request_toString(request);
    log(what);
  }

  // Writes all the operations that took place
  void operationlog(const Request &request) {
    ofstream server_log;
    string serverName = serverAddress.hostname + "_" + to_string(serverAddress.port);
    string logName = "../Logs/operation/server_" + serverName + "_operationlog.txt";
    server_log.open(logName, ios_base::app);

    string what = to_string(acceptedProposal.roundNumber) + " " + to_string(acceptedProposal.serverID);
    what += request_toString(request);

    server_log << what << "\n";
  }

  // The server is just starting or recovering from failure
  void read_storelog() {
    // Lock
    std::lock_guard<std::mutex> storelog_lk(storelog_mutex);

    // Open store log for reading
    string serverName = serverAddress.hostname + "_" + to_string(serverAddress.port);
    string logName = "../Logs/store/server_" + serverName + "_storelog.txt";
    ifstream server_log(logName);

    string line;
    while (getline(server_log, line))
    {
      istringstream iss(line);
      string type;
      iss >> type;
      if (type == "MAXROUND") {
        iss >> maxRound;
      }
      else if (type == "MINPROPOSAL") {
        iss >> minProposal.roundNumber >> minProposal.serverID;
      }
      else if (type == "ACCEPTEDVALUE") {
        string operation;
        iss >> operation >> acceptedValue.key >> acceptedValue.value;
        if (operation == "PUT") acceptedValue.op = Operation::PUT;
        else if (operation == "GET") acceptedValue.op = Operation::GET;
        else if (operation == "DEL") acceptedValue.op = Operation::ERASE;
      }
      else if (type == "ACCEPTEDPROPOSAL") {
        iss >> acceptedProposal.roundNumber >> acceptedProposal.serverID;
      }
    }
  }

  // Store critical values
  void update_storelog() {
    // Lock
    std::lock_guard<std::mutex> storelog_lk(storelog_mutex);

    // Open store log
    ofstream server_log;
    string serverName = serverAddress.hostname + ":" + to_string(serverAddress.port);
    string logName = "../Logs/store/server_" + serverName + "_storelog.txt";
    server_log.open(logName);

    // Write current values
    server_log << "MAXROUND " << maxRound << "\n";
    server_log << "MINPROPOSAL " << minProposal.roundNumber << " " << minProposal.serverID << "\n";
    server_log << "ACCEPTEDVALUE " << request_toString(acceptedValue) << "\n";
    server_log << "ACCEPTEDPROPOSAL " << acceptedProposal.roundNumber << " " << acceptedProposal.serverID << "\n";
  }

  // Connect to acceptor but don't say hello
  void connectToAcceptorNoHello(const ServerAddress &pServerAddress) {
    log("Attempting to connect to acceptor", pServerAddress);
    boost::shared_ptr<TTransport> socket(new TSocket(pServerAddress.hostname, pServerAddress.port));
    boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    boost::shared_ptr<TProtocol> mp(new TMultiplexedProtocol(protocol, "Proposer"));
    ProposerClient acceptor(mp);
    try {
      // Connect to server
      transport->open();
      // Store transport and client
      connectedServers[pServerAddress] = new ProposerClientTransport(acceptor, transport);

    } catch (TException& tx) {
      string what(tx.what());
      log("Failed to connect to acceptor", pServerAddress);
    }
  }
  // Connect to another server's acceptor (AKA ProposerClient)
  void connectToAcceptor(const ServerAddress &pServerAddress) {
    log("Attempting to connect to acceptor", pServerAddress);
    // Make sure not already connected
    auto it = connectedServers.find(pServerAddress);
    // Not found, try to connect
    if (it == connectedServers.end() || it->second->transport == NULL) {
      // Set up connection
      boost::shared_ptr<TTransport> socket(new TSocket(pServerAddress.hostname, pServerAddress.port));
      boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
      boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
      boost::shared_ptr<TProtocol> mp(new TMultiplexedProtocol(protocol, "Proposer"));
      ProposerClient acceptor(mp);
      try {
        // Connect to server
        transport->open();
        // Introduce yourself to server
        if (acceptor.hello(serverAddress)) {
          // Store transport and client
          connectedServers[pServerAddress] = new ProposerClientTransport(acceptor, transport);
        }
        else {
          log("Acceptor did not return hello", pServerAddress);
        }
      } catch (TException& tx) {
        string what(tx.what());
        log("Failed to connect to acceptor", pServerAddress);
      }
    } // end if
  }

  // Set the Max Round number
  // if value is not greater than current max round return 0
  // Otherwise return max round
  int setMaxRound(const int roundNumber) {
    // Set the maxRound
    if (roundNumber <= maxRound) {
      return 0;
    }
    maxRound = roundNumber;
    // Log the new maxRound for recovery
    update_storelog();

    return maxRound;
  }

  int incrementMaxRound() {
    return setMaxRound(maxRound + 1);
  }

  void setAcceptedValue(const Request &request) {
    acceptedValue = request;
  }

};

//==========================================================
// HANDLERS
//==========================================================

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// AdministratorHandler
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class AdministratorHandler : virtual public AdministratorIf {
private:
  KeyValueStoreServer* thisServer;

public:
  AdministratorHandler(KeyValueStoreServer* server) {
    thisServer = server;
  }

  // Set this servers ID
  // There is no check for duplicate IDs
  // Onus is on human administrator
  bool setID(const int16_t id) {
    thisServer->serverID = id;
    thisServer->log("Set server ID to " + to_string(id));
    return true;
  }

  // Get a list of connected servers from diplomat and
  // establish connection with all of them
  bool connectToPaxos(const ServerAddress& serverAddress) {
    bool result = false;
    thisServer->log("Connecting to PAXOS");
    // First connect to yourself
    thisServer->connectToAcceptorNoHello(thisServer->serverAddress);

    // Set up connection with diplomat
    boost::shared_ptr<TTransport> socket(new TSocket(serverAddress.hostname, serverAddress.port));
    boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    boost::shared_ptr<TProtocol> mp(new TMultiplexedProtocol(protocol, "Administrator"));
    AdministratorClient diplomat(mp);
    try {
      transport->open();
      // Fetch connected servers from already connected server
      set<ServerAddress> connectedServerAddresses;
      diplomat.getConnectedServers(connectedServerAddresses);
      // Connect to all servers provided by diplomat
      for (auto serverAddr : connectedServerAddresses) {
        thisServer->connectToAcceptor(serverAddr);
      }
      thisServer->log("Finished connecting to acceptors");
      // Don't need this connection live anymore
      transport->close();
      result = true;
    } catch (TException& tx) {
      thisServer->log("Unable to connect to PAXOS via server", serverAddress);
    }
    thisServer->log("Connected to PAXOS");
    return result;
  }

  // Tell all servers you are leaving and
  // clear list of connected servers
  bool disconnectFromPaxos() {
    bool result = true;
    thisServer->log("Disconnecting from PAXOS");
    // Say goodbye to all connected servers
    auto it = thisServer->connectedServers.begin();
    while (it != thisServer->connectedServers.end()) {
      // Say goodbye
      if (!it->second->client.goodbye(thisServer->serverAddress)) {
        thisServer->log("Goodbye unsuccessful", it->first);
      }
      // Close the transport
      try {
        it->second->transport->close();
      } catch (TException& tx) {
        thisServer->log("Closing transport unsuccessful", it->first);
      }
      // Remove from list of servers
      it = thisServer->connectedServers.erase(it);
    }
    thisServer->log("Disconnected from PAXOS");
    return result;
  }

  // Return a list of server addresses connected with PAXOS
  void getConnectedServers(std::set<ServerAddress> & _return) {
    thisServer->log("Received request to get connected servers");
    for (auto &server : thisServer->connectedServers) {
      _return.insert(server.first);
    }
  }
};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// KeyValueStoreHandler
//
// These are Proposers/Learners
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class KeyValueStoreHandler : virtual public KeyValueStoreIf {
private:
  KeyValueStoreServer* thisServer;
  atomic<int> numPrepareResponses;
  atomic<int> numAcceptResponses;
  std::mutex paxos_mutex, values_mutex, nums_mutex;
  vector<ProposalValue> propVals;
  vector<ProposalNumber> resultNums;


  // Prepare gets called on all connected acceptors
  void prepareThread(ProposerClient acceptor, const ProposalNumber &propNum)
  {
    ProposalValue _return;
    try {
      acceptor.prepare(_return, propNum);
      // Update value if return proposal number is greater than maxRound
      // and only update accepted value if still preparing
      // LOCK HERE
      std::lock_guard<std::mutex> values_lk(values_mutex);
      if (propNum < _return.proposalNumber &&
          thisServer->isPreparing) {
        propVals.push_back(_return);
      }
      numPrepareResponses++;
    }
    catch (TException& tx) {
      string what(tx.what());
      thisServer->log("Unable to send prepare to server");
    }
  }

  // Multicast prepare to all connected acceptors
  // wait for response from majority
  void multicastPrepare(const ProposalNumber &propNum, Request &value) {
    int numThreads = (int)thisServer->connectedServers.size();
    int majority = (numThreads / 2) + 1;
    numPrepareResponses = 0;

    // Send prepare on multiple threads
    for (auto server : thisServer->connectedServers) {
      // Make thread fail 10% of the time
      // Don't fail local acceptor
      if (server.first != thisServer->serverAddress && (rand() % 11) == 1) {
        thisServer->log("Acceptor failed on prepare", server.first);
      }
      // Thread did not fail
      else {
        // Start thread
        std::thread t(&KeyValueStoreHandler::prepareThread, this, server.second->client, propNum);
        // Detach because we are only waiting for the majority
        t.detach();
      }
    }
    // wait for responses from majority
    while (numPrepareResponses < majority) {}
    // Select value as value from greatest proposal number
    // Lock
    // in case tardy prepare response tries to add to propVals
    std::lock_guard<std::mutex> values_lk(values_mutex);
    ProposalNumber maxPropNum;
    maxPropNum.roundNumber = 0;
    maxPropNum.serverID = 0;
    for (auto propVal : propVals) {
      if (propVal.proposalNumber > maxPropNum) {
        maxPropNum = propVal.proposalNumber;
        value = propVal.request;
      }
    }
  }

// Accept gets called on all connected acceptors
  void acceptThread(ProposerClient acceptor, ProposalValue propVal) {
    ProposalNumber _return;
    try {
      acceptor.accept(_return, propVal);
      // Lock
      std::lock_guard<std::mutex> nums_lk(nums_mutex);
      thisServer->setMaxRound(_return.roundNumber);
      if (propVal.proposalNumber < _return &&
          thisServer->isAccepting)
      {
        resultNums.push_back(_return);
      }
      numAcceptResponses++;
    }
    catch (TException& tx) {
      string what(tx.what());
      thisServer->log("Unable to send accept to server, " + what);
    }
  }

// Multicast accept to all connected acceptors
// wait for response from majority
  void multicastAccept(ProposalValue &propVal) {
    int numThreads = (int)thisServer->connectedServers.size();
    int majority = (numThreads / 2) + 1;
    numAcceptResponses = 0;
    // Send accept on multiple threads
    for (auto server : thisServer->connectedServers) {
      // Make thread fail 10% of the time
      // Don't fail local acceptor
      if (server.first != thisServer->serverAddress && (rand() % 11) == 1) {
        thisServer->log("Acceptor failed on accept", server.first);
      }
      // Thread did not fail
      else {
        // Start thread
        std::thread t(&KeyValueStoreHandler::acceptThread, this, server.second->client, propVal);
        // Detach because we are only waiting for the majority
        t.detach();
      }
    }
    // wait for responses from majority
    while (numAcceptResponses < majority) {}
  }

// Tell all connected acceptors to perform request
  void multicastCommit(Request& _return, const Request& request) {
    for (auto server : thisServer->connectedServers) {
      Request commitReturn;
      try {
        server.second->client.commit(commitReturn, request);
        // Check if acceptor successfully committed
        if (!commitReturn.successful) {
          thisServer->log("Declined commit from server", server.first);
        }
        // Reply to client with response from this server
        if (server.first == thisServer->serverAddress) {
          _return = commitReturn;
        }
      }
      catch (TException& tx) {
        string what(tx.what());
        thisServer->log("Unable to send commit to server", server.first);
      }
    }
  }

//
  void beginPaxos(Request& _return, const Request& request) {
    // Lock
    // Only initiate one paxos at a time
    std::unique_lock<std::mutex> paxos_lk(paxos_mutex);

    ProposalNumber propNum;
    propNum.roundNumber = thisServer->incrementMaxRound();
    propNum.serverID = thisServer->serverID;
    Request value = request;

    propVals.clear();
    resultNums.clear();

    thisServer->isPreparing = true;
    multicastPrepare(propNum, value);
    thisServer->isAccepting = true;
    thisServer->isPreparing = false;

    ProposalValue propVal;
    propVal.proposalNumber = propNum;
    propVal.request = value;
    multicastAccept(propVal);
    thisServer->isAccepting = false;

    // If we got any proposal numbers greater than the
    // initial proposal number, accept phase failed, so restart
    if (resultNums.size() > 0) {
      // Set maxRound with the highest returned proposal number
      thisServer->setMaxRound(max_element(resultNums.begin(), resultNums.end())->roundNumber);
      // Unlock paxos for next round
      paxos_lk.unlock();
      // Restart
      beginPaxos(_return, value);
    }
    // Value chosen
    else {
      multicastCommit(_return, value);
    }

    // The original request never went through
    if ((_return.op != request.op && _return.key != request.key) ||
        (request.op == Operation::PUT && _return.value != request.value))
    {
      // Unlock paxos for next round
      paxos_lk.unlock();
      // Start again with original request
      beginPaxos(_return, request);
    }

    // Paxos unlocked on destructor
  }

public:
  KeyValueStoreHandler(KeyValueStoreServer* server) {
    thisServer = server;
  }

  void clientRequest(Request& _return, const Request& request) {
    _return.successful = false;
    thisServer->log("RECV:", request);

    beginPaxos(_return, request);

    thisServer->log("SEND:", _return);
  }
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// ProposerHandler
//
// Acceptors handle calls from proposers
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class ProposerHandler : virtual public ProposerIf {
private:
  KeyValueStoreServer* thisServer;
  std::mutex minProp_mutex, commit_mutex;

  void get_request(Request& _return, const Request& request) {
    _return.key = request.key;
    // Find key
    map<string, string>::iterator it = thisServer->hashmap.find(request.key);
    // Not found
    if (it == thisServer->hashmap.end()) {
      _return.comment = "not found";
    }
    // Found, return value
    else {
      _return.value = thisServer->hashmap[request.key];
    }
    _return.successful = true;
  }

  void put_request(Request& _return, const Request& request) {
    // Set key to value
    thisServer->hashmap[request.key] = request.value;
    // Set return value
    _return.key = request.key;
    _return.value = thisServer->hashmap[request.key];
    _return.comment = "added to KeyValueStore";
    _return.successful = true;
  }

  void del_request(Request& _return, const Request& request) {
    // Key is deleted
    _return.key = request.key;
    if (thisServer->hashmap.erase(request.key) > 0) {
      _return.comment = "erased from KeyValueStore";
    }
    // Key is not found
    else {
      _return.comment = "not found in KeyValueStore";
    }
    _return.successful = true;
  }

public:
  ProposerHandler(KeyValueStoreServer* server) {
    thisServer = server;
  }

  // New server sharing contact information with you
  bool hello(const ServerAddress& serverAddress) {
    // Make sure server gave you the right phone
    // number and add to address book
    thisServer->connectToAcceptorNoHello(serverAddress);
    return true;
  }

  // Server wants to end communication
  // Close transport and remove from phone book
  bool goodbye(const ServerAddress& serverAddress) {
    // Make sure this server is in list of connected servers
    auto it = thisServer->connectedServers.find(serverAddress);
    if (it != thisServer->connectedServers.end()) {
      // Close the transport
      try {
        it->second->transport->close();
      } catch (TException& tx) {
        thisServer->log("Closing transport unsuccessful with", it->first);
      }
      // Remove from list of connected servers
      thisServer->connectedServers.erase(it);
    }
    thisServer->log("Ended communication with", it->first);
    return true;
  }

  // How the acceptor handles a prepare
  void prepare(ProposalValue& _return, const ProposalNumber& propNum) {
    _return.proposalNumber = thisServer->acceptedProposal;
    _return.request = thisServer->acceptedValue;

    std::lock_guard<std::mutex> minProp_lk(minProp_mutex);
    if (propNum > thisServer->minProposal) {
      thisServer->minProposal = propNum;
      thisServer->update_storelog();
    }
  }

  // How the acceptor handles an accept
  void accept(ProposalNumber& _return, const ProposalValue& propVal) {
    std::lock_guard<std::mutex> minProp_lk(minProp_mutex);

    // This proposal is >= previous proposals
    if (propVal.proposalNumber > thisServer->minProposal ||
        propVal.proposalNumber == thisServer->minProposal) {
      thisServer->acceptedProposal = propVal.proposalNumber;
      thisServer->minProposal = propVal.proposalNumber;
      thisServer->acceptedValue = propVal.request;
      thisServer->update_storelog();
    }
    _return = thisServer->minProposal;
  }

  // Actually perform a request! About time
  void commit(Request& _return, const Request& request) {
    // Lock
    // Only perform one commit at a time
    std::lock_guard<std::mutex> commit_lk(commit_mutex);

    // Log request
    thisServer->operationlog(request);
    // perform request
    _return.op = request.op;
    if (request.op == Operation::GET) get_request(_return, request);
    else if (request.op == Operation::PUT) put_request(_return, request);
    else if (request.op == Operation::ERASE) del_request(_return, request);

    // End of paxos, clear accepted value and number
    ProposalNumber blankPropposalNumber;
    Request blankRequest;
    thisServer->acceptedProposal = blankPropposalNumber;
    thisServer->acceptedValue = blankRequest;
    thisServer->update_storelog();
  }

};

//**********************************************************
// checkArgs
//
// validates arguments
//**********************************************************
void checkArgs(int argc, char **argv, int &port, struct hostent *&server)
{
  if (argc < 3) {
    error("please use arguments: ./Server hostname port\nhostname is IP address of this server");
  }

  port = atoi(argv[2]);
  if (port < 2000 || port > 65535) {
    error("please use a port number between 2000 and 65535");
  }

  server = gethostbyname(argv[1]);
  if (server == NULL) {
    error("no such host");
  }
}

//**********************************************************
// Main
//**********************************************************
int main(int argc, char *argv[]) {

  srand(time(NULL));

  struct hostent *host;
  int port;
  KeyValueStoreServer* thisServer;

  // Make sure valid hostname and port
  checkArgs(argc, argv, port, host);

  thisServer = new KeyValueStoreServer(port, host->h_name);

  // Handlers
  boost::shared_ptr<KeyValueStoreHandler> kvs_handler(new KeyValueStoreHandler(thisServer));
  boost::shared_ptr<AdministratorHandler> admin_handler(new AdministratorHandler(thisServer));
  boost::shared_ptr<ProposerHandler> proposer_handler(new ProposerHandler(thisServer));

  // Processors
  boost::shared_ptr<TMultiplexedProcessor> processor(new TMultiplexedProcessor());
  boost::shared_ptr<TProcessor> kvs_processor(new KeyValueStoreProcessor(kvs_handler));
  boost::shared_ptr<TProcessor> admin_processor(new AdministratorProcessor(admin_handler));
  boost::shared_ptr<TProcessor> proposer_processor(new ProposerProcessor(proposer_handler));

  // Add all the services!!!
  processor->registerProcessor("KeyValueStore", kvs_processor);
  processor->registerProcessor("Administrator", admin_processor);
  processor->registerProcessor("Proposer", proposer_processor);

  boost::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  boost::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
  boost::shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(4);
  boost::shared_ptr<PosixThreadFactory> threadFactory = boost::shared_ptr<PosixThreadFactory> (new PosixThreadFactory());
  threadManager->threadFactory(threadFactory);
  threadManager->start();

  TThreadedServer server(processor, serverTransport, transportFactory, protocolFactory);

  cout << "Server started, waiting for clients..." << endl;
  server.serve();
  cout << "Done." << endl;
  return 0;
}