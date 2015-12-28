# Apache-Thrift-Basic-Paxos
An implementation of basic PAXOS using multiplexed Apache Thrift services. Allows for multi-threaded client requests to a replicated key-value store.

Process Overview

1. Start x Server(s)
2. Admin into each server to set ID and connect to PAXOS
4. Connect client(s) to any server
5. Send client request

Design Overview

| Server |
| ------ |
|- int serverID | 
|- int maxRound | 
|- proposer | 
|- acceptor | 
|- minProposal | 
|- acceptedProposal | 
|- acceptedValue | 

| KeyValueStore | 
| ------------- |
| - clientRequest(request) |

| Administrator |
| ------------ |
| - setID(ID) |
| - connectToPaxos(serverAddress) |
| - disconnectFromPaxos() |
| - getConnectedServers() |

| Proposer |
| --------- |
| - prepare() |
| - accept() |
| - commit() |

