#include "stdafx.h"
#include <iostream>
#include "PhysicalMonitorEnumerationAPI.h"
#include "HighLevelMonitorConfigurationAPI.h"
#include "capabilities.h"

int getBrightness(HANDLE monitor) {
	DWORD pdwMinimumBrightness = 0;
	DWORD pdwCurrentBrightness = 0;
	DWORD pdwMaximumBrightness = 0;

	BOOL bSuccess = GetMonitorBrightness(monitor, &pdwMinimumBrightness, &pdwCurrentBrightness, &pdwMaximumBrightness);
	return bSuccess == TRUE ? pdwCurrentBrightness : -1;
}

void setBrightness(HANDLE monitor, long value) {
	if (value < 0 || value > 100) {
		value = getBrightness(monitor);
		std::cerr << "setBrightness(long value) is out of range";
	}
	SetMonitorBrightness(monitor, value);
}

std::string getColorTemperature(HANDLE monitor) {
	MC_COLOR_TEMPERATURE temp;
	std::string result;
	GetMonitorColorTemperature(monitor, &temp);
	switch (temp) {
		case MC_COLOR_TEMPERATURE_UNKNOWN:
			result.append("Unknown temperature.");
			break;
		case MC_COLOR_TEMPERATURE_4000K:
			result.append("4,000 kelvins (K).");
			break;
		case MC_COLOR_TEMPERATURE_5000K:
			result.append("5,000 kelvins (K).");
			break;
		case MC_COLOR_TEMPERATURE_6500K:
			result.append("6,500 kelvins (K).");
			break;
		case MC_COLOR_TEMPERATURE_7500K:
			result.append("7,500 kelvins (K).");
			break;
		case MC_COLOR_TEMPERATURE_8200K:
			result.append("8,200 kelvins (K).");
			break;
		case MC_COLOR_TEMPERATURE_9300K:
			result.append("9,300 kelvins (K).");
			break;
		case MC_COLOR_TEMPERATURE_10000K:
			result.append("10,000 kelvins (K).");
			break;
		case MC_COLOR_TEMPERATURE_11500K:
			result.append("11,500 kelvins (K).");
			break;
		default:
			result.append("Not found");
			break;
	}
	return result;
}

void setColorTemperature(HANDLE monitor, MC_COLOR_TEMPERATURE temperature) {
	if (!SetMonitorColorTemperature(monitor, temperature)) {
		std::cerr << "Set color temperature is not supported.";
	}
}

DWORD * getContrast(HANDLE monitor) {
	DWORD *pdwContrast = new DWORD[2];
	if (!GetMonitorContrast(monitor, &pdwContrast[0], &pdwContrast[1], &pdwContrast[2])) {
		std::cerr << "Get contrast is not supported.";
	}
	return pdwContrast;
}

void setContrast(HANDLE monitor, DWORD value) {
	DWORD *supportedValues = getContrast(monitor);
	if (value < supportedValues[0] || value > supportedValues[2]) {
		std::cerr << "Set contrast value is out of range.";
	}
	if (!SetMonitorContrast(monitor, value)) {
		std::cerr << "Set contrast is not supported.";
	}
}

void degauss(HANDLE monitor) {
	if (!DegaussMonitor(monitor)) {
		std::cerr << "Degauss monitor is not supported.";
	}
}

std::string getTechnologyType(HANDLE monitor) {
	_MC_DISPLAY_TECHNOLOGY_TYPE type;
	std::string result;
	if (!GetMonitorTechnologyType(monitor, &type)) {
		std::cerr << "Get monitor technology type is not supported.";
	}
	switch (type) {
		case MC_SHADOW_MASK_CATHODE_RAY_TUBE:
			result.append("Shadow-mask cathode ray tube (CRT).");
			break;
		case MC_APERTURE_GRILL_CATHODE_RAY_TUBE:
			result.append("Aperture-grill CRT.");
			break;
		case MC_THIN_FILM_TRANSISTOR:
			result.append("Thin-film transistor (TFT) display.");
			break;
		case MC_LIQUID_CRYSTAL_ON_SILICON:
			result.append("Liquid crystal on silicon (LCOS) display.");
			break;
		case MC_PLASMA:
			result.append("Plasma display.");
			break;
		case MC_ORGANIC_LIGHT_EMITTING_DIODE:
			result.append("Organic light emitting diode (LED) display.");
			break;
		case MC_ELECTROLUMINESCENT:
			result.append("Electroluminescent display.");
			break;
		case MC_MICROELECTROMECHANICAL:
			result.append("Microelectromechanical display.");
			break;
		case MC_FIELD_EMISSION_DEVICE:
			result.append("Field emission device (FED) display.");
			break;
	}
	return result;
}

