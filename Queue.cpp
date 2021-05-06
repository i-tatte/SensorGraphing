#include "Queue.h"

Queue::Queue() {
  _pointer = 0;
  _dataNum = 0;
  for (int i = 0;i < _size; i++) {
    _data[i] = 0;
  }
}

int Queue::get(int index) {
  return _data[(_pointer+index)%_size];
}

void Queue::push(int num) {
  _data[(_pointer+_dataNum)%_size] = num;
  _dataNum++;
  if(_dataNum>_size) {
    _dataNum = _size;
    _pointer++;
    _pointer %= _size;
  }
}

void Queue::clear() {
  for(int i = 0;i < _size; i++) {
    _data[i] = 0;
    _pointer = 0;
    _dataNum = 0;
  }
}
