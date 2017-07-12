#include "stdafx.h"
#include "PhysicalMonitorEnumerationAPI.h"
#include "HighLevelMonitorConfigurationAPI.h"
#include "capabilities.h"
#include <strsafe.h>
#include <iostream>
#include <string>
#include <sstream>

void ShowError(LPTSTR lpszFunction) {
	// Retrieve the system error message for the last-error code
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process
	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}

inline bool isInteger(const std::string & s) {
	if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;
	char * p;
	try {
		std::strtol(s.c_str(), &p, 10);
	} catch (const std::invalid_argument& ia) {
		std::cerr << "Invalud argument: " << ia.what() << "\n";
	}
	return (*p == 0);
}

void setConsoleColorTextForRedOrGreen(HANDLE hConsole, bool isSupported) {
	if (isSupported) {
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	}
	else {
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
	}
}

std::string findSupportedColorTemperature(DWORD pdw) {
	std::string temp;
	switch (pdw) {
		case MC_SUPPORTED_COLOR_TEMPERATURE_NONE:
			temp.append("No color temperatures are supported.");
			break;
		case MC_SUPPORTED_COLOR_TEMPERATURE_4000K:
			temp.append("The monitor supports 4,000 kelvins (K) color temperature.");
			break;
		case MC_SUPPORTED_COLOR_TEMPERATURE_5000K:
			temp.append("The monitor supports 5,000 kelvins (K) color temperature.");
			break;
		case MC_SUPPORTED_COLOR_TEMPERATURE_6500K:
			temp.append("The monitor supports 6,500 kelvins (K) color temperature.");
			break;
		case MC_SUPPORTED_COLOR_TEMPERATURE_7500K:
			temp.append("The monitor supports 7,500 kelvins (K) color temperature.");
			break;
		case MC_SUPPORTED_COLOR_TEMPERATURE_8200K:
			temp.append("The monitor supports 8,200 kelvins (K) color temperature.");
			break;
		case MC_SUPPORTED_COLOR_TEMPERATURE_9300K:
			temp.append("The monitor supports 9,300 kelvins (K) color temperature.");
			break;
		case MC_SUPPORTED_COLOR_TEMPERATURE_10000K:
			temp.append("The monitor supports 10,000 kelvins (K) color temperature.");
			break;
		case MC_SUPPORTED_COLOR_TEMPERATURE_11500K:
			temp.append("The monitor supports 11,500 kelvins (K) color temperature.");
			break;
		default:
			break;
	}
	return temp;
}

