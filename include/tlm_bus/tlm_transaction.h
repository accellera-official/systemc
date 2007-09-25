#ifndef TLM_TRANSACTION_H
#define TLM_TRANSACTION_H

//FIXME
class tlm_transaction
{
public:
  tlm_transaction() : mData(0) {}
  virtual ~tlm_transaction() {}

  void setAddress(uint64_t address) { mAddress = address; }
  uint64_t getAddress() const { return mAddress; }

  void setDataPtr(char* data) { mData = data; }
  char* getDataPtr() const { return mData; }

  void setIsRead(bool b = true) { mIsRead = b; }
  void setIsWrite(bool b = true) { setIsRead(!b); }
  bool isRead() const { return mIsRead; }
  bool isWrite() const { return !isRead(); }

  void setResponse(bool response) { mResponse = response; }
  bool getResponse() const { return mResponse; }

private:
  uint64_t mAddress;
  char* mData;
  bool mResponse;
  bool mIsRead;
};

#endif
