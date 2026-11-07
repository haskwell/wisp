#include "../serializer.hpp"
#include "../connection.hpp"
#include "../deserializer.hpp"

class Registry{
    Connection& connection;
    MessageSerializer serializer;
    MessageDeserializer deserializer;

    public:
    Registry(Connection &c);
    void getRegistry();
};