| Supported Targets | ESP32 | ESP32-C3 | ESP32-C5 | ESP32-C6 | ESP32-H2 | ESP32-P4 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- | -------- | -------- | -------- |

# Z21 Example
This example shows how to open a UDP socket on port 21105 and listen for incoming UDP datasets of the Z21 protocol. Received datasets are logged to stdout via default console output.

## How to Use Example
### Hardware Required
* A development board with any supported Espressif SOC chip (see `Supported Targets` table above)
* A USB cable for Power supply and programming

### Build and Flash
Run `idf.py -p PORT flash monitor` to build, flash and monitor the project.

(To exit the serial monitor, type ``Ctrl-]``.)

See the [Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html) for full steps to configure and use ESP-IDF to build projects.