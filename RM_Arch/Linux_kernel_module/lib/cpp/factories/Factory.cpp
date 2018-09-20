#include "Factory.h"

int Factory::initialize() {
    return raven_init();
}

int Factory::terminate() {
    return raven_term();
}

key_t Factory::getBufferId(const char *file, int const id) {
    return ftok(file, id);
}