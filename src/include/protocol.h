#pragma once

#include "protocol_sensors.h"

// CDH protocol format
struct OutProtocolHeader {
    uint8_t opcode;
} __attribute__((packed));

struct InProtocolHeader {
    uint8_t start; // Value: 0x01
    uint8_t status;
    uint16_t numBytes; // Value: sizeof(SensorData)
} __attribute__((packed));

struct InProtocolFooter {
    uint16_t checksum;
    uint8_t end; // Value: 0x04
} __attribute__((packed));

struct CDHPacket {
    InProtocolHeader header;
    SensorData data;
    InProtocolFooter footer;
} __attribute__((packed));

#ifdef __cplusplus
#if __GNUG__
static_assert(__has_trivial_copy(CDHPacket), "CDHPacket must be trivially copyable.");
#else
static_assert(std::is_trivially_copyable<CDHPacket>::value, "CDHPacket must be trivially copyable.");
static_assert(std::is_pod<SensorData>::value, "SensorData must be a POD type.");
static_assert(std::is_pod<CDHPacket>::value, "CDHPacket must be a POD type.");
#endif
#endif
