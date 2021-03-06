/**
 * Autogenerated by Thrift Compiler (0.9.3)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef KeyValueStore_H
#define KeyValueStore_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "keyvaluestore_types.h"

namespace keyvaluestore {

#ifdef _WIN32
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class KeyValueStoreIf {
 public:
  virtual ~KeyValueStoreIf() {}
  virtual void clientRequest(Request& _return, const Request& request) = 0;
};

class KeyValueStoreIfFactory {
 public:
  typedef KeyValueStoreIf Handler;

  virtual ~KeyValueStoreIfFactory() {}

  virtual KeyValueStoreIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(KeyValueStoreIf* /* handler */) = 0;
};

class KeyValueStoreIfSingletonFactory : virtual public KeyValueStoreIfFactory {
 public:
  KeyValueStoreIfSingletonFactory(const boost::shared_ptr<KeyValueStoreIf>& iface) : iface_(iface) {}
  virtual ~KeyValueStoreIfSingletonFactory() {}

  virtual KeyValueStoreIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(KeyValueStoreIf* /* handler */) {}

 protected:
  boost::shared_ptr<KeyValueStoreIf> iface_;
};

class KeyValueStoreNull : virtual public KeyValueStoreIf {
 public:
  virtual ~KeyValueStoreNull() {}
  void clientRequest(Request& /* _return */, const Request& /* request */) {
    return;
  }
};

typedef struct _KeyValueStore_clientRequest_args__isset {
  _KeyValueStore_clientRequest_args__isset() : request(false) {}
  bool request :1;
} _KeyValueStore_clientRequest_args__isset;

class KeyValueStore_clientRequest_args {
 public:

  KeyValueStore_clientRequest_args(const KeyValueStore_clientRequest_args&);
  KeyValueStore_clientRequest_args& operator=(const KeyValueStore_clientRequest_args&);
  KeyValueStore_clientRequest_args() {
  }

  virtual ~KeyValueStore_clientRequest_args() throw();
  Request request;

  _KeyValueStore_clientRequest_args__isset __isset;

  void __set_request(const Request& val);

  bool operator == (const KeyValueStore_clientRequest_args & rhs) const
  {
    if (!(request == rhs.request))
      return false;
    return true;
  }
  bool operator != (const KeyValueStore_clientRequest_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const KeyValueStore_clientRequest_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class KeyValueStore_clientRequest_pargs {
 public:


  virtual ~KeyValueStore_clientRequest_pargs() throw();
  const Request* request;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _KeyValueStore_clientRequest_result__isset {
  _KeyValueStore_clientRequest_result__isset() : success(false), invalidOp(false) {}
  bool success :1;
  bool invalidOp :1;
} _KeyValueStore_clientRequest_result__isset;

class KeyValueStore_clientRequest_result {
 public:

  KeyValueStore_clientRequest_result(const KeyValueStore_clientRequest_result&);
  KeyValueStore_clientRequest_result& operator=(const KeyValueStore_clientRequest_result&);
  KeyValueStore_clientRequest_result() {
  }

  virtual ~KeyValueStore_clientRequest_result() throw();
  Request success;
  InvalidOperation invalidOp;

  _KeyValueStore_clientRequest_result__isset __isset;

  void __set_success(const Request& val);

  void __set_invalidOp(const InvalidOperation& val);

  bool operator == (const KeyValueStore_clientRequest_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    if (!(invalidOp == rhs.invalidOp))
      return false;
    return true;
  }
  bool operator != (const KeyValueStore_clientRequest_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const KeyValueStore_clientRequest_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _KeyValueStore_clientRequest_presult__isset {
  _KeyValueStore_clientRequest_presult__isset() : success(false), invalidOp(false) {}
  bool success :1;
  bool invalidOp :1;
} _KeyValueStore_clientRequest_presult__isset;

class KeyValueStore_clientRequest_presult {
 public:


  virtual ~KeyValueStore_clientRequest_presult() throw();
  Request* success;
  InvalidOperation invalidOp;

  _KeyValueStore_clientRequest_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class KeyValueStoreClient : virtual public KeyValueStoreIf {
 public:
  KeyValueStoreClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  KeyValueStoreClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
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
  void clientRequest(Request& _return, const Request& request);
  void send_clientRequest(const Request& request);
  void recv_clientRequest(Request& _return);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class KeyValueStoreProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<KeyValueStoreIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (KeyValueStoreProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_clientRequest(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  KeyValueStoreProcessor(boost::shared_ptr<KeyValueStoreIf> iface) :
    iface_(iface) {
    processMap_["clientRequest"] = &KeyValueStoreProcessor::process_clientRequest;
  }

  virtual ~KeyValueStoreProcessor() {}
};

class KeyValueStoreProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  KeyValueStoreProcessorFactory(const ::boost::shared_ptr< KeyValueStoreIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< KeyValueStoreIfFactory > handlerFactory_;
};

class KeyValueStoreMultiface : virtual public KeyValueStoreIf {
 public:
  KeyValueStoreMultiface(std::vector<boost::shared_ptr<KeyValueStoreIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~KeyValueStoreMultiface() {}
 protected:
  std::vector<boost::shared_ptr<KeyValueStoreIf> > ifaces_;
  KeyValueStoreMultiface() {}
  void add(boost::shared_ptr<KeyValueStoreIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void clientRequest(Request& _return, const Request& request) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->clientRequest(_return, request);
    }
    ifaces_[i]->clientRequest(_return, request);
    return;
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class KeyValueStoreConcurrentClient : virtual public KeyValueStoreIf {
 public:
  KeyValueStoreConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  KeyValueStoreConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
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
  void clientRequest(Request& _return, const Request& request);
  int32_t send_clientRequest(const Request& request);
  void recv_clientRequest(Request& _return, const int32_t seqid);
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
