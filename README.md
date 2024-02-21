# Cheat-Sheet-Module-IoT-ESP32

This Repository describe about list of example general module/protocol in IoT Firmware (like Modbus TCP/IP, MQTT, Bluetooth, etc).

## List of Feature

- Modbus TCP Server
- Modbus TCP Client
- MQTT Client
- etc

## PreSetup

- Install [GIT CLI](https://git-scm.com/downloads)
- Install [VSCode Tools](https://code.visualstudio.com/download)
- Add Extension PlatformIO in VSCode.
- Install PlatformIO CLI (**optional**)

## Runing of One Project

```
git clone https://github.com/Fernandosiahaaan/Cheat-Sheet-Module-IoT-ESP32.git
cd Cheat-Sheet-Module-IoT-ESP32
cd TCP_Client # exmaple select one of example you need
code .
```

Use `Upload` and `Serial monitor` using `PlatformIO` tools at the bottom (look for icons on the right side and the bottom). If you have installed `PlatformIo CLI`, you can run it using the VSCode terminal.

Command Line =

```
 pio device monitor --rts 0 --dtr 0 --port NoPort -b Baudrate -f
 pio run -e esp32doit-devkit-v1 --upload-port NoPort -t upload
```

Example =

```
 pio device monitor --rts 0 --dtr 0 --port COM10 -b 115200 -f
 pio run -e esp32doit-devkit-v1 --upload-port COM10 -t upload
```

## File Tree

```
.
├── library                 # List of Module/protocol sensor IoT
│   ├── General
│   │   ├── general.h
│   ├── Modbus-TCP-Server
│   │   └── ServerTCP.hpp
│   │   └── ServerTCP.cpp
├── TCP_CLient              # Example Repo of Modbus TCP Client
├── TCP_Server              # Example Repo of Modbus TCP Server
└── README.md               # Detail of this repo

```
