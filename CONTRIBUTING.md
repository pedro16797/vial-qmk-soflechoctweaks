# Contributing to this vial-qmk fork

Thank you for your interest in contributing to this personal fork of vial-qmk! We aim for a highly customized and reliable firmware for the Sofle Choc keyboard.

---

## Core Principles

### Predictable, Low-Risk Contributions
We value stability. Changes should be tightly scoped and well-tested to ensure they don't break the keyboard functionality.

### Solve One Problem at a Time
- Keep each contribution focused on a single sprint goal or bug fix.
- Avoid mixing refactors or formatting changes with behavior changes.

### Minimize Blast Radius
- Touch only the files and functions required for the task, primarily within the `keyboards/sofle_choc` directory.
- Avoid "drive-by improvements" in unrelated code.

### Prove the Change
- Always verify that the firmware compiles successfully after any change.

---

## Firmware Verification

Ensuring the firmware builds correctly is critical. Before submitting any changes, you must verify the build.

### Building the Firmware
To build the firmware for the Sofle Choc with Vial support, use the following command:

```bash
make sofle_choc:vial
```

This will generate the `.hex` or `.bin` file in the root directory (or as specified by the build system).

---

## AI-Assisted Workflow

If you are using an AI agent to contribute, please follow these guidelines:
1.  **Propose a Plan:** Always start with a detailed proposal and plan.
2.  **Minimum Changes:** Make only the minimum required changes.
3.  **Preserve Interfaces:** Do not change existing APIs or data structures unless necessary.
4.  **Verify:** Use available tools to confirm every modification and ensure build success.

---

## PR Process
1.  **Self-Review:** Review your own diff to ensure it follows these guidelines.
2.  **Description:** Use a clear summary and explain the "why" behind the change.
3.  **Validation:** List the checks and build tests performed.

Thanks for helping build this customized firmware!
