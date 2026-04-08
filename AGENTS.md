# Agent Guidance for vial-qmk fork (Sofle Choc)

Welcome, Agent. This document serves as the primary entry point for understanding the goals, technical standards, and safety protocols for this vial-qmk fork.

## Project Overview
This is a fork of vial-qmk, intended for personal use to modify the configuration of the Sofle Choc keyboard and set up GitHub Actions to autonomously build the necessary files to flash the keyboard.

## Main Objectives
The primary goal of this project is to create a highly customized and feature-rich firmware for the Sofle Choc keyboard using the vial-qmk framework. This involves implementing advanced input logic, a dynamic RGB lighting engine, and a specialized OLED display system. Additionally, the project aims to establish a robust CI/CD pipeline for automated firmware building. Detailed tasks and progress are tracked in `ROADMAP.md`.

## Core Directives
To maintain the quality and reliability of this firmware:
-   **Stability First:** Firmware changes must be verified to ensure they don't break the keyboard's functionality.
-   **Minimize Blast Radius:** Touch only the files and functions required for your specific task (primarily under `keyboards/sofle_choc`).
-   **Verify Everything:** Use `list_files`, `read_file`, and relevant verification tools to confirm every modification.
-   **Build Check:** Before submitting any firmware changes, ensure they compile successfully using `make sofle_choc:vial`.

## Working Workflow
1.  **Plan:** Propose a detailed plan before making changes.
2.  **Isolate:** Work on the minimum subset of code needed.
3.  **Validate:** Run build checks and tests where applicable.
4.  **Review:** Perform a self-review or request a review on your diff before finalizing.

## Documentation Maintenance
Keep the project documentation accurate:
-   **`README.md`**: Update with project status if necessary.
-   **`AGENTS.md`**: (This file) Update with major goals and guidelines.
-   **`ROADMAP.md`**: Update as sprints are completed.
