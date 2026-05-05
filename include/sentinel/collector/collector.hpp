#pragma once

/* The collector listens on a TCP port and reads the JSON bytes
   sent from an agent and prints it to stdout for now. */

void collect();
