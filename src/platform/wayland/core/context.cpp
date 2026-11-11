#include <wisp/context.hpp>

#include "display.hpp"
#include "context_implementation.hpp"
#include "globals.hpp"
using namespace wisp;

Context::Context() : implementation(std::make_unique<Implementation>()){}
Context::~Context() = default;
bool Context::isRunning() const {
    return implementation->display.running;
}

void Context::stop() const {
    implementation->display.running = false;
}

void Context::dispatch() const {
    implementation->display.run();
}
