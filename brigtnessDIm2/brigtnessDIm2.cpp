#include "stdafx.h"
#include <iostream>
#include "Windows.h"
#include "WinUser.h"
#include "PhysicalMonitorEnumerationAPI.h"
#include "HighLevelMonitorConfigurationAPI.h"
#include "capabilities.h"
#include <strsafe.h>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <ctime>

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

std::string getDesiredBrightness(DWORD currentBrightness) {
	std::string n;
	std::cin >> n;
	return n;
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
	DWORD pdwMonitorCapabilities;
	DWORD pdwSupportedColorTemperatures;

	if (!GetMonitorCapabilities(hPhysicalMonitor, &pdwMonitorCapabilities, &pdwSupportedColorTemperatures)) {
		ShowError(TEXT("GetMonitorCapabilities returned False	!"));
	}

	setConsoleColorTextForRedOrGreen(hConsole, (pdwMonitorCapabilities & MC_CAPS_BRIGHTNESS) == MC_CAPS_BRIGHTNESS);
	std::cout << "a) -GetMonitorBrightness & SetMonitorBrightness\n";

	setConsoleColorTextForRedOrGreen(hConsole, (pdwMonitorCapabilities & MC_CAPS_COLOR_TEMPERATURE) == MC_CAPS_COLOR_TEMPERATURE);
	std::cout << "b) -GetMonitorColorTemperature & SetMonitorColorTemperature (" << findSupportedColorTemperature(pdwSupportedColorTemperatures) << ")\n";

	setConsoleColorTextForRedOrGreen(hConsole, (pdwMonitorCapabilities & MC_CAPS_CONTRAST) == MC_CAPS_CONTRAST);
	std::cout << "c) -GetMonitorContrast & GetMonitorContrast\n";

	setConsoleColorTextForRedOrGreen(hConsole, (pdwMonitorCapabilities & MC_CAPS_DEGAUSS) == MC_CAPS_DEGAUSS);
	std::cout << "d) -Degauss monitor\n";

	setConsoleColorTextForRedOrGreen(hConsole, (pdwMonitorCapabilities & MC_CAPS_MONITOR_TECHNOLOGY_TYPE) == MC_CAPS_MONITOR_TECHNOLOGY_TYPE);
	std::cout << "f) -GetMonitorTechnologyType\n";

	setConsoleColorTextForRedOrGreen(hConsole, (pdwMonitorCapabilities & MC_CAPS_NONE) == MC_CAPS_NONE);
	std::cout << "g) -The monitor does not support any monitor settings\n";

	setConsoleColorTextForRedOrGreen(hConsole, (pdwMonitorCapabilities & MC_CAPS_RED_GREEN_BLUE_DRIVE) == MC_CAPS_RED_GREEN_BLUE_DRIVE);
	std::cout << "h) -GetMonitorRedGreenOrBlueDrive & SetMonitorRedGreenOrBlueDrive\n";

	setConsoleColorTextForRedOrGreen(hConsole, (pdwMonitorCapabilities & MC_CAPS_RED_GREEN_BLUE_GAIN) == MC_CAPS_RED_GREEN_BLUE_GAIN);
	std::cout << "i) -GetMonitorRedGreenOrBlueGain & SetMonitorRedGreenOrBlueGain\n";

	setConsoleColorTextForRedOrGreen(hConsole, (pdwMonitorCapabilities & MC_CAPS_RESTORE_FACTORY_COLOR_DEFAULTS) == MC_CAPS_RESTORE_FACTORY_COLOR_DEFAULTS);
	std::cout << "j) -RestoreMonitorFactoryColorDefaults\n";

	setConsoleColorTextForRedOrGreen(hConsole, (pdwMonitorCapabilities & MC_CAPS_RESTORE_FACTORY_DEFAULTS) == MC_CAPS_RESTORE_FACTORY_DEFAULTS);
	std::cout << "k) -RestoreMonitorFactoryDefaults\n";

	//Reset text color
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}

int main() {
	DWORD cPhysicalMonitors;
	std::string value;
	long desiredValue;
	//Get the monitor handle.
	HMONITOR hMonitor = MonitorFromWindow(GetDesktopWindow(), MONITOR_DEFAULTTOPRIMARY);
	//Get the number of physical monitors.
	BOOL bSuccess = GetNumberOfPhysicalMonitorsFromHMONITOR(hMonitor, &cPhysicalMonitors);
	if (bSuccess) {
		// Allocate the array of PHYSICAL_MONITOR structures.
		LPPHYSICAL_MONITOR pPhysicalMonitors = (LPPHYSICAL_MONITOR)malloc(cPhysicalMonitors * sizeof(PHYSICAL_MONITOR));
		if (pPhysicalMonitors != NULL) {
			//Get the array.
			bSuccess = GetPhysicalMonitorsFromHMONITOR(hMonitor, cPhysicalMonitors, pPhysicalMonitors);
			//Get physical monitor handle.
			HANDLE hPhysicalMonitor = pPhysicalMonitors[0].hPhysicalMonitor;
			std::cout << getTechnologyType(hPhysicalMonitor);
			int currentBrightness = getBrightness(hPhysicalMonitor);
			do {
				std::cout << "Type 'help' for showing monitor capabilities.\n";
				std::cout << "Set your monitor brightness (in %, current: " << currentBrightness << "): ";
				value = getDesiredBrightness(currentBrightness);
				if (value.compare("help") == 0) {
					capabilities(hPhysicalMonitor);
				}
			} while (value.compare("help") == 0);
			try {
				desiredValue = std::stol(value);
			} catch (const std::invalid_argument& ia) {
				std::cerr << "Invalid argument: " << ia.what() << '\n';
				desiredValue = currentBrightness;
			}
			// Close the monitor handles.
			bSuccess = DestroyPhysicalMonitors(cPhysicalMonitors, pPhysicalMonitors);
			// Free the array.
			free(pPhysicalMonitors);
		}
	}
	return 0;
}
