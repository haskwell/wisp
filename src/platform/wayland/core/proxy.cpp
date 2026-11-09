#include "proxy.hpp"
#include "display.hpp"

Proxy::~Proxy() {
    display.unregisterProxy(objectId);
}
