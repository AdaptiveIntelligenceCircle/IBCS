# Project Structure – IBCS

## Overview
The IBCS system is composed of multiple layers to ensure modularity, introspection, and extensibility.

### Core Layer (`core/`)
- Kernel logic, introspection engine, and system state model.

### Language Layer (`language/`)
- Grammar definitions, AST transformation, and interpreter.

### Runtime Layer (`runtime/`)
- Execution sandbox, memory management, and reaction loop.

### Simulation Layer (`simulation/`)
- Behavior simulation and human-in-the-loop testing.

### Plugins Layer (`plugins/`)
- External or domain-specific extensions.

---

Each module communicates via a unified interface, maintaining adaptive and introspective behavior across the stack.
