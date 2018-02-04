#include "Storage.hpp"

void Storage::lock() {
    data_mutex.lock();
}

void Storage::unlock() {
    data_mutex.unlock();
}
