# Sentinel

Sentinel is a lightweight host-based intrusion detection system for Linux homelabs and small self-hosted environments.

The project is currently focused on building a clean agent-to-collector event pipeline in C++.

## Current Status

Early development. The current prototype supports:

- Structured heartbeat events
- Common event envelopes
- JSON serialization with `nlohmann/json`
- A `sentinel-agent` executable
- A `sentinel-collector` executable
- Length-prefixed TCP event delivery from agent to collector
- Basic collector-side event output + storage

## Goals

- Generate structured host events
- Send events from an agent to a collector over TCP
- Store events locally for later inspection
- Detect basic suspicious or unhealthy behavior
- Provide a small, understandable security monitoring tool for personal infrastructure

## Build

```bash
make
```

This builds:

```text
sentinel-agent
sentinel-collector
```

## Run

Start the collector first:

```bash
./sentinel-collector
```

Then start the agent in another terminal:

```bash
./sentinel-agent
```

The agent sends periodic JSON heartbeat events to the collector.

## Development Notes

Sentinel is intentionally being built in small stages. The current milestone is not full intrusion detection yet. 
The focus is getting the event format, transport layer, and process separation correct before adding real host telemetry, storage, and detection rules.
Accepted events are appended to data/events.jsonl when the collector is run from the project root.
