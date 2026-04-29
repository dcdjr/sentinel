# Sentinel

Sentinel is a lightweight host-based intrusion detection system for Linux homelabs and small self-hosted environments.

## Current Status

Early development. The current focus is building the event pipeline:

- Structured security/status events
- JSON serialization
- Heartbeat events
- Basic local output
- Future storage and alerting support

## Goals

- Generate structured host events
- Store events locally for later inspection
- Detect basic suspicious or unhealthy behavior
- Provide a small, understandable security monitoring tool for personal infrastructure

## Planned Architecture

```text
agent/
  collects host status and security events

event builder/
  creates structured event objects

serializer/
  converts events to JSON

storage/
  stores events locally for later analysis

detector/
  checks events against simple rules

alerter/
  reports important findings
