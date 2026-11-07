#include "platform/wayland/protocol/registry.hpp"
#include "platform/wayland/connection.hpp"

int main() {
    Connection c;
    Registry r(c);
    r.getRegistry();
    return 0;
}
