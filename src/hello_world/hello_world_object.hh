#ifndef __HELLO_WORLD_HELLO_WORLD_OBJECT_HH__
#define __HELLO_WORLD_HELLO_WORLD_OBJECT_HH__

#include "params/HelloWorldObject.hh"
#include "sim/sim_object.hh"

class HelloWorldObject : public SimObject
{
  public:
    HelloWorldObject(HelloWorldObjectParams *p);
};

#endif // __HELLO_WORLD_HELLO_WORLD_OBJECT_HH__