void capabilities(HANDLE hPhysicalMonitor) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout << "Monitor Capabilities:\n";
	std::cout << "Type 'Code ?Color ?Value'\n";
	DWORD pdwMonitorCapabilities;
	DWORD pdwSupportedColorTemperatures;

	if (!GetMonitorCapabilities(hPhysicalMonitor, &pdwMonitorCapabilities, &pdwSupportedColorTemperatures)) {
		ShowError(TEXT("GetMonitorCapabilities returned False	!"));
	}

	setConsoleColorTextForRedOrGreen(hConsole, (pdwMonitorCapabilities & MC_CAPS_BRIGHTNESS) == MC_CAPS_BRIGHTNESS);
	std::cout << "a) [Default] -GetMonitorBrightness & SetMonitorBrightness\n";

	setConsoleColorTextForRedOrGreen(hConsole, (pdwMonitorCapabilities & MC_CAPS_COLOR_TEMPERATURE) == MC_CAPS_COLOR_TEMPERATURE);
	std::cout << "b) [b] [?int] -GetMonitorColorTemperature & SetMonitorColorTemperature (" << findSupportedColorTemperature(pdwSupportedColorTemperatures) << ")\n";

	setConsoleColorTextForRedOrGreen(hConsole, (pdwMonitorCapabilities & MC_CAPS_CONTRAST) == MC_CAPS_CONTRAST);
	std::cout << "c) [c] [?int] -GetMonitorContrast & SetMonitorContrast\n";

	setConsoleColorTextForRedOrGreen(hConsole, (pdwMonitorCapabilities & MC_CAPS_DEGAUSS) == MC_CAPS_DEGAUSS);
	std::cout << "d) [d] -Degauss monitor\n";

	setConsoleColorTextForRedOrGreen(hConsole, (pdwMonitorCapabilities & MC_CAPS_MONITOR_TECHNOLOGY_TYPE) == MC_CAPS_MONITOR_TECHNOLOGY_TYPE);
	std::cout << "e) [e] -GetMonitorTechnologyType\n";

	setConsoleColorTextForRedOrGreen(hConsole, (pdwMonitorCapabilities & MC_CAPS_NONE) == MC_CAPS_NONE);
	std::cout << "f) -The monitor does not support any monitor settings\n";

	setConsoleColorTextForRedOrGreen(hConsole, (pdwMonitorCapabilities & MC_CAPS_RED_GREEN_BLUE_DRIVE) == MC_CAPS_RED_GREEN_BLUE_DRIVE);
	std::cout << "g) [g] [?r/?g/?b] [?int] -GetMonitorRedGreenOrBlueDrive & SetMonitorRedGreenOrBlueDrive\n";

	setConsoleColorTextForRedOrGreen(hConsole, (pdwMonitorCapabilities & MC_CAPS_RED_GREEN_BLUE_GAIN) == MC_CAPS_RED_GREEN_BLUE_GAIN);
	std::cout << "h) [h] [?r/?g/?b] [?int] -GetMonitorRedGreenOrBlueGain & SetMonitorRedGreenOrBlueGain\n";

	setConsoleColorTextForRedOrGreen(hConsole, (pdwMonitorCapabilities & MC_CAPS_RESTORE_FACTORY_COLOR_DEFAULTS) == MC_CAPS_RESTORE_FACTORY_COLOR_DEFAULTS);
	std::cout << "i) [i] -RestoreMonitorFactoryColorDefaults\n";

	setConsoleColorTextForRedOrGreen(hConsole, (pdwMonitorCapabilities & MC_CAPS_RESTORE_FACTORY_DEFAULTS) == MC_CAPS_RESTORE_FACTORY_DEFAULTS);
	std::cout << "j) [j] -RestoreMonitorFactoryDefaults\n";

	//Reset text color
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}

bool checkColorTemperature(int value) {
	switch (value) {
		case 4000:
		case 5000:
		case 6500:
		case 7500:
		case 8200:
		case 9300:
		case 10000:
		case 11500:
			return true;
		default:
			return false;
	}
	return false;
}

