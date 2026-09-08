# ESE5180: Lab 0 Zephyr

| Team Member Name | Email Address              |
|------------------|----------------------------|
| Weiye Zhai       |zhaiwy@engineering.upenn.edu|

**GitHub Repository URL:**  [https://github.com/master869/lab0-zephyr-master869.git](https://github.com/master869/lab0-zephyr-master869.git)

## 1. Hello (Vanilla) Zephyr

### (1.1) Create a video showing blinky on all 3x MCU boards.

## 2. Hello (Nordic) Zephyr

### (2.1) Commit your Zephyr application to your GitHub repository

### (2.2) Create a video showing the change in blinky behavior on the nRF7002DK

## 3. Building with West

### (3.1) Build and Flash using only west commands and not the GUI. Show the terminal prints by embedding a screenshot in your README.md.

## 4. Kconfig

## 5. Device Tree (DT)

### (5.1) Switch out the blinking to LED2. Create an overlay file with your own alias named LED5180 that links to LED2 on the nRF7002 DK. Ensure you only call this alias in the main.c program. Commit your code to this GitHub Classroom repository.

### (5.2) Poll for a button press and switch the LED state. Commit these application changes to your GitHub repository.

### (5.3) (5.3) Create a new alias for your button and call your alias within your main.c. Commit these application changes to your GitHub repository.

## 6. Printing vs. Logging

### (6.1) (6.1) Take screenshots of console output for both builds:
- `CONFIG_SUM_PRINT=y` → result printed with `printk()`.
- `CONFIG_SUM_LOG=y` → result printed with the `Logger` (include hexdump).


