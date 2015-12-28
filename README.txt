BASIC PAXOS with multiplexed Apache Thrift services
Trevor Meiss
12/15/2015

Ｐｒｏｃｅｓｓ  Ｏｖｅｒｖｉｅｗ
───────────────────────────────────────────────────────────────────────

1. Start x Server(s)
2. Admin into each server to set ID and connect to PAXOS
4. Connect client(s) to any server
5. Send client request

Ｄｅｓｉｇｎ  Ｏｖｅｒｖｉｅｗ
───────────────────────────────────────────────────────────────────────

	┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	┃ Server                            ┃
	┡━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┩
	│ - int ServerID                    │
	│ - int maxRound                    │
	│                                   │
	│ - KeyValueStoreClient[] proposer  │
	│ - ProposerClient[] acceptors      │
	│                                   │
	│ - minProposal                     │
	│ - acceptedProposal                │
	│ - acceptedValue                   │
	│                                   │
	│  ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓  │
	│  ┃ KeyValueStore               ┃  │ ◁─ Client
	│  ┡━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┩  │
	│  │ + clientRequest()           │  │ ─▷ proposer
	│  ╰─────────────────────────────╯  │
	│                                   │
	│  ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓  │
	│  ┃ Administrator               ┃  │ ◁─ Admin
	│  ┡━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┩  │
	│  │ + setID(ID)                 │  │
	│  │ + connectToPaxos(Server)    │  │ ─▷ Admin of Server specified
	|  │ + disconnectFromPaxos()     │  │ ─▷ All acceptors
	│  │ + getConnectedServers()     │  │
	│  ╰─────────────────────────────╯  │
	│                                   │
	│  ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓  │
	│  ┃ Proposer                    ┃  │ ◁─ KeyValueStore
	│  ┡━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┩  │
	│  │ + prepare(proposalNumber)   │  │ ─▷ All acceptors
	│  │ + accept(proposalValue)     │  │ ─▷ All acceptors
	│  │ + commit(request)           │  │ ─▷ All acceptors	
	│  ╰─────────────────────────────╯  │
	│                                   │
	╰───────────────────────────────────╯

All servers are connected to all other servers in PAXOS
- Proposers are KeyValueStoreHandlers. The receive requests from clients
  and multicast to all acceptors.
- Acceptors are ProposerHandlers. They handle prepare, accept, and
  commit calls from the proposer.
- Proposers also act as learners. If they don't receive any higher
  proposal numbers from accept calls, they tell all servers to 
  commit the request. The return value from the local server is
  returned to the client.

Acceptor Failing
- The acceptor threads are programmed to fail 10% of the time when
  multicasting prepare and accept. This is can be seen at Server.cpp
  line 439 and 498.
- When an acceptor fails, it is logged in the status log of the server
  that was proposing.
- There is a small chance that more than the majority would fail,
  which would break the system.

Logs
- Each server has three separate logs
	1. Status: For the general status of the server. Logs when
	   connections are made or fails and when acceptor thread fail.
	2. Operation: Logs all operations that have been committed to the
	   key-value store. It includes the proposal number for each
	   operation and should be the same for all servers.
	3. Store: Logs the max round, minimum proposal, accepted value and
	   accepted proposal number in case the server crashes.

Shortcomings
- No liveliness

Future Directions
- Multi-Paxos


Ｂｕｉｌｄ
───────────────────────────────────────────────────────────────────────

1. Install Apache Thrift
    - on OSX, recommend homebrew
2. Navigate to src/
3. $ make


Ｒｕｎ
───────────────────────────────────────────────────────────────────────

1. Start the servers in separate terminals/clusters
    $ ./Server IP PORT
    - IP is the IP where the server is hosted (used for
    internal communication)

2. Connect to each server with admin and connect to PAXOS
    a. connect admin to each server
        $ ./Admin IP PORT
    b. set server ID (unique for each server)
        $ s 1
    c. connect to PAXOS
        $ c IP PORT
        - IP and PORT of any server currently connected to PAXOS
        - if this is the first server, just put own IP and PORT
    d. list connected servers for sanity check
        $ ls
    e. quit admin
        $ q

3. Connect client(s) to any server
    $ ./Client IP PORT


