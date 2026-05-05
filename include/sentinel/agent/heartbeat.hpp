#pragma once

/* Heartbeat is meant to send system info and other important metadata to
   collector The heartbeat is sent every N seconds, is sent in structured JSON
   format, and will eventually send over TCP but for now it just sends to
   stdout. */

void send_heartbeat();
