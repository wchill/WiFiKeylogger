#pragma once

typedef unsigned char uint8_t;

struct log_entry {
  uint8_t metadata;
  uint8_t key;
};

#define CMD_PRESS     0x0
#define CMD_RELEASE   0x1

#define SIG_SENDING   0x80
#define SIG_STOP_SEND 0x81
