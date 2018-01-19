#include <mbed.h>
#include <Sensors.hpp>

Sensors sen;

int main() {
    sen.setup();
    sen.loop();
}
