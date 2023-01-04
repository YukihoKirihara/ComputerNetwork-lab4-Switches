#ifndef COMPNET_LAB4_SRC_SWITCH_H
#define COMPNET_LAB4_SRC_SWITCH_H

#include "types.h"

class SwitchBase {
 public:
  SwitchBase() = default;
  ~SwitchBase() = default;

  virtual void InitSwitch(int numPorts) = 0;
  virtual int ProcessFrame(int inPort, char* framePtr) = 0;
};

extern SwitchBase* CreateSwitchObject();

bool Mac_Addr_Compare(mac_addr_t * x, mac_addr_t * y) {
  char * px = (char *) x;
  char * py = (char *) y;
  for (int i=0; i < ETH_ALEN; i++) 
    if (px[i] != py[i]) 
      return false;
  return true;
}

void Mac_Addr_Copy(mac_addr_t * src, mac_addr_t * dst) {
  char * ps = (char *) src;
  char * pd = (char *) dst;
  for (int i=0; i < ETH_ALEN; i++) 
    pd[i] = ps[i];
}


// TODO : Implement your switch class.
class EthernetSwitch : public SwitchBase {
public:
  int port_num;
  forward_table_item * forward_table;
  void InitSwitch(int numPorts) override {
    port_num = numPorts;
    forward_table = new forward_table_item();
    forward_table->counter = -1;  // invalid
    forward_table->next = NULL;    
  }
  int ProcessFrame(int inPort, char* framePtr) override {
    ether_header_t * header = (ether_header_t *) calloc(1, HEADER_SIZE);
    for (int i=0; i<HEADER_SIZE; i++) 
      ((char *)header)[i]= framePtr[i];
    // for (int i=0; i<HEADER_SIZE; i++) 
    //   printf("\033[0;32m%02x \033[0m", (unsigned char)framePtr[i]);
    // printf("\n");
    if (header->ether_type == ETHER_CONTROL_TYPE) {
      forward_table_item * p = NULL;
      for (forward_table_item * iter = forward_table; iter->next != NULL; iter = iter->next) {
        p = iter->next;
        p->counter--;
        if (p->counter == 0) {
          iter->next = p->next; 
          delete p;
        }
      }
      return -1;
    }
    else if (header->ether_type == ETHER_DATA_TYPE) {
      int outPort = 0;
      for (forward_table_item * iter = forward_table->next; iter != NULL; iter = iter->next) {
        if (Mac_Addr_Compare(&iter->addr, &header->ether_dest)) {
          outPort = iter->port;
          if (outPort == inPort) 
            return -1;
          // else return outPort;
        }
      }
      forward_table_item * iter = forward_table;
      forward_table_item * p = iter->next;
      for (; iter->next != NULL; iter = iter->next)  {
        p = iter->next;
        if ((p->port == inPort) && (Mac_Addr_Compare(&p->addr, &header->ether_src))) {
          p->counter = ETHER_MAC_AGING_THRESHOLD;
          break;
        }
      }
      if (iter->next == NULL) {
        p = new forward_table_item();
        Mac_Addr_Copy(&header->ether_src, &p->addr);
        p->port = inPort;
        p->counter = ETHER_MAC_AGING_THRESHOLD;
        p->next = NULL;
        iter->next = p;
        // printf("\033[0;31m Add an item \033[0m : port = %d, mac=", p->port);
        // char * px = (char *)&p->addr;
        // for (int i=0; i<6; i++) 
        //   printf("%02x ", (unsigned char)px[i]);
        // printf("\n");
      }
      return outPort;
    }
    return -1;
  }
};

#endif  // ! COMPNET_LAB4_SRC_SWITCH_H
