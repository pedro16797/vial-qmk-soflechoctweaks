# Analysis of Firmware Hangs and Performance

## Current State
As of the completion of the Performance Optimization phase (Post-Sprint 3), the system has been improved by throttling housekeeping tasks and increasing I2C clock speeds to 400kHz. However, the slave (right) half still experiences intermittent hangs where it stops sending key inputs while the OLED continues to update.

## Possible Causes (Updated)

### 1. I2C/Split Bus Saturation (Contention)
Even though visual-related RPC traffic has been eliminated, QMK's standard split transport (syncing matrix state, layer state, and modifiers) still consumes significant bandwidth.
*   **Contention:** If the OLED driver (which also uses I2C) or custom matrix scanning logic blocks the main loop for too long, the split synchronization window might be missed, leading to retries and input lag.
*   **I2C Blocking:** Standard QMK OLED writes are often blocking. A single `oled_write_P` call for a large multiline string, while efficient in terms of transaction count, still holds the bus for several milliseconds.

### 2. High Computational Load in Housekeeping
The `housekeeping_task_user` is now performing a "shadow" matrix scan of 10 rows to trigger independent visual effects.
*   **Overhead:** While necessary for independence, diffing 10 rows and performing ASCII mapping/RNG calls in every main loop cycle adds overhead that competes with the high-priority `matrix_scan` task.
*   **RNG performance:** Frequent calls to `rand()` in the main loop can be surprisingly expensive on microcontrollers without hardware RNG.

### 3. Custom RGB Shader Complexity
The `pastel_pulse` effect performs linear interpolation and floating-point-like math (fixed point) for every LED on every frame.
*   **Rendering Latency:** Complex shaders can delay the execution of the next matrix scan, especially on split keyboards where the CPU is already heavily taxed by transport logic.

## Potential Measures for Fixes

*   **[DONE] Throttle Visual Tasks:** Shadow matrix scan moved to a 10ms gate in `housekeeping_task_user`.
*   **[DONE] Hardware I2C Speed:** Increased to 400kHz via `I2C1_CLOCK_SPEED`.
*   **[DONE] State-Based OLED Throttling:** The OLED task yields immediately if no animation is active.
*   **[DONE] Global FPS Throttling:** RGB Matrix capped at 30 FPS, OLED at 12.5 FPS.
*   **[DONE] Bitwise Math:** Replaced all divisions with bit-shifts or fixed-point approximations.
*   **[DONE] RNG Removal:** Replaced expensive `rand()` calls with `timer_read32()` based entropy.
*   **[TESTING] OLED Toggle:** Display now only active when CAPS LOCK is enabled to isolate I2C bus impact.

## Hierarchy of Compromises
To ensure the keyboard remains a reliable input device, we must adhere to the following priority hierarchy:
**Reliable key strokes > Fluid key strokes > Dynamic RGB backlight > Dynamic OLED display**

When performance targets are not met, the following compromises will be implemented in order:

1.  **Reduce OLED Animation:** If hangs persist, we will replace the smooth vertical "sliding" (which requires constant redraws) with a "static list" update that only draws when a new key is pressed.
2.  **Throttle RGB Refresh Rate:** Reduce the RGB Matrix frame rate (FPS) to give the CPU more breathing room for matrix scanning and split transport.
3.  **Simplify Custom Shader:** Replace the multi-stage linear interpolation in `pastel_pulse` with simpler, less intensive calculations (e.g., standard QMK breathing or solid colors with a simple boost).
4.  **Reduce Independent Visuals:** If the local shadow-matrix scan is too heavy, we may need to revert to a master-driven model for some effects, accepting some visual latency on the slave side to preserve input integrity.
