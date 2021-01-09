#include "hello_world/hello_world_object.hh"

#include "base/trace.hh" // DPRINTF
#include "debug/HelloWorld.hh"

// constructor
HelloWorldObject::HelloWorldObject(HelloWorldObjectParams *params) :
    SimObject(params)
{
    // use debug flags instead of std::cout
    DPRINTF(HelloWorld, "Created the hello object\n");
}

// function for the param type
// implicitly created from the SimObject Python declaration
HelloWorldObject*
HelloWorldObjectParams::create()
{
    return new HelloWorldObject(this);
}
