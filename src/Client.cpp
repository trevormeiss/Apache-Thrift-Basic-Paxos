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
#include <vector>
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

#include "KeyValueStore.h"
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
  ofstream client_log;
  client_log.open("../Logs/client_log.txt", ios_base::app);

  client_log << systemTime() << " " << msg << "\n";
}

void log(string what, Request &request) {
  string operation;
  if (request.op == Operation::PUT) {
    operation = "PUT";
    what += " " + operation + "(" + request.key + "," + request.value + ")";
  }
  else {
    if (request.op == Operation::GET) {
      operation = "GET";
    }
    else if (request.op == Operation::ERASE) {
      operation = "DEL";
    }
    what += " " + operation + "(" + request.key + ")";
  }

  log(what);
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
// checkArgs
//
// validates arguments
//**********************************************************
void checkArgs(int argc, char **argv, int &port, struct hostent *&server)
{
  if (argc < 3) {
    error("please use arguments: ./Client hostname port");
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
// checkInput
//
// validates command to send to server
//
// Return: string result if invalid command
//**********************************************************
string checkInput(string input, KeyValueStoreClient &client)
{
  int comma_pos = (int)input.find(",");
  int paren_pos = (int)input.find(")");
  string result = "";
  Request request;
  Request _return;

  // Universal checks for all operations
  if (input.length() <= 5)
    result = "Invalid input. Length must be great than 5 to be valid.";
  if (paren_pos == string::npos)
    result = "Invalid input. Missing closing parenthesis";
  else if ((paren_pos + 1) < input.length())
    result = "Invalid input. Character(s) found after closing parenthesis";

  // PUT command
  else if (compareStringNoCase(input, 0, 4, "put("))
  {
    request.op = Operation::PUT;
    request.key = input.substr(4, comma_pos - 4);
    request.value = input.substr(comma_pos + 1, paren_pos - comma_pos - 1);

    if (comma_pos == string::npos)
      result = "Invalid PUT. No comma found";
    else if (!stringIsValid(request.key) || request.key.length() < 1)
      result = "Invalid PUT. Key \"" + request.key + "\" is not alphanumeric";
    else if (!stringIsValid(request.value) || request.value.length() < 1)
      result = "Invalid PUT. Value \"" + request.value + "\" is not alphanumeric";
    else
      // Valid PUT request
      try {
        log("Send:", request);
        client.clientRequest(_return, request);
        result = "Recv: PUT key:" + _return.key + " value:" + _return.value + " " + _return.comment;
      } catch (InvalidOperation& io) {
        result = "InvalidOperation: " + io.why;
      }
  }

  // GET command
  else if (compareStringNoCase(input, 0, 4, "get("))
  {
    request.op = Operation::GET;
    request.key = input.substr(4, paren_pos - 4);
    if (!stringIsValid(request.key) || request.key.length() < 1)
      result = "Invalid GET/DEL. Key \"" + request.key + "\" is not alphanumeric";
    else
      // Valid GET request
      try {
        log("Send:", request);
        client.clientRequest(_return, request);
        if (_return.value != "") {
          result = "Recv: GET key:" + _return.key + " value:" + _return.value;
        }
        else {
          result = "Recv: GET key:" + _return.key + " " + _return.comment;
        }
      } catch (InvalidOperation& io) {
        result = "InvalidOperation: " + io.why;
      }
  }

  // DEL command
  else if (compareStringNoCase(input, 0, 4, "del("))
  {
    request.op = Operation::ERASE;
    request.key = input.substr(4, paren_pos - 4);
    if (!stringIsValid(request.key) || request.key.length() < 1)
      result = "Invalid GET/DEL. Key \"" + request.key + "\" is not alphanumeric";
    else
      // Valid DELETE request
      try {
        log("SEND:", request);
        client.clientRequest(_return, request);
        result = "Recv: DEL key:" + _return.key + " " + _return.comment;
      } catch (InvalidOperation& io) {
        result = "InvalidOperation: " + io.why;
      }
  }
  return result;
}

//**********************************************************
// sender
//
// Recieves command from user
// Sends message to server
//
// Return: -1 if server disconnect or quit
//**********************************************************
int sender(KeyValueStoreClient &client)
{
  char buffer[256];
  int result = 0;
  string input_result;

  // Receive command from user
  printf("Enter command: ");
  bzero(buffer, 256);
  fgets(buffer, 255, stdin);

  // Remove the newline from command input
  if (buffer[strlen(buffer) - 1] == '\n')
    buffer[strlen(buffer) - 1] = '\0';

  // User quit, notify server and listen for response
  if (tolower(buffer[0]) == 'q')
  {
    log("User quit");
    result = -1;
  }
  // User did not quit, check input and send request
  else {
    input_result = checkInput(string(buffer), client);
    if (input_result == "")
    {
      log("Invalid input. No operation match");
      cout << "Invalid input. Viable commands are:\nput(key,value)\nget(key)\ndel(key)\nq quit\n";
    }
    else
    {
      log(input_result);
      cout << input_result << "\n";
    }
  }

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
  boost::shared_ptr<TProtocol> mp(new TMultiplexedProtocol(protocol, "KeyValueStore"));
  KeyValueStoreClient client(mp);

  try {
    transport->open();

    //client.ping();
    printf("Client connected to Key-Value Store server\nViable commands are:\nput(key,value)\nget(key)\ndel(key)\nq quit\n");

    // Continue accepting commands until user quit
    // Result = -1 if user quit
    while (result >= 0)
    {
      result = sender(client);
    }

    transport->close();
  } catch (TException& tx) {
    string what(tx.what());
    error("server unresponsive, " + what);
  }
}