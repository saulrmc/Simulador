# ADR-001 – Architectural Style

## Context

The project consists of a celestial body simulator developed in C++ by a single developer.  
The software requires high performance for physics calculations and flexibility in the graphical interface. There are several key constraints:

- **Limited resources:** Single-developer project running on standard hardware.
- **Adaptability:** The graphical library (GUI/rendering layer) may change in the future, or the simulator may need to be ported to other platforms, while the physics logic (Newtonian laws) is expected to remain stable.
- **Integrity:** The physics engine must not be affected by failures or errors in the presentation layer.

## Decision

A **modular monolith** architecture was chosen.

## Rationale

A modular monolith was selected instead of a microkernel architecture because it provides the simplicity required for software developed and maintained by a single person.

Additionally, the architecture conceptually protects the software core through intermediate components, preventing direct dependencies between the UI and the physics engine. This approach increases component decoupling and therefore improves maintainability and adaptability.

## Consequences

### Positive Consequences

- Faster development speed
- Predictable performance
- No network latency
- Easier maintenance
- More time spent developing features instead of learning and managing additional technologies

### Negative Consequences

- Greater discipline is required to preserve the architectural dependency constraints
- Lower extensibility (e.g., plugin support) compared to a microkernel architecture