DWORD * getRedGreenOrBlueDrive(HANDLE monitor) {
	DWORD *drive = new DWORD[10];
	if (!GetMonitorRedGreenOrBlueDrive(monitor, MC_RED_DRIVE, &drive[0], &drive[1], &drive[2])) {
		std::cerr << "Get red drive is not supported.";
	}
	if (!GetMonitorRedGreenOrBlueDrive(monitor, MC_GREEN_DRIVE, &drive[3], &drive[4], &drive[5])) {
		std::cerr << "Get green drive is not supported.";
	}
	if (!GetMonitorRedGreenOrBlueDrive(monitor, MC_BLUE_DRIVE, &drive[6], &drive[7], &drive[8])) {
		std::cerr << "Get blue drive is not supported.";
	}
	return drive;
}

bool checkDriveValue(HANDLE monitor, std::string type, DWORD value, DWORD *supportedValues) {
	if (type.compare("r") != 0 && type.compare("g") != 0 && type.compare("b") != 0) {
		return false;
	}
	if (type.compare("r") == 0 && (value < supportedValues[0] || value > supportedValues[2])) {
		return false;
	}
	if (type.compare("g") == 0 && (value < supportedValues[3] || value > supportedValues[5])) {
		return false;
	}
	if (type.compare("b") == 0 && (value < supportedValues[6] || value > supportedValues[8])) {
		return false;
	}
	return true;
}

void setRedGreenOrBlueDrive(HANDLE monitor, std::string type, int value) {
	MC_DRIVE_TYPE driveType;
	if (!checkDriveValue(monitor, type, value, getRedGreenOrBlueDrive(monitor))) {
		std::cerr << "Value is out of range";
	}
	else {
		if (type.compare("r") == 0) {
			driveType = MC_RED_DRIVE;
		}
		else if (type.compare("g") == 0) {
			driveType = MC_GREEN_DRIVE;
		}
		else if (type.compare("b") == 0) {
			driveType = MC_BLUE_DRIVE;
		}
		if (!SetMonitorRedGreenOrBlueDrive(monitor, driveType, value)) {
			std::cerr << "Cannot set desired drive";
		}
	}
}

DWORD * getRedGreenOrBlueGain(HANDLE monitor) {
	DWORD *gain = new DWORD[10];
	if (!GetMonitorRedGreenOrBlueGain(monitor, MC_RED_GAIN, &gain[0], &gain[1], &gain[2])) {
		std::cerr << "Get red gain is not supported.";
	}
	if (!GetMonitorRedGreenOrBlueGain(monitor, MC_GREEN_GAIN, &gain[3], &gain[4], &gain[5])) {
		std::cerr << "Get green gain is not supported.";
	}
	if (!GetMonitorRedGreenOrBlueGain(monitor, MC_BLUE_GAIN, &gain[6], &gain[7], &gain[8])) {
		std::cerr << "Get blue gain is not supported.";
	}
	return gain;
}

void setRedGreenOrBlueGain(HANDLE monitor, std::string type, int value) {
	MC_GAIN_TYPE gainType;
	if (!checkDriveValue(monitor, type, value, getRedGreenOrBlueGain(monitor))) {
		std::cerr << "Value is out of range";
	}
	else {
		if (type.compare("r") == 0) {
			gainType = MC_RED_GAIN;
		}
		else if (type.compare("g") == 0) {
			gainType = MC_GREEN_GAIN;
		}
		else if (type.compare("b") == 0) {
			gainType = MC_BLUE_GAIN;
		}
		if (!SetMonitorRedGreenOrBlueGain(monitor, gainType, value)) {
			std::cerr << "Cannot set desired gain";
		}
	}
}

void resetFactoryColorDefaults(HANDLE monitor) {
	if (!RestoreMonitorFactoryColorDefaults(monitor)) {
		std::cerr << "Reset factory color defaults is not supported";
	}
}

void resetFactoryDefaults(HANDLE monitor) {
	if (!RestoreMonitorFactoryDefaults(monitor)) {
		std::cerr << "Reset factory monitor defaults is not supported";
	}
}
