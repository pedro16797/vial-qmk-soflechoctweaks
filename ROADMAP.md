## Sprint 0: Setting Things Up [Completed]
**Goal:** Set up the git ignore and make sure we have a clearly defined dist folder, set up a GitHub action to autonomously build the necessary files to flash a sofle-choc keyboard.

## Sprint 1: Core Logic & Hardware Configuration [Completed]
**Goal:** Establish the foundational hardware behaviors, split communication, and critical keymap logic.

* **1.1 Define Encoder Resolution:**
    * Modify `config.h` to add `#define ENCODER_RESOLUTION 4` for smoother, more precise dial rotation.
* **1.2 Enable Split Transport Mirroring:**
    * Modify `config.h` to add `#define SPLIT_TRANSPORT_MIRROR`. This ensures state data (like the active layer and Caps Lock) is synced to both halves, which is required for the OLED status UI later.
* **1.3 Configure Combos:**
    * Modify `rules.mk` to include `COMBO_ENABLE = yes`.
    * Modify `config.h` to add `#define COMBO_COUNT 1`.
    * Modify `keymap.c` to define the combo array and struct for `MO(1) + MO(2) = MO(3)`.

## Sprint 2: The "Pastel Pulse" RGB Engine [Completed]
**Goal:** Implement the custom reactive lighting physics engine with additive per-LED state, localized radial pulses, and layer-aware behaviors.

* **2.1 Enable RGB Features:**
    * Modify `rules.mk` to ensure `RGB_MATRIX_ENABLE = yes` and `RGB_MATRIX_CUSTOM_USER = yes`.
    * Modify `config.h` to include `#define RGB_MATRIX_FRAMEBUFFER_EFFECTS` and `#define RGB_MATRIX_KEYREACTIVE_ENABLED`.
* **2.2 Implement the Brightness Buffer:**
    * In `keymap.c`, declare a global static variable: `static uint16_t boost_brightness = 0;`.
* **2.3 Create the Additive "Boost" Logic (With Safety):**
    * In `keymap.c` within `process_record_user()`, add logic to increment `boost_brightness` by 51 on every key press.
    * **CRITICAL:** Implement the safety clamp immediately after the increment: `if (boost_brightness > 255) boost_brightness = 255;`.
* **2.4 Create the Linear Decay Math:**
    * In `keymap.c` within `matrix_scan_user()`, implement a timer to decrement `boost_brightness` (e.g., `-5` every `10ms`) until it reaches 0.
* **2.5 Build the Custom RGB Effect:**
    * In `keymap.c`, write the custom RGB matrix function.
    * Define the origin point coordinates (Top-Right for Left Half, Top-Left for Right Half).
    * Calculate the radial distance of each LED from the origin.
    * Output a high-Value, low-Saturation color based on the base brightness (127) + `boost_brightness`.

## Sprint 3: The OLED "Waterfall" System [Completed]
**Goal:** Create a visual, falling character buffer on the master display that respects the split handedness.

* **3.1 Enable OLED Drivers:**
    * Modify `rules.mk` to include `OLED_ENABLE = yes`.
* **3.2 Configure OLED Orientation:**
    * In `keymap.c`, implement `oled_init_user()` to set the display rotation to `OLED_ROTATION_270` (or 90) for vertical rendering.
* **3.3 Build the Data Structures:**
    * In `keymap.c`, create a circular character buffer array (e.g., `char typing_buffer[10];`) and a tracking index.
* **3.4 Intercept Keystrokes & Handedness:**
    * In `process_record_user()`, add logic on the master side to convert standard keycodes (A-Z, 0-9) to their ASCII equivalents for keystrokes from both halves.
    * Map `KC_SPACE` to `_` and `KC_ENTER` to `↵`.
* **3.5 Implement the Render Loop:**
    * In `keymap.c` within `oled_task_user()`, write the loop that iterates through `typing_buffer` and prints the characters vertically.

## Sprint 4: Status UI & Aesthetic Polish [Completed]
**Goal:** Implement a dedicated status display on the slave OLED to monitor keyboard state without impacting performance.

* **4.1 Define the Slave Status Logic:**
    * In `oled_task_user()`, separate master and slave rendering paths. Ensure the slave OLED stays on and calls its own status rendering function.
* **4.2 Implement Modifier Synchronization:**
    * Modify `config.h` to add `#define SPLIT_MODS_ENABLE` to ensure the slave half has access to current modifier states.
* **4.3 Implement Status Polling & Rendering:**
    * In `render_status_slave()`, implement state-change detection to minimize I2C traffic.
    * Display Active Layer, held Modifiers (Shift, Ctrl, Alt, Gui), and Lock states (Caps Lock).

## Sprint 5: Build Automation & CI/CD [Pending]
**Goal:** Containerize the build process to eliminate local dependency issues and automate compilation on GitHub.

* **5.1 Create Local Docker Script:**
    * Create `build_firmware.sh` in the repository root.
    * Write the Docker command: `docker run --rm -v "$(pwd):/qmk_firmware" qmkfm/qmk_cli qmk compile -kb sofle_choc -km vial_custom`.
    * Add a post-build step in the script to copy the resulting `.hex`/`.bin` file to a clearly labeled `./dist` directory.
* **5.2 Create the GitHub Action Workflow:**
    * Create `.github/workflows/build.yml`.
    * Set the `on:` trigger for pushes and pull requests to the main/vial branches.
* **5.3 Define the Action Steps:**
    * Step 1: `actions/checkout@v4` with `submodules: recursive`.
    * Step 2: Run the QMK Docker compilation command.
    * Step 3: Use `actions/upload-artifact@v4` to target the compiled `.hex`/`.bin` and the `vial.json` file so they are easily downloadable from the GitHub UI.
