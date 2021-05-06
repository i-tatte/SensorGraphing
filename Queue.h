#ifndef QUEUE_INCLUDED
#define QUEUE_INCLUDED

class Queue {
  public:
    Queue();
    int get(int index);
    void push(int num);
    void clear();

  private:
    static const int _size = 250;
    int _data[_size];
    int _pointer;
    int _dataNum;
};

#endif
