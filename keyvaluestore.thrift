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

namespace cpp keyvaluestore

enum Operation {
  PUT,
  GET,
  ERASE
}

struct ProposalNumber {
  1: i64 roundNumber,
  2: i16 serverID
}

struct Request {
  1: Operation op,
  2: string key,
  3: string value,
  4: string comment,
  5: bool successful
}

struct ProposalValue {
  1: ProposalNumber proposalNumber,
  2: Request request
}

struct ServerAddress {
  1: i16 port,
  2: string hostname,
}

exception InvalidOperation {
  1: i32 whatOp,
  2: string why
}

/*****************
 * Services
 *****************/
service KeyValueStore {
  Request clientRequest(1:Request request) throws (1:InvalidOperation invalidOp)
}

service Administrator {
  bool setID(1:i16 id),
  bool connectToPaxos(1:ServerAddress serverAddress),
  bool disconnectFromPaxos(),
  set<ServerAddress> getConnectedServers()
}

service Proposer {
  bool hello(1:ServerAddress serverAddress),
  bool goodbye(1:ServerAddress serverAddress),
  ProposalValue prepare(1:ProposalNumber propNum) throws (1:InvalidOperation invalidOp),
  ProposalNumber accept(1:ProposalValue propVal) throws (1:InvalidOperation invalidOp),
  Request commit(1:Request request) throws (1:InvalidOperation invalidOp)
}