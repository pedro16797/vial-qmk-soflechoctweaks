# Roadmap

- [x] **Sprint 0: Project Setup.** Set up the repository, including `AGENTS.md`, `CONTRIBUTING.md`, and `ROADMAP.md`.
- [ ] **Sprint 1: Setting Things Up.** Set up the git ignore and make sure we have a clearly defined dist folder, set up a GitHub action to autonomously build the necessary files to flash a sofle-choc keyboard.
- [ ] **Sprint 2: Core Logic & Handing.** Hardcoding Encoders (Resolution 4), Layer Combos (MO1+MO2=MO3), and Split-transport mirroring.
- [ ] **Sprint 3: The "Pastel Pulse" RGB Engine.** Implementation of the radial origin (Top-Center), a global `boost_brightness` variable with a **255-limit safety clamp**, and linear decay math in `matrix_scan`.
- [ ] **Sprint 4: The OLED "Waterfall" System.** Setup of vertical rotation (90/270°), a circular character buffer, and handedness logic to isolate typing data per half.
- [ ] **Sprint 5: Status UI & Aesthetic Polish.** Implementation of a persistent "Status Zone" on the OLED edges for Caps Lock and Layer icons.
- [ ] **Sprint 6: CI/CD Pipeline.** Creation of a GitHub Actions workflow and a local `build_firmware.sh` Docker wrapper.
