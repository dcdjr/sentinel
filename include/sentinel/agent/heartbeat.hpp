#pragma once

/* Heartbeat sends periodic host status and metadata to the collector.
   The heartbeat is sent every N seconds in structured JSON format. */

void send_heartbeat();
