project(Hypnos-Core-Runtime)

add_library(Hypnos-Core STATIC
    "Cache.hpp"
    "Container.hpp"
    "Delegate.hpp"
    "Encoding.hpp"
    "IO.hpp"
    "Allocator.hpp"
    "PortableDefinition.hpp"
    "Thread.hpp"
    "Type.hpp")

set_target_properties(Hypnos-Core PROPERTIES LINKER_LANGUAGE CXX)