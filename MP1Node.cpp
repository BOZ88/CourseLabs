/**********************************
 * FILE NAME: MP1Node.cpp
 *
 * DESCRIPTION: Membership protocol run by this Node.
 * 				Definition of MP1Node class functions.
 **********************************/

#include "MP1Node.h"

/*
 * Note: You can change/add any functions in MP1Node.{h,cpp}
 */

/**
 * Overloaded Constructor of the MP1Node class
 * You can add new members to the class if you think it
 * is necessary for your logic to work
 */
MP1Node::MP1Node(Member *member, Params *params, EmulNet *emul, Log *log, Address *address) {
  for (int i = 0; i < 6; i++) {
    NULLADDR[i] = 0;
  }
  this->memberNode = member;
  this->emulNet = emul;
  this->log = log;
  this->par = params;
  this->memberNode->addr = *address;
}

/**
 * Destructor of the MP1Node class
 */
MP1Node::~MP1Node() {}

/**
 * FUNCTION NAME: recvLoop
 *
 * DESCRIPTION: This function receives message from the network and pushes into the queue
 * 				This function is called by a node to receive messages currently waiting for it
 */
int MP1Node::recvLoop() {
  if (memberNode->bFailed) {
    return false;
  } else {
    return emulNet->ENrecv(&(memberNode->addr), enqueueWrapper, NULL, 1, &(memberNode->mp1q));
  }
}

/**
 * FUNCTION NAME: enqueueWrapper
 *
 * DESCRIPTION: Enqueue the message from Emulnet into the queue
 */
int MP1Node::enqueueWrapper(void *env, char *buff, int size) {
  return Queue::enqueue((queue<q_elt> *) env, (void *) buff, size);
}

/**
 * FUNCTION NAME: nodeStart
 *
 * DESCRIPTION: This function bootstraps the node
 * 				All initializations routines for a member.
 * 				Called by the application layer.
 */
void MP1Node::nodeStart(char *servaddrstr, short servport) {
  Address joinaddr;
  joinaddr = getJoinAddress();

  // Self booting routines
  if (initThisNode(&joinaddr) == -1) {
#ifdef DEBUGLOG
    log->LOG(&memberNode->addr, "init_thisnode failed. Exit.");
#endif
    exit(1);
  }

  if (!introduceSelfToGroup(&joinaddr)) {
    finishUpThisNode();
#ifdef DEBUGLOG
    log->LOG(&memberNode->addr, "Unable to join self to group. Exiting.");
#endif
    exit(1);
  }

  return;
}

/**
 * FUNCTION NAME: initThisNode
 *
 * DESCRIPTION: Find out who I am and start up
 */
int MP1Node::initThisNode(Address *joinaddr) {
  /*
   * This function is partially implemented and may require changes
   */
//  int id = *(int *) (&memberNode->addr.addr);
//  int port = *(short *) (&memberNode->addr.addr[4]);

  memberNode->bFailed = false;
  memberNode->inited = true;
  memberNode->inGroup = false;
  // node is up!
  memberNode->nnb = 0;
  memberNode->heartbeat = 0;
  memberNode->pingCounter = TFAIL;
  memberNode->timeOutCounter = -1;
  initMemberListTable(memberNode);

  return 0;
}

/**
 * FUNCTION NAME: introduceSelfToGroup
 *
 * DESCRIPTION: Join the distributed system
 */
int MP1Node::introduceSelfToGroup(Address *joinaddr) {
  MessageHdr *msg;
#ifdef DEBUGLOG
  static char s[1024];
#endif

  if (0 == memcmp((char *) &(memberNode->addr.addr), (char *) &(joinaddr->addr), sizeof(memberNode->addr.addr))) {
    // I am the group booter (first process to join the group). Boot up the group
#ifdef DEBUGLOG
    log->LOG(&memberNode->addr, "Starting up group...");
#endif
    memberNode->inGroup = true;

    int id = 0;
    short port = 0;
    int timestamp = par->getcurrtime();
    memcpy(&id, &memberNode->addr.addr[0], sizeof(int));
    memcpy(&port, &memberNode->addr.addr[4], sizeof(short));

    MemberListEntry myEntry = MemberListEntry(id, port, memberNode->heartbeat, timestamp);
    memberNode->memberList.push_back(myEntry);
  } else {
    int msgsize = sizeof(MessageHdr) + sizeof(joinaddr->addr) + sizeof(long) + 1;
    msg = (MessageHdr *) malloc(msgsize * sizeof(char));

    // create JOINREQ message: format of data is {struct Address myaddr}
    msg->msgType = JOINREQ;
    memcpy((char *) (msg + 1), &memberNode->addr.addr, sizeof(memberNode->addr.addr));
    memcpy((char *) (msg + 1) + 1 + sizeof(memberNode->addr.addr), &memberNode->heartbeat, sizeof(long));

#ifdef DEBUGLOG
    sprintf(s, "Trying to join...");
    log->LOG(&memberNode->addr, s);
#endif

    // send JOINREQ message to introducer member
    emulNet->ENsend(&memberNode->addr, joinaddr, (char *) msg, msgsize);

    free(msg);
  }

  return 1;

}

