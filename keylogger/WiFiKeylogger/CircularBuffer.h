#pragma once
#define CIRCULAR_BUFFER_SIZE 2048

template<typename T>
class CircularBuffer {
  private:
    T data[CIRCULAR_BUFFER_SIZE];
    T *start_ptr;
    int capacity = CIRCULAR_BUFFER_SIZE;
    int numEntries = 0;

  public:
    CircularBuffer() {
      start_ptr = data;
    }
    int getCapacity() {
      return capacity;
    }
    int getFreeSpace() {
      return capacity - numEntries;
    }
    int getNumEntries() {
      return numEntries;
    }
    bool add(T t) {
      if (capacity == numEntries) {
        *start_ptr = t;
        start_ptr++;
        if (start_ptr >= data + capacity) {
          start_ptr = data;
        }
        return true;
      } else {
        data[numEntries++] = t;
        return false;
      }
    }
    T removeFirst() {
      T ret = *start_ptr;
      start_ptr++;
      if (start_ptr >= data + capacity) {
        start_ptr = data;
      }
      numEntries = (numEntries + capacity - 1) % capacity;
      return ret;
    }
};
