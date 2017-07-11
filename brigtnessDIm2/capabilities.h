#pragma once

#include "PhysicalMonitorEnumerationAPI.h"

int getBrightness(HANDLE monitor);

void setBrightness(HANDLE monitor, long value);

std::string getColorTemperature(HANDLE monitor);

void setColorTemperature(HANDLE monitor, MC_COLOR_TEMPERATURE temperature);

DWORD *getContrast(HANDLE monitor);

void setContrast(HANDLE monitor, DWORD value);

void degauss(HANDLE monitor);

std::string getTechnologyType(HANDLE monitor);

DWORD *getRedGreenOrBlueDrive(HANDLE monitor);

void setRedGreenOrBlueDrive(HANDLE monitor, std::string type, int value);

DWORD *getRedGreenOrBlueGain(HANDLE monitor);

void setRedGreenOrBlueGain(HANDLE monitor, std::string type, int value);

void resetFactoryColorDefaults(HANDLE monitor);

void resetFactoryDefaults(HANDLE monitor);