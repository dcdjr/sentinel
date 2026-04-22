#include "heartbeat.hpp"

int main() {
    while (1) {
        send_heartbeat();
    }    

    return 0;
}
