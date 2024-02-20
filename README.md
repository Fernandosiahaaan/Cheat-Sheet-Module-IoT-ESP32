# Cheat-Sheet-Module-IoT-ESP32

This Repository describe about list of example general module/protocol in IoT Firmware (like Modbus TCP/IP, MQTT, Bluetooth, etc).

## List of Feature

- Modbus TCP Server
- Modbus TCP Client
- MQTT Client
- etc

## PreSetup

- Install GIT CLI in (here)[https://git-scm.com/downloads]
- Install VSCode Tools in (here)[https://code.visualstudio.com/download]
- Add Extension PlatformIO in VSCode.
- Install PlatformIO CLI (**optional**)

## Runing of One Project

```
git clone https://github.com/Fernandosiahaaan/Cheat-Sheet-Module-IoT-ESP32.git
cd Cheat-Sheet-Module-IoT-ESP32
cd TCP_Client # exmaple select one of example you need
code .
```

After that `upload` & `Serial monitor` using `PlatformIO` tools.

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
