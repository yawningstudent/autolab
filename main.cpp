#include <queue>
#include <future>
#include <iostream>

#include "Latch.h"

#include "MoveBase.h"
#include "Proxy.h"

#include "Server.h"

#include "Connection.h"

#include <chrono>
using namespace std::chrono_literals;

#define VARIANT_FUTURE_GET 1
#define VARIANT_CLIENT_METHOD 2
#define VARIANT_CLIENT_METHOD_TWO_SERVICES 3
#define VARIANT_LAMBDA 4
#define VARIANT_LAMBDA_CHAIN 5

#define VARIANT VARIANT_CLIENT_METHOD_TWO_SERVICES

Latch completion;

std::shared_ptr<Connection> connectionClientServer{std::make_shared<Connection>("1:1:1:1", 1010)};
std::shared_ptr<Connection> connectionServerClient{std::make_shared<Connection>("2:2:2:2", 2020)};

#if VARIANT == VARIANT_CLIENT_METHOD
class Client : public Proxy {
public:
    Client(std::shared_ptr<Connection>& connCS, std::shared_ptr<Connection>& connSC) : Proxy(connCS, connSC) {}

    virtual void onMethod1ResultAvailable(const Method_Out &out) override {
        std::cout << "Result: " << out.r << std::endl << std::flush;
        completion.trigger();
    }
 };
#endif

#if VARIANT == VARIANT_CLIENT_METHOD_TWO_SERVICES
class Client : public Proxy {
public:
    Client(std::shared_ptr<Connection>& connCS, std::shared_ptr<Connection>& connSC) : Proxy(connCS, connSC) {}

    virtual void onMethod1ResultAvailable(const Method_Out &out) override {
        std::cout << "Result1: " << out.r << std::endl << std::flush;
        completion.trigger();
    }

    virtual void onMethod2ResultAvailable(const Method_Out &out) override {
        std::cout << "Result2: " << out.r << std::endl << std::flush;
        completion.trigger();
    }
};
#endif

int main() {
    CONTEXT_BEGIN("main")

    std::cout << "main-2: " << std::endl << std::flush;

    std::cout << "Variant: " << VARIANT << std::endl;

    Server server(connectionClientServer, connectionServerClient);

#if VARIANT == VARIANT_FUTURE_GET
    Proxy service(connectionClientServer, connectionServerClient);
    std::shared_future<Method_Out> resFuture1(service.method1({10}));

    std::cout << "Result1: " << "before" << std::endl;
    std::cout << "Result1: " << resFuture1.get().r << std::endl;
    std::cout << "Result1: " << "after" << std::endl;
#endif

#if VARIANT == VARIANT_CLIENT_METHOD
    Client client(connectionClientServer, connectionServerClient);

    std::shared_future<Method_Out> resFuture(client.method1({5}));
#endif

#if VARIANT == VARIANT_CLIENT_METHOD
    Client client(connectionClientServer, connectionServerClient);

    std::shared_future<Method_Out> resFuture(client.method1({5}));
#endif

#if VARIANT == VARIANT_CLIENT_METHOD_TWO_SERVICES
    std::shared_ptr<Connection> connectionClientServer2{std::make_shared<Connection>("3:3:3:3", 3030)};
    std::shared_ptr<Connection> connectionServerClient2{std::make_shared<Connection>("4:4:4:4", 4040)};

    //Server server2(connectionClientServer2, connectionServerClient2);

    Client client1(connectionClientServer, connectionServerClient);
    std::shared_future<Method_Out> resFuture1(client1.method1({5}));
    std::shared_future<Method_Out> resFuture2(client1.method2({4, 9}));

    //Client client2(connectionClientServer2, connectionServerClient2);
    //std::shared_future<Method_Out> resFuture2(client2.method2({4, 9}));
#endif

#if VARIANT == VARIANT_LAMBDA
    Proxy service(connectionClientServer, connectionServerClient);

    std::shared_future<Method_Out> resFutureA(service.method1({5}, [&](const Method_Out &out){
        std::cout << "Result: " << out.r << std::endl;
        completion.trigger();
    }));
#endif

#if VARIANT == VARIANT_LAMBDA_CHAIN
    Proxy serviceA(connectionClientServer, connectionServerClient);
    Proxy serviceB(connectionClientServer, connectionServerClient);

    // Place your code here:
    std::shared_future<Method_Out> resFutureA(serviceA.method1({5}, [&](const Method_Out &out) {
        std::cout << "First result: " << out.r << std::endl;
        std::shared_future<Method_Out> resFutureA(serviceB.method1({out.r}, [&](const Method_Out &out) {
            std::cout << "Final result: " << out.r << std::endl;
            completion.trigger();
        }));
    }));
#endif

    std::this_thread::sleep_for(2s);

    completion.trigger();
    completion.wait();

    CONTEXT_END()

    return 0;
}
