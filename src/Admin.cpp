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

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TMultiplexedProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include <boost/algorithm/string.hpp>

#include "Administrator.h"
#include "helper.hpp"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace keyvaluestore;
using namespace std;

//**********************************************************
// log
//
// logs messages
//**********************************************************
void log(string msg)
{
  // Open log file to write
  ofstream admin_log;
  admin_log.open("../Logs/admin_log.txt", ios_base::app);

  admin_log << systemTime() << " " << msg << "\n";
}

//**********************************************************
// error
//
// logs errors and quits
//**********************************************************
void error(string msg)
{
  log("ERROR: " + msg);
  fprintf(stderr, "%s ERROR: %s\n\n", systemTime().c_str(), msg.c_str());
  exit(1);
}

//**********************************************************
// Split string into vector
// http://stackoverflow.com/a/236803
//**********************************************************
vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

//**********************************************************
// checkArgs
//
// validates arguments
//**********************************************************
void checkArgs(int argc, char **argv, int &port, struct hostent *&server)
{
  if (argc < 3) {
    error("please use arguments: ./Admin hostname port");
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
// sender
//
// Recieves command from user
// Sends message to server
//
// Return: -1 if server disconnect or quit
//**********************************************************
int sender(AdministratorClient &admin)
{
  int result = 0;
  string input;
  string r;

  // Receive command from user
  printf("Enter command: ");
  getline(cin, input);

  vector<string> splitInput = split(input, ' ');

  // Admin quit
  if (boost::iequals(splitInput[0], "q")) {
    r = "Admin quit";
    result = -1;
  }
  // Set server ID
  else if (boost::iequals(splitInput[0], "s")) {
    log("Set server ID");
    if (admin.setID(stoi(splitInput[1]))) {
      r = "Successfully set server ID as " + splitInput[1];
    }
    else {
      r = "Unable to set server ID as " + splitInput[1];
    }
  }
  // List connected servers
  else if (boost::iequals(splitInput[0], "ls")) {
    set<ServerAddress> connectedServers;
    admin.getConnectedServers(connectedServers);
    for (auto server : connectedServers) {
      cout << server.hostname << ":" << server.port << "\n";
    }
    r = "Listed connected servers";
  }
  // Disconnect from PAXOS
  else if (boost::iequals(splitInput[0], "d")) {
    if (admin.disconnectFromPaxos()) {
      r = "Disconnected from PAXOS";
    }
    else {
      r = "Unable to disconnect from PAXOS";
    }
  }
  // Connect to PAXOS
  else if (splitInput.size() >= 3 && boost::iequals(splitInput[0], "c")) {
    ServerAddress server;
    server.hostname = splitInput[1];
    server.port = stoi(splitInput[2]);

    if (admin.connectToPaxos(server)) {
      r = "Successfully connected to PAXOS";
    }
    else {
      r = "Unable to connect to PAXOS";
    }
  }
  log(r);
  cout << r << "\n";
  return result;
}

//**********************************************************
// main
//
// starts connection with host and port specified
// in command-line arguments
//**********************************************************
int main(int argc, char *argv[])
{
  struct hostent *server;
  int port;
  int result = 0;

  // Make sure the host and port are valid
  checkArgs(argc, argv, port, server);

  boost::shared_ptr<TTransport> socket(new TSocket(server->h_name, port));
  boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  boost::shared_ptr<TProtocol> mp(new TMultiplexedProtocol(protocol, "Administrator"));
  AdministratorClient admin(mp);

  try {
    transport->open();

    log("Admin connected to server");
    printf("Admin connected to Key-Value Store server"
      "\nViable commands are:"
      "\ns serverID:\t\tset server ID"
      "\nls:\t\tlist connected servers"
      "\nc IP PORT:\tconnect to server in PAXOS"
      "\nd:\t\tdisconnect from PAXOS"
      "\nq:\t\tquit\n");

    // Continue accepting commands until user quit
    // Result = -1 if user quit
    while (result >= 0)
    {
      result = sender(admin);
    }

    transport->close();
  } catch (TException& tx) {
    string what(tx.what());
    error("server unresponsive, " + what);
  }
}