/**
 * FUNCTION NAME: finishUpThisNode
 *
 * DESCRIPTION: Wind up this node and clean up state
 */
int MP1Node::finishUpThisNode() {
  memberNode->inGroup = false;
  free(memberNode);
}

/**
 * FUNCTION NAME: nodeLoop
 *
 * DESCRIPTION: Executed periodically at each member
 * 				Check your messages in queue and perform membership protocol duties
 */
void MP1Node::nodeLoop() {
  if (memberNode->bFailed) {
    return;
  }

  // Check my messages
  checkMessages();

  // Wait until you're in the group...
  if (!memberNode->inGroup) {
    return;
  }

  // ...then jump in and share your responsibilites!
  nodeLoopOps();

  return;
}

/**
 * FUNCTION NAME: checkMessages
 *
 * DESCRIPTION: Check messages in the queue and call the respective message handler
 */
void MP1Node::checkMessages() {
  void *ptr;
  int size;

  // Pop waiting messages from memberNode's mp1q
  while (!memberNode->mp1q.empty()) {
    ptr = memberNode->mp1q.front().elt;
    size = memberNode->mp1q.front().size;
    memberNode->mp1q.pop();
    recvCallBack((void *) memberNode, (char *) ptr, size);
  }
  return;
}

/**
 * FUNCTION NAME: recvCallBack
 *
 * DESCRIPTION: Message handler for different message types
 */
bool MP1Node::recvCallBack(void *env, char *data, int size) {
  MessageHdr *msg = (MessageHdr *) data;
  if (msg->msgType == JOINREQ) {
    handleJoinReq(msg);
  } else if (msg->msgType == JOINREP) {
    handleJoinRep(msg);
  } else if (msg->msgType == INQUIRY) {
    if (memberNode->inGroup) {
      handleInquiry(msg);
    }
  } else {
    cout << "unknown msg type" << endl;
  }

  return true;
}

void MP1Node::handleInquiry(const MessageHdr *msg) {

  Address from;
  from.init();
  long heartbeat = 0;
  int memberSize = 0;

  long currentPos = 0;
  memcpy(from.addr, (char *) (msg + 1), sizeof(memberNode->addr.addr));
  currentPos += sizeof(memberNode->addr.addr);
  memcpy(&heartbeat, (char *) (msg + 1) + currentPos, sizeof(long));
  currentPos += sizeof(long);
  memcpy(&memberSize, (char *) (msg + 1) + currentPos, sizeof(size_t));
  currentPos += sizeof(long);
  MemberListEntry *memberList = (MemberListEntry *) malloc(memberSize * sizeof(MemberListEntry));
  memcpy(memberList, (char *) (msg + 1) + currentPos, sizeof(MemberListEntry) * memberSize);

#ifdef DEBUGLOG
  char s[100] = {0};
  sprintf(s, "inquiry from %d.%d.%d.%d:%d", from.addr[0], from.addr[1], from.addr[2],
          from.addr[3], *(short *) &from.addr[4]);
  log->LOG(&memberNode->addr, s);
#endif

  for (unsigned int i = 0; i < memberSize; i++) {
    MemberListEntry *inquiryEntry = &memberList[i];
    MemberListEntry *localEntry = findLocalMember(inquiryEntry);
    if (localEntry != NULL) {
      assert(inquiryEntry->id == localEntry->id);
      if (inquiryEntry->heartbeat > localEntry->heartbeat) {
        localEntry->heartbeat = inquiryEntry->heartbeat;
        localEntry->timestamp = par->getcurrtime();
      }
    } else { // Entry is new added
      Address addr;
      addr.init();
      memcpy(&addr.addr, &inquiryEntry->id, sizeof(int));
      memcpy(&addr.addr[4], &inquiryEntry->port, sizeof(int));

      inquiryEntry->timestamp = par->getcurrtime();
      memberNode->memberList.push_back(*inquiryEntry);
      memberNode->nnb++;
      log->logNodeAdd(&memberNode->addr, &addr);
    }
  }
}

