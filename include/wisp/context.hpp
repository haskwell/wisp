#pragma once
#include <memory>
namespace wisp {
    class Window;
    class Context {
        struct Implementation;
        std::unique_ptr<Implementation> implementation;
        friend class Window;
    public:
        Context();
        bool isRunning() const;
        void stop() const;
        void dispatch() const;
        ~Context();
    };
}