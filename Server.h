#ifndef SERVER_H
#define SERVER_H

#include "Skeleton.h"

class Server : public Skeleton {
public:
    Server(std::shared_ptr<Connection>& connCS, std::shared_ptr<Connection>& connSC)
        : Skeleton(connCS, connSC)
        {}

    virtual Method_Out method1(const Method1_In &in) override {
        std::cout << "--         method1 called with parameter " << in.a << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        LOG(ERROR) << "Processing method1 for 0.5s" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        LOG(ERROR) << "Processing method1 for 1.0s" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        LOG(ERROR) << "Processing method1 for 1.5s" << std::endl;
        return Method_Out{in.a * in.a};
    }

    virtual Method_Out method2(const Method2_In &in) override {
        std::cout << "--         method2 called with parameters " << in.x << ", " << in.y << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        LOG(ERROR) << "Processing method2 for 0.5s" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        LOG(ERROR) << "Processing method2 for 1.0s" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        LOG(ERROR) << "Processing method2 for 1.5s" << std::endl;
        return Method_Out{in.x * in.y};
    }
};

#endif //SERVER_H
