/**
 * Autogenerated by Thrift Compiler (0.9.3)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef Administrator_H
#define Administrator_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "keyvaluestore_types.h"

namespace keyvaluestore {

#ifdef _WIN32
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class AdministratorIf {
 public:
  virtual ~AdministratorIf() {}
  virtual bool setID(const int16_t id) = 0;
  virtual bool connectToPaxos(const ServerAddress& serverAddress) = 0;
  virtual bool disconnectFromPaxos() = 0;
  virtual void getConnectedServers(std::set<ServerAddress> & _return) = 0;
};

class AdministratorIfFactory {
 public:
  typedef AdministratorIf Handler;

  virtual ~AdministratorIfFactory() {}

  virtual AdministratorIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(AdministratorIf* /* handler */) = 0;
};

class AdministratorIfSingletonFactory : virtual public AdministratorIfFactory {
 public:
  AdministratorIfSingletonFactory(const boost::shared_ptr<AdministratorIf>& iface) : iface_(iface) {}
  virtual ~AdministratorIfSingletonFactory() {}

  virtual AdministratorIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(AdministratorIf* /* handler */) {}

 protected:
  boost::shared_ptr<AdministratorIf> iface_;
};

class AdministratorNull : virtual public AdministratorIf {
 public:
  virtual ~AdministratorNull() {}
  bool setID(const int16_t /* id */) {
    bool _return = false;
    return _return;
  }
  bool connectToPaxos(const ServerAddress& /* serverAddress */) {
    bool _return = false;
    return _return;
  }
  bool disconnectFromPaxos() {
    bool _return = false;
    return _return;
  }
  void getConnectedServers(std::set<ServerAddress> & /* _return */) {
    return;
  }
};

typedef struct _Administrator_setID_args__isset {
  _Administrator_setID_args__isset() : id(false) {}
  bool id :1;
} _Administrator_setID_args__isset;

class Administrator_setID_args {
 public:

  Administrator_setID_args(const Administrator_setID_args&);
  Administrator_setID_args& operator=(const Administrator_setID_args&);
  Administrator_setID_args() : id(0) {
  }

  virtual ~Administrator_setID_args() throw();
  int16_t id;

  _Administrator_setID_args__isset __isset;

  void __set_id(const int16_t val);

  bool operator == (const Administrator_setID_args & rhs) const
  {
    if (!(id == rhs.id))
      return false;
    return true;
  }
  bool operator != (const Administrator_setID_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Administrator_setID_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Administrator_setID_pargs {
 public:


  virtual ~Administrator_setID_pargs() throw();
  const int16_t* id;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Administrator_setID_result__isset {
  _Administrator_setID_result__isset() : success(false) {}
  bool success :1;
} _Administrator_setID_result__isset;

class Administrator_setID_result {
 public:

  Administrator_setID_result(const Administrator_setID_result&);
  Administrator_setID_result& operator=(const Administrator_setID_result&);
  Administrator_setID_result() : success(0) {
  }

  virtual ~Administrator_setID_result() throw();
  bool success;

  _Administrator_setID_result__isset __isset;

  void __set_success(const bool val);

  bool operator == (const Administrator_setID_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const Administrator_setID_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Administrator_setID_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Administrator_setID_presult__isset {
  _Administrator_setID_presult__isset() : success(false) {}
  bool success :1;
} _Administrator_setID_presult__isset;

class Administrator_setID_presult {
 public:


  virtual ~Administrator_setID_presult() throw();
  bool* success;

  _Administrator_setID_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _Administrator_connectToPaxos_args__isset {
  _Administrator_connectToPaxos_args__isset() : serverAddress(false) {}
  bool serverAddress :1;
} _Administrator_connectToPaxos_args__isset;

class Administrator_connectToPaxos_args {
 public:

  Administrator_connectToPaxos_args(const Administrator_connectToPaxos_args&);
  Administrator_connectToPaxos_args& operator=(const Administrator_connectToPaxos_args&);
  Administrator_connectToPaxos_args() {
  }

  virtual ~Administrator_connectToPaxos_args() throw();
  ServerAddress serverAddress;

  _Administrator_connectToPaxos_args__isset __isset;

  void __set_serverAddress(const ServerAddress& val);

  bool operator == (const Administrator_connectToPaxos_args & rhs) const
  {
    if (!(serverAddress == rhs.serverAddress))
      return false;
    return true;
  }
  bool operator != (const Administrator_connectToPaxos_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Administrator_connectToPaxos_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Administrator_connectToPaxos_pargs {
 public:


  virtual ~Administrator_connectToPaxos_pargs() throw();
  const ServerAddress* serverAddress;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Administrator_connectToPaxos_result__isset {
  _Administrator_connectToPaxos_result__isset() : success(false) {}
  bool success :1;
} _Administrator_connectToPaxos_result__isset;

class Administrator_connectToPaxos_result {
 public:

  Administrator_connectToPaxos_result(const Administrator_connectToPaxos_result&);
  Administrator_connectToPaxos_result& operator=(const Administrator_connectToPaxos_result&);
  Administrator_connectToPaxos_result() : success(0) {
  }

  virtual ~Administrator_connectToPaxos_result() throw();
  bool success;

  _Administrator_connectToPaxos_result__isset __isset;

  void __set_success(const bool val);

  bool operator == (const Administrator_connectToPaxos_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const Administrator_connectToPaxos_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Administrator_connectToPaxos_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Administrator_connectToPaxos_presult__isset {
  _Administrator_connectToPaxos_presult__isset() : success(false) {}
  bool success :1;
} _Administrator_connectToPaxos_presult__isset;

class Administrator_connectToPaxos_presult {
 public:


  virtual ~Administrator_connectToPaxos_presult() throw();
  bool* success;

  _Administrator_connectToPaxos_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};


class Administrator_disconnectFromPaxos_args {
 public:

  Administrator_disconnectFromPaxos_args(const Administrator_disconnectFromPaxos_args&);
  Administrator_disconnectFromPaxos_args& operator=(const Administrator_disconnectFromPaxos_args&);
  Administrator_disconnectFromPaxos_args() {
  }

  virtual ~Administrator_disconnectFromPaxos_args() throw();

  bool operator == (const Administrator_disconnectFromPaxos_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const Administrator_disconnectFromPaxos_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Administrator_disconnectFromPaxos_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Administrator_disconnectFromPaxos_pargs {
 public:


  virtual ~Administrator_disconnectFromPaxos_pargs() throw();

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Administrator_disconnectFromPaxos_result__isset {
  _Administrator_disconnectFromPaxos_result__isset() : success(false) {}
  bool success :1;
} _Administrator_disconnectFromPaxos_result__isset;

class Administrator_disconnectFromPaxos_result {
 public:

  Administrator_disconnectFromPaxos_result(const Administrator_disconnectFromPaxos_result&);
  Administrator_disconnectFromPaxos_result& operator=(const Administrator_disconnectFromPaxos_result&);
  Administrator_disconnectFromPaxos_result() : success(0) {
  }

  virtual ~Administrator_disconnectFromPaxos_result() throw();
  bool success;

  _Administrator_disconnectFromPaxos_result__isset __isset;

  void __set_success(const bool val);

  bool operator == (const Administrator_disconnectFromPaxos_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const Administrator_disconnectFromPaxos_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Administrator_disconnectFromPaxos_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Administrator_disconnectFromPaxos_presult__isset {
  _Administrator_disconnectFromPaxos_presult__isset() : success(false) {}
  bool success :1;
} _Administrator_disconnectFromPaxos_presult__isset;

class Administrator_disconnectFromPaxos_presult {
 public:


  virtual ~Administrator_disconnectFromPaxos_presult() throw();
  bool* success;

  _Administrator_disconnectFromPaxos_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};


class Administrator_getConnectedServers_args {
 public:

  Administrator_getConnectedServers_args(const Administrator_getConnectedServers_args&);
  Administrator_getConnectedServers_args& operator=(const Administrator_getConnectedServers_args&);
  Administrator_getConnectedServers_args() {
  }

  virtual ~Administrator_getConnectedServers_args() throw();

  bool operator == (const Administrator_getConnectedServers_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const Administrator_getConnectedServers_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Administrator_getConnectedServers_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Administrator_getConnectedServers_pargs {
 public:


  virtual ~Administrator_getConnectedServers_pargs() throw();

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Administrator_getConnectedServers_result__isset {
  _Administrator_getConnectedServers_result__isset() : success(false) {}
  bool success :1;
} _Administrator_getConnectedServers_result__isset;

class Administrator_getConnectedServers_result {
 public:

  Administrator_getConnectedServers_result(const Administrator_getConnectedServers_result&);
  Administrator_getConnectedServers_result& operator=(const Administrator_getConnectedServers_result&);
  Administrator_getConnectedServers_result() {
  }

  virtual ~Administrator_getConnectedServers_result() throw();
  std::set<ServerAddress>  success;

  _Administrator_getConnectedServers_result__isset __isset;

  void __set_success(const std::set<ServerAddress> & val);

  bool operator == (const Administrator_getConnectedServers_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const Administrator_getConnectedServers_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Administrator_getConnectedServers_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Administrator_getConnectedServers_presult__isset {
  _Administrator_getConnectedServers_presult__isset() : success(false) {}
  bool success :1;
} _Administrator_getConnectedServers_presult__isset;

class Administrator_getConnectedServers_presult {
 public:


  virtual ~Administrator_getConnectedServers_presult() throw();
  std::set<ServerAddress> * success;

  _Administrator_getConnectedServers_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class AdministratorClient : virtual public AdministratorIf {
 public:
  AdministratorClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  AdministratorClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  bool setID(const int16_t id);
  void send_setID(const int16_t id);
  bool recv_setID();
  bool connectToPaxos(const ServerAddress& serverAddress);
  void send_connectToPaxos(const ServerAddress& serverAddress);
  bool recv_connectToPaxos();
  bool disconnectFromPaxos();
  void send_disconnectFromPaxos();
  bool recv_disconnectFromPaxos();
  void getConnectedServers(std::set<ServerAddress> & _return);
  void send_getConnectedServers();
  void recv_getConnectedServers(std::set<ServerAddress> & _return);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class AdministratorProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<AdministratorIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (AdministratorProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_setID(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_connectToPaxos(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_disconnectFromPaxos(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_getConnectedServers(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  AdministratorProcessor(boost::shared_ptr<AdministratorIf> iface) :
    iface_(iface) {
    processMap_["setID"] = &AdministratorProcessor::process_setID;
    processMap_["connectToPaxos"] = &AdministratorProcessor::process_connectToPaxos;
    processMap_["disconnectFromPaxos"] = &AdministratorProcessor::process_disconnectFromPaxos;
    processMap_["getConnectedServers"] = &AdministratorProcessor::process_getConnectedServers;
  }

  virtual ~AdministratorProcessor() {}
};

class AdministratorProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  AdministratorProcessorFactory(const ::boost::shared_ptr< AdministratorIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< AdministratorIfFactory > handlerFactory_;
};

class AdministratorMultiface : virtual public AdministratorIf {
 public:
  AdministratorMultiface(std::vector<boost::shared_ptr<AdministratorIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~AdministratorMultiface() {}
 protected:
  std::vector<boost::shared_ptr<AdministratorIf> > ifaces_;
  AdministratorMultiface() {}
  void add(boost::shared_ptr<AdministratorIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  bool setID(const int16_t id) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->setID(id);
    }
    return ifaces_[i]->setID(id);
  }

  bool connectToPaxos(const ServerAddress& serverAddress) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->connectToPaxos(serverAddress);
    }
    return ifaces_[i]->connectToPaxos(serverAddress);
  }

  bool disconnectFromPaxos() {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->disconnectFromPaxos();
    }
    return ifaces_[i]->disconnectFromPaxos();
  }

  void getConnectedServers(std::set<ServerAddress> & _return) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->getConnectedServers(_return);
    }
    ifaces_[i]->getConnectedServers(_return);
    return;
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class AdministratorConcurrentClient : virtual public AdministratorIf {
 public:
  AdministratorConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  AdministratorConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  bool setID(const int16_t id);
  int32_t send_setID(const int16_t id);
  bool recv_setID(const int32_t seqid);
  bool connectToPaxos(const ServerAddress& serverAddress);
  int32_t send_connectToPaxos(const ServerAddress& serverAddress);
  bool recv_connectToPaxos(const int32_t seqid);
  bool disconnectFromPaxos();
  int32_t send_disconnectFromPaxos();
  bool recv_disconnectFromPaxos(const int32_t seqid);
  void getConnectedServers(std::set<ServerAddress> & _return);
  int32_t send_getConnectedServers();
  void recv_getConnectedServers(std::set<ServerAddress> & _return, const int32_t seqid);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
  ::apache::thrift::async::TConcurrentClientSyncInfo sync_;
};

#ifdef _WIN32
  #pragma warning( pop )
#endif

} // namespace

#endif