A shell script is included to automate KeyValueStore insertion
After starting the servers and connecting them with Admin
1. Modify IP/PORTS in sim_client.sh
2. $ chmod +x sim_client.sh
3. $ ./sim_client.sh


Ｅｘｅｃｕｔｉｖｅ  Ｓｕｍｍａｒｙ
───────────────────────────────────────────────────────────────────────

Assignment overview

    The purpose of this assignment was to implement replicated servers
    using PAXOS. This is an improvement on the previous project that
    used two-phase commits in that having one or two servers crash will
    not bring down the entire system. To accomplish this with c++,
    Apache Thrift was utilized. Each server has three services that
    are multiplexed together. The Administrator service is where an
    admin can set the ID of a server and connect it to PAXOS. To give
    one server a higher priority, a higher ID can be set. The admin
    can also disconnect the server from PAXOS and list the servers
    that are connected. The KeyValueStore service is where a client
    can make a request to put, get, or delete from the replicated
    key-value store. The Proposer service is how a proposer communicates
    with all of the acceptors in PAXOS. First the proposer generates a
    new proposal number and multicasts a prepare message. Then the
    proposer multicasts an accept message. If all of the acceptors
    accept this message by not sending back a higher proposal number,
    the proposer then tells all the servers to commit the request.
    The proposer only needs a majority of servers to respond to the
    prepare and accept messages. To test fault-tolerance, the prepare
    and accept messages were programmed so that 10% of them would
    not go through. This was accomplished with a random number
    generator. Each server also saved the critical information to a
    log any time it was updated. This includes the minimum proposal
    number, accepted proposal number, and accepted value. If a server
    crashes, these values can be read once it restarts. Additionally,
    another log was used to store every operation that was committed
    and the proposal number that went with it. This is to allow a
    failed server to catch back up to the other servers.

Technical impression

	This project took a tremendous amount of planning. One of the
	difficulties was that there are many implementations of PAXOS
	and the requirements of this project were fairly vague. For
	example, I wasn't sure what it meant to have an acceptor failing.
	I think my implementation satisfies this requirement in that
	the connection is not made from the proposer to an acceptor
	on roughly 10% of the attempts. It would have been nice to have
	an example of what a log should look like. Because I wasn't sure,
	I decided to make 3 separate logs for each server, each serving
	their own purpose. It was also difficult to test my implementation.
	In the future, it would be nice to have some sort of testing suite
	to hook our projects up to. I designed my project to be extensible
	instead of hard-coding the number of servers and their connections.
	This took another large amount of planning and time to implement it
	but I believe it was worth it to design the servers with the
	ability to access their backend with an administrator interface.
	While currently only the acceptor connections are programmed to
	fail, it would be fairly straightforward to fail an entire server
	and have it recover.


Ｔｈｒｉｆｔ  Ｍｏｄｉｆｉｃａｔｉｏｎｓ
───────────────────────────────────────────────────────────────────────
keyvaluestore_types.h

  bool operator < (const ServerAddress &rhs) const {
    return this->hostname < rhs.hostname ||
           (this->hostname == rhs.hostname && this->port < rhs.port);
  }

  bool operator < (const ProposalNumber &rhs) const {
    return this->roundNumber < rhs.roundNumber ||
           (this->roundNumber == rhs.roundNumber &&
            this->serverID < rhs.serverID);
  }
  bool operator > (const ProposalNumber &rhs) const {
    return this->roundNumber > rhs.roundNumber ||
           (this->roundNumber == rhs.roundNumber &&
            this->serverID > rhs.serverID);
  }

  bool operator < (const ProposalValue &rhs) const {
    return this->proposalNumber.roundNumber < rhs.proposalNumber.roundNumber ||
           (this->proposalNumber.roundNumber == rhs.proposalNumber.roundNumber &&
            this->proposalNumber.serverID < rhs.proposalNumber.serverID);
  }
  bool operator > (const ProposalValue &rhs) const {
    return this->proposalNumber.roundNumber > rhs.proposalNumber.roundNumber ||
           (this->proposalNumber.roundNumber == rhs.proposalNumber.roundNumber &&
            this->proposalNumber.serverID > rhs.proposalNumber.serverID);
  }