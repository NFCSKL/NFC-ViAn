#include "constants.h"

Singleton::Singleton() {}

Singleton* Singleton::get_instance() {
    static Singleton instance;
    return &instance;
}