void processRequest(HANDLE monitor, std::string code, int value, std::string type) {
	if (isInteger(code)) {
		long desiredBrightness;
		try {
			desiredBrightness = std::stol(code);
		} catch (const std::invalid_argument& ia) {
			std::cerr << "Invalid argument: " << ia.what() << "\n";
		}
		setBrightness(monitor, desiredBrightness);
	}
	if (code.compare("help") == 0) {
		capabilities(monitor);
	}
	if (code.compare("b") == 0) {
		if (value == -1) {
			std::cout << "Current color temperature: " << getColorTemperature(monitor);
		}
		else {
			if (checkColorTemperature(value)) {
				_MC_COLOR_TEMPERATURE temp = static_cast<_MC_COLOR_TEMPERATURE>(value);
				setColorTemperature(monitor, temp);
			}
			else {
				std::cerr << "This color temperature is not supported.";
			}
		}
	}
	if (code.compare("c") == 0) {
		if (value == -1) {
			DWORD *contrast = getContrast(monitor);
			std::cout << "Color contrast:\n";
			std::cout << "Min: " << contrast[0] << "\nCurrent: " << contrast[1] << "\nMax: " << contrast[2];
		}
		else {
			setContrast(monitor, value);
		}
	}
	if (code.compare("d") == 0) {
		degauss(monitor);
	}
	if (code.compare("e") == 0) {
		std::cout << getTechnologyType(monitor);
	}
	if (code.compare("g") == 0) {
		if (value == -1) {
			DWORD *drive = getRedGreenOrBlueDrive(monitor);
			std::cout << "Red green and blue drive:\n";
			std::cout << "RED:\n" << "Min: " << drive[0] << "\nCurrent: " << drive[1] << "\nMax: " << drive[2];
			std::cout << "\nGREEN:\n" << "Min: " << drive[3] << "\nCurrent: " << drive[4] << "\nMax: " << drive[5];
			std::cout << "\nBLUE:\n" << "Min: " << drive[6] << "\nCurrent: " << drive[7] << "\nMax: " << drive[8];
		}
		else {
			if (!(type.compare("r") == 0 || (type.compare("g") == 0 || type.compare("b") == 0))) {
				std::cerr << "Invalid type";
			}
			else {
				setRedGreenOrBlueDrive(monitor, type, value);
			}
		}
	}
	if (code.compare("h") == 0) {
		if (value == -1) {
			DWORD *gain = getRedGreenOrBlueGain(monitor);
			std::cout << "Red green and blue gain\n";
			std::cout << "RED:\n" << "Min: " << gain[0] << "\nCurrent: " << gain[1] << "\nMax: " << gain[2];
			std::cout << "\nGREEN:\n" << "Min: " << gain[3] << "\nCurrent: " << gain[4] << "\nMax: " << gain[5];
			std::cout << "\nBLUE:\n" << "Min: " << gain[6] << "\nCurrent: " << gain[7] << "\nMax: " << gain[8];
		}
		else {
			if (!(type.compare("r") == 0 || (type.compare("g") == 0 || type.compare("b") == 0))) {
				std::cerr << "Invalid type";
			}
			else {
				setRedGreenOrBlueGain(monitor, type, value);
			}
		}
	}
	if (code.compare("i") == 0) {
		resetFactoryColorDefaults(monitor);
	}
	if (code.compare("j") == 0) {
		resetFactoryDefaults(monitor);
	}
	std::cout << "\n";
}

std::string getInput(HANDLE monitor) {
	std::string line;
	std::string code;
	std::string type;
	int value = -1;
	std::getline(std::cin, line);
	std::istringstream iss(line);
	iss >> code >> type >> value;
	processRequest(monitor, code, value, type);
	return line;
}

int main() {
	DWORD cPhysicalMonitors;
	std::string input;
	HMONITOR hMonitor = MonitorFromWindow(GetDesktopWindow(), MONITOR_DEFAULTTOPRIMARY);
	BOOL bSuccess = GetNumberOfPhysicalMonitorsFromHMONITOR(hMonitor, &cPhysicalMonitors);
	if (bSuccess) {
		LPPHYSICAL_MONITOR pPhysicalMonitors = (LPPHYSICAL_MONITOR)malloc(cPhysicalMonitors * sizeof(PHYSICAL_MONITOR));
		if (pPhysicalMonitors != NULL) {
			bSuccess = GetPhysicalMonitorsFromHMONITOR(hMonitor, cPhysicalMonitors, pPhysicalMonitors);
			HANDLE hPhysicalMonitor = pPhysicalMonitors[0].hPhysicalMonitor;
			do {
				int currentBrightness = getBrightness(hPhysicalMonitor);
				std::cout << "Type 'help' for showing monitor capabilities.\n";
				std::cout << "Set your monitor brightness (in %, current: " << currentBrightness << "): ";
				input = getInput(hPhysicalMonitor);
			} while (!input.compare("exit") == 0 && !isInteger(input));
			bSuccess = DestroyPhysicalMonitors(cPhysicalMonitors, pPhysicalMonitors);
			free(pPhysicalMonitors);
		}
	}
	return 0;
}
