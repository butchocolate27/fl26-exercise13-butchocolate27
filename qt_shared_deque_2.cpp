// qt_shared_deque_2.cpp
#include <iostream>
#include <cassert>

#include "shared_ring_buffer.h"

int main(int argc, char *argv[])
{
  SharedRingBuffer<int> buffer;

  buffer.create(100);

  while(true){
    if(buffer.getCount() < 100){
      int value;
      std::cout << "$>";
      if(!(std::cin >> value)){
        break;
      }
      if(buffer.enqueue(value)){
        std::cout << "Enqueued: " << value << std::endl;
      }
    }
  }

  return 0;
}
