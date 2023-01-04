#ifndef COMPNET_LAB4_TYPES_H
#define COMPNET_LAB4_TYPES_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cstdio>

#include <chrono>

typedef uint8_t mac_addr_t[6];
typedef std::chrono::time_point<std::chrono::steady_clock> timestamp_t;

#define ETH_ALEN 6  // length of mac address

#define HEADER_SIZE 16

const uint16_t ETHER_DATA_TYPE = 0;
const uint16_t ETHER_CONTROL_TYPE = 1;

// MAC Table aging.
const uint32_t ETHER_COMMAND_TYPE_AGING = 0;
const int32_t ETHER_MAC_AGING_THRESHOLD = 10;

struct forward_table_item_ {
  int port;
  mac_addr_t addr;
  int counter;
  struct forward_table_item_ * next;
};

typedef struct forward_table_item_ forward_table_item;

#pragma pack(2)
typedef struct {
  mac_addr_t ether_dest;
  mac_addr_t ether_src;
  uint16_t ether_type;
  uint16_t length;
} ether_header_t;
#pragma pack()

#endif  // ! COMPNET_LAB4_TYPES_H