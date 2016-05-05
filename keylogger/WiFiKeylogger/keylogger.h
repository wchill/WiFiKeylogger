#pragma once

typedef unsigned char uint8_t;

struct log_entry {
  uint8_t metadata;
  uint8_t key;
};

#define CMD_START     0xFF
#define CMD_PRESS     0x01
#define CMD_RELEASE   0x02
#define CMD_CHANGEMOD 0x04

#define SIG_SENDING   0x80
#define SIG_STOP_SEND 0x81