void MP1Node::handleJoinRep(const MessageHdr *msg) {
  assert(memberNode->memberList.size() == 0);
  memberNode->inGroup = true;

#ifdef DEBUGLOG
  log->LOG(&memberNode->addr, "handleJoinRep");
#endif

  Address from;
  from.init();
  long heartbeat = 0;
  int memberSize = 0;

  long currentPos = 0;
  memcpy(from.addr, (char *) (msg + 1), sizeof(memberNode->addr.addr));
  currentPos += sizeof(memberNode->addr.addr);
  memcpy(&heartbeat, (char *) (msg + 1) + currentPos, sizeof(long));
  currentPos += sizeof(long);
  memcpy(&memberSize, (char *) (msg + 1) + currentPos, sizeof(size_t));
  currentPos += sizeof(long);
  MemberListEntry *memberListEntry = (MemberListEntry *) malloc(memberSize * sizeof(MemberListEntry));
  memcpy(memberListEntry, (char *) (msg + 1) + currentPos, sizeof(MemberListEntry) * memberSize);

  memberNode->nnb = memberSize;
  for (size_t i = 0; i < memberSize; i++) {

    MemberListEntry entry = *(memberListEntry + i);
    entry.timestamp = par->getcurrtime(); // update local timestampAddress address;
    Address address = getAddressFromEntry(entry);

    memberNode->memberList.push_back(entry);

    if (!(address == memberNode->addr)) {
      log->logNodeAdd(&memberNode->addr, &address);
    }
  }
}
Address MP1Node::getAddressFromEntry(MemberListEntry &entry) const {
  Address address;
  address.init();
  memcpy(address.addr, &entry.id, sizeof(int));
  memcpy(&address.addr[5], &entry.port, sizeof(short));
  return address;
}

void MP1Node::handleJoinReq(const MessageHdr *msg) const {
  Address from;
  from.init();
  long heartbeat = 0;

  memcpy(&(from.addr), (char *) (msg + 1), sizeof(memberNode->addr.addr));
  memcpy(&heartbeat, (char *) (msg + 1) + 1 + sizeof(memberNode->addr.addr), sizeof(long));

  assert(!isAddressEqual(&from, &memberNode->addr));

  log->logNodeAdd(&memberNode->addr, &from);

  int id = 0;
  short port = 0;
  int timestamp = par->getcurrtime();
  memcpy(&id, &from.addr[0], sizeof(int));
  memcpy(&port, &from.addr[4], sizeof(short));

  MemberListEntry entry = MemberListEntry(id, port, heartbeat, timestamp);
  memberNode->memberList.push_back(entry);

  size_t memberSize = memberNode->memberList.size();

  MessageHdr *resMsg;
  int resMsgsize = sizeof(MessageHdr) + sizeof(from.addr)
      + sizeof(long) // heart beat
      + sizeof(size_t) // member size
      + memberSize * sizeof(MemberListEntry); // member list

  resMsg = (MessageHdr *) malloc(resMsgsize * sizeof(char));
  resMsg->msgType = JOINREP;
  long shiftPos = 0;

  memcpy((char *) (resMsg + 1) + shiftPos, &memberNode->addr.addr, sizeof(memberNode->addr.addr));
  shiftPos += sizeof(memberNode->addr.addr);
  memcpy((char *) (resMsg + 1) + shiftPos, &memberNode->heartbeat, sizeof(long));
  shiftPos += sizeof(size_t);
  memcpy((char *) (resMsg + 1) + shiftPos, &memberSize, sizeof(size_t));
  shiftPos += sizeof(size_t);
  memcpy((char *) (resMsg + 1) + shiftPos, memberNode->memberList.data(), sizeof(MemberListEntry) * memberSize);

  // send JOINREP message to new member
  emulNet->ENsend(&memberNode->addr, &from, (char *) resMsg, resMsgsize);
  free(resMsg);

}

/**
 * FUNCTION NAME: nodeLoopOps
 *
 * DESCRIPTION: Check if any node hasn't responded within a timeout period and then delete
 * 				the nodes
 * 				Propagate your membership list
 */
void MP1Node::nodeLoopOps() {
  memberNode->heartbeat++;
#ifdef DEBUGLOG
  string s = to_string(memberNode->heartbeat);
  s = "heartbeat " + s;
  log->LOG(&memberNode->addr, s.c_str());
#endif

  MemberListEntry *myEntry = getMyEntry();
  assert(myEntry != NULL);

  vector<MemberListEntry> liveEntryList;
  findLiveMember(liveEntryList);

  myEntry->heartbeat = memberNode->heartbeat;
  myEntry->timestamp = par->getcurrtime();
  MessageHdr *msg;
  size_t memberSize = liveEntryList.size();

  int msgsize = sizeof(MessageHdr)
      + sizeof(sizeof(char) * 6) // addr
      + sizeof(long) // heart beat
      + sizeof(size_t) // member size
      + memberSize * sizeof(MemberListEntry); // member list

  msg = (MessageHdr *) malloc(msgsize * sizeof(char));
  msg->msgType = INQUIRY;
  long shiftPos = 0;
  memcpy((char *) (msg + 1) + shiftPos, &memberNode->addr.addr, sizeof(memberNode->addr.addr));
  shiftPos += sizeof(memberNode->addr.addr);
  memcpy((char *) (msg + 1) + shiftPos, &memberNode->heartbeat, sizeof(long));
  shiftPos += sizeof(size_t);
  memcpy((char *) (msg + 1) + shiftPos, &memberSize, sizeof(size_t));
  shiftPos += sizeof(size_t);
  memcpy((char *) (msg + 1) + shiftPos, liveEntryList.data(), sizeof(MemberListEntry) * memberSize);

  for (vector<MemberListEntry>::iterator iter = memberNode->memberList.begin();
      iter != memberNode->memberList.end(); iter++) {

    if (rand() % 6 != 0) {
      continue;
    }

    if (iter->timestamp + TREMOVE < par->getcurrtime()) {
      Address addr = getAddressFromEntry(*iter);
      log->logNodeRemove(&memberNode->addr, &addr);
      memberNode->memberList.erase(iter);
    }

    Address address = getAddressFromEntry(*iter);
    emulNet->ENsend(&memberNode->addr, &address, (char *) msg, msgsize);
  }
  free(msg);

  return;
}
void MP1Node::findLiveMember(vector<MemberListEntry>& liveEntryList) {
  for (vector<MemberListEntry>::iterator iter = memberNode->memberList.begin();
       iter != memberNode->memberList.end(); iter++) {
    if (iter->timestamp + TFAIL > par->getcurrtime()) {
      liveEntryList.push_back(*iter);
    }
  }
}

MemberListEntry *MP1Node::getMyEntry() const {
  MemberListEntry *myEntry = NULL;
  int id = 0;
  memcpy(&id, &memberNode->addr.addr[0], sizeof(int));
  for (unsigned int i = 0; i < memberNode->memberList.size(); i++) {
    MemberListEntry *entry = &(memberNode->memberList.at(i));
    if (id == entry->id) {
      myEntry = entry;
      break;
    }
  }
  return myEntry;
}

MemberListEntry * MP1Node::findLocalMember(MemberListEntry *entry) {
  for (unsigned int i = 0; i < memberNode->memberList.size(); i++) {
    if (memberNode->memberList.at(i).id == entry->id) {
      return &memberNode->memberList.at(i);
    }
  }
  return NULL;
}

/**
 * FUNCTION NAME: isNullAddress
 *
 * DESCRIPTION: Function checks if the address is NULL
 */
//int MP1Node::isNullAddress(Address *addr) {
//  return (memcmp(addr->addr, NULLADDR, 6) == 0 ? 1 : 0);
//}

/**
 * FUNCTION NAME: getJoinAddress
 *
 * DESCRIPTION: Returns the Address of the coordinator
 */
Address MP1Node::getJoinAddress() {
  Address joinaddr;

  memset(&joinaddr, 0, sizeof(Address));
  *(int *) (&joinaddr.addr) = 1;
  *(short *) (&joinaddr.addr[4]) = 0;

  return joinaddr;
}

/**
 * FUNCTION NAME: initMemberListTable
 *
 * DESCRIPTION: Initialize the membership list
 */
void MP1Node::initMemberListTable(Member *memberNode) {
  memberNode->memberList.clear();
}

/**
 * FUNCTION NAME: printAddress
 *
 * DESCRIPTION: Print the Address
 */
//void MP1Node::printAddress(Address *addr) {
//  printf("%d.%d.%d.%d:%d \n", addr->addr[0], addr->addr[1], addr->addr[2],
//         addr->addr[3], *(short *) &addr->addr[4]);
//}

bool MP1Node::isAddressEqual(const Address *addr, const Address *other) const {
  return (0 == memcmp((char *) &(addr->addr), (char *) &(other->addr), sizeof(char) * 6));
}