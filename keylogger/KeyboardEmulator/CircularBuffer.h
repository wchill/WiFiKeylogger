#pragma once
#define CIRCULAR_BUFFER_SIZE 2048
#define BUFFER_SIZE_MASK (CIRCULAR_BUFFER_SIZE - 1)

template<typename T>
class CircularBuffer {
  private:
    T data[CIRCULAR_BUFFER_SIZE];
    volatile unsigned int readIndex = 0;
    volatile unsigned int writeIndex = 0;
    volatile unsigned int numEntries = 0;

  public:
    CircularBuffer() {}
    unsigned int getCapacity() { return CIRCULAR_BUFFER_SIZE; }
    unsigned int getFreeSpace() { return CIRCULAR_BUFFER_SIZE - numEntries; }
    unsigned int getNumEntries() { return numEntries; }
    void add(T t) {
      data[(writeIndex++) & BUFFER_SIZE_MASK] = t;
      numEntries++;
    }
    void add(T *buf, unsigned int len) {
      unsigned int n = (len + 7) / 8;
      switch(len%8){
          case 0: do{ data[(writeIndex++) & BUFFER_SIZE_MASK] = *buf++;
          case 7:   data[(writeIndex++) & BUFFER_SIZE_MASK] = *buf++;
          case 6:   data[(writeIndex++) & BUFFER_SIZE_MASK] = *buf++;
          case 5:   data[(writeIndex++) & BUFFER_SIZE_MASK] = *buf++;
          case 4:   data[(writeIndex++) & BUFFER_SIZE_MASK] = *buf++;
          case 3:   data[(writeIndex++) & BUFFER_SIZE_MASK] = *buf++;
          case 2:   data[(writeIndex++) & BUFFER_SIZE_MASK] = *buf++;
          case 1:   data[(writeIndex++) & BUFFER_SIZE_MASK] = *buf++;
        }while(--n>0);
      }
      numEntries += len;
    }
    T peek() {
      return data[(readIndex) & BUFFER_SIZE_MASK];
    }
    T remove() {
      T ret = data[(readIndex++) & BUFFER_SIZE_MASK];
      numEntries--;
      return ret;
    }
    void remove(T *buf, unsigned int len) {
      unsigned int n = (len + 7) / 8;
      switch(len%8){
          case 0: do{ *buf++ = data[(readIndex++) & BUFFER_SIZE_MASK];
          case 7:   *buf++ = data[(readIndex++) & BUFFER_SIZE_MASK];
          case 6:   *buf++ = data[(readIndex++) & BUFFER_SIZE_MASK];
          case 5:   *buf++ = data[(readIndex++) & BUFFER_SIZE_MASK];
          case 4:   *buf++ = data[(readIndex++) & BUFFER_SIZE_MASK];
          case 3:   *buf++ = data[(readIndex++) & BUFFER_SIZE_MASK];
          case 2:   *buf++ = data[(readIndex++) & BUFFER_SIZE_MASK];
          case 1:   *buf++ = data[(readIndex++) & BUFFER_SIZE_MASK];
        }while(--n>0);
      }
      numEntries -= len;
    }
};
