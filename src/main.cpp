#include "script.h"

extern "C" __attribute__((visibility("default"))) int module_start(size_t argc, const void* argv) {
    scriptMain();
    return 0;
}
extern "C" int module_stop(size_t argc, const void* argv) { return 0; }
