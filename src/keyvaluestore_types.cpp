/**
 * Autogenerated by Thrift Compiler (0.9.3)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "keyvaluestore_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>

namespace keyvaluestore {

int _kOperationValues[] = {
  Operation::PUT,
  Operation::GET,
  Operation::ERASE
};
const char* _kOperationNames[] = {
  "PUT",
  "GET",
  "ERASE"
};
const std::map<int, const char*> _Operation_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(3, _kOperationValues, _kOperationNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));


ProposalNumber::~ProposalNumber() throw() {
}


void ProposalNumber::__set_roundNumber(const int64_t val) {
  this->roundNumber = val;
}

void ProposalNumber::__set_serverID(const int16_t val) {
  this->serverID = val;
}

uint32_t ProposalNumber::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->roundNumber);
          this->__isset.roundNumber = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I16) {
          xfer += iprot->readI16(this->serverID);
          this->__isset.serverID = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t ProposalNumber::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("ProposalNumber");

  xfer += oprot->writeFieldBegin("roundNumber", ::apache::thrift::protocol::T_I64, 1);
  xfer += oprot->writeI64(this->roundNumber);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("serverID", ::apache::thrift::protocol::T_I16, 2);
  xfer += oprot->writeI16(this->serverID);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(ProposalNumber &a, ProposalNumber &b) {
  using ::std::swap;
  swap(a.roundNumber, b.roundNumber);
  swap(a.serverID, b.serverID);
  swap(a.__isset, b.__isset);
}

ProposalNumber::ProposalNumber(const ProposalNumber& other0) {
  roundNumber = other0.roundNumber;
  serverID = other0.serverID;
  __isset = other0.__isset;
}
ProposalNumber& ProposalNumber::operator=(const ProposalNumber& other1) {
  roundNumber = other1.roundNumber;
  serverID = other1.serverID;
  __isset = other1.__isset;
  return *this;
}
void ProposalNumber::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "ProposalNumber(";
  out << "roundNumber=" << to_string(roundNumber);
  out << ", " << "serverID=" << to_string(serverID);
  out << ")";
}


Request::~Request() throw() {
}


void Request::__set_op(const Operation::type val) {
  this->op = val;
}

void Request::__set_key(const std::string& val) {
  this->key = val;
}

void Request::__set_value(const std::string& val) {
  this->value = val;
}

void Request::__set_comment(const std::string& val) {
  this->comment = val;
}

void Request::__set_successful(const bool val) {
  this->successful = val;
}

uint32_t Request::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          int32_t ecast2;
          xfer += iprot->readI32(ecast2);
          this->op = (Operation::type)ecast2;
          this->__isset.op = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->key);
          this->__isset.key = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->value);
          this->__isset.value = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->comment);
          this->__isset.comment = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_BOOL) {
          xfer += iprot->readBool(this->successful);
          this->__isset.successful = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t Request::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("Request");

  xfer += oprot->writeFieldBegin("op", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32((int32_t)this->op);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("key", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->key);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("value", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeString(this->value);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("comment", ::apache::thrift::protocol::T_STRING, 4);
  xfer += oprot->writeString(this->comment);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("successful", ::apache::thrift::protocol::T_BOOL, 5);
  xfer += oprot->writeBool(this->successful);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(Request &a, Request &b) {
  using ::std::swap;
  swap(a.op, b.op);
  swap(a.key, b.key);
  swap(a.value, b.value);
  swap(a.comment, b.comment);
  swap(a.successful, b.successful);
  swap(a.__isset, b.__isset);
}

Request::Request(const Request& other3) {
  op = other3.op;
  key = other3.key;
  value = other3.value;
  comment = other3.comment;
  successful = other3.successful;
  __isset = other3.__isset;
}
Request& Request::operator=(const Request& other4) {
  op = other4.op;
  key = other4.key;
  value = other4.value;
  comment = other4.comment;
  successful = other4.successful;
  __isset = other4.__isset;
  return *this;
}
void Request::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "Request(";
  out << "op=" << to_string(op);
  out << ", " << "key=" << to_string(key);
  out << ", " << "value=" << to_string(value);
  out << ", " << "comment=" << to_string(comment);
  out << ", " << "successful=" << to_string(successful);
  out << ")";
}


ProposalValue::~ProposalValue() throw() {
}


void ProposalValue::__set_proposalNumber(const ProposalNumber& val) {
  this->proposalNumber = val;
}

void ProposalValue::__set_request(const Request& val) {
  this->request = val;
}

uint32_t ProposalValue::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->proposalNumber.read(iprot);
          this->__isset.proposalNumber = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->request.read(iprot);
          this->__isset.request = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t ProposalValue::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("ProposalValue");

  xfer += oprot->writeFieldBegin("proposalNumber", ::apache::thrift::protocol::T_STRUCT, 1);
  xfer += this->proposalNumber.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("request", ::apache::thrift::protocol::T_STRUCT, 2);
  xfer += this->request.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(ProposalValue &a, ProposalValue &b) {
  using ::std::swap;
  swap(a.proposalNumber, b.proposalNumber);
  swap(a.request, b.request);
  swap(a.__isset, b.__isset);
}

ProposalValue::ProposalValue(const ProposalValue& other5) {
  proposalNumber = other5.proposalNumber;
  request = other5.request;
  __isset = other5.__isset;
}
ProposalValue& ProposalValue::operator=(const ProposalValue& other6) {
  proposalNumber = other6.proposalNumber;
  request = other6.request;
  __isset = other6.__isset;
  return *this;
}
void ProposalValue::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "ProposalValue(";
  out << "proposalNumber=" << to_string(proposalNumber);
  out << ", " << "request=" << to_string(request);
  out << ")";
}


ServerAddress::~ServerAddress() throw() {
}


void ServerAddress::__set_port(const int16_t val) {
  this->port = val;
}

void ServerAddress::__set_hostname(const std::string& val) {
  this->hostname = val;
}

uint32_t ServerAddress::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I16) {
          xfer += iprot->readI16(this->port);
          this->__isset.port = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->hostname);
          this->__isset.hostname = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t ServerAddress::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("ServerAddress");

  xfer += oprot->writeFieldBegin("port", ::apache::thrift::protocol::T_I16, 1);
  xfer += oprot->writeI16(this->port);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("hostname", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->hostname);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(ServerAddress &a, ServerAddress &b) {
  using ::std::swap;
  swap(a.port, b.port);
  swap(a.hostname, b.hostname);
  swap(a.__isset, b.__isset);
}

ServerAddress::ServerAddress(const ServerAddress& other7) {
  port = other7.port;
  hostname = other7.hostname;
  __isset = other7.__isset;
}
ServerAddress& ServerAddress::operator=(const ServerAddress& other8) {
  port = other8.port;
  hostname = other8.hostname;
  __isset = other8.__isset;
  return *this;
}
void ServerAddress::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "ServerAddress(";
  out << "port=" << to_string(port);
  out << ", " << "hostname=" << to_string(hostname);
  out << ")";
}


InvalidOperation::~InvalidOperation() throw() {
}


void InvalidOperation::__set_whatOp(const int32_t val) {
  this->whatOp = val;
}

void InvalidOperation::__set_why(const std::string& val) {
  this->why = val;
}

uint32_t InvalidOperation::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->whatOp);
          this->__isset.whatOp = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->why);
          this->__isset.why = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t InvalidOperation::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("InvalidOperation");

  xfer += oprot->writeFieldBegin("whatOp", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->whatOp);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("why", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->why);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(InvalidOperation &a, InvalidOperation &b) {
  using ::std::swap;
  swap(a.whatOp, b.whatOp);
  swap(a.why, b.why);
  swap(a.__isset, b.__isset);
}

InvalidOperation::InvalidOperation(const InvalidOperation& other9) : TException() {
  whatOp = other9.whatOp;
  why = other9.why;
  __isset = other9.__isset;
}
InvalidOperation& InvalidOperation::operator=(const InvalidOperation& other10) {
  whatOp = other10.whatOp;
  why = other10.why;
  __isset = other10.__isset;
  return *this;
}
void InvalidOperation::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "InvalidOperation(";
  out << "whatOp=" << to_string(whatOp);
  out << ", " << "why=" << to_string(why);
  out << ")";
}

const char* InvalidOperation::what() const throw() {
  try {
    std::stringstream ss;
    ss << "TException - service has thrown: " << *this;
    this->thriftTExceptionMessageHolder_ = ss.str();
    return this->thriftTExceptionMessageHolder_.c_str();
  } catch (const std::exception&) {
    return "TException - service has thrown: InvalidOperation";
  }
}

} // namespace