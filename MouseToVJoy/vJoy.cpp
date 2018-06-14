#include "vjoy.h"
//Tests if the driver version is equal to dll version.
int VJoy::testDriver() {
	printf("Mouse to vJoy Feeder\n");
	printf("==================================\n");
	printf("Author: R1per\n");
	printf("Version: 1.68\n");
	// Get the driver attributes (Vendor ID, Product ID, Version Number)
	if (!vJoyEnabled())
	{
		printf("Failed Getting vJoy attributes.\n");
		return -2;
	}
	else
	{
		printf("vJoy Version Number: %S\n", TEXT(GetvJoySerialNumberString()));
	};
	// Test interface DLL matches vJoy driver
	// Compare versions
	WORD VerDll, VerDrv;
	if (!DriverMatch(&VerDll, &VerDrv))
		printf("Failed\r\nvJoy Driver (version %04x) does not match\
 vJoyInterface DLL (version %04x)\n", VerDrv, VerDll);
	else
		printf("vJoyInterface DLL Version: %04x\n", VerDrv);
	printf("OK - Driver and DLL match\n");

}
//Tests if UINT iInterface is existing.
int VJoy::testVirtualDevices(UINT iInterface) {
	// Get the state of the requested device (iInterface)
	_status = GetVJDStatus(iInterface);
	switch (_status)
	{
	case VJD_STAT_OWN:
		printf("vJoy Device %d is already owned by this feeder\n", iInterface);
		break;
	case VJD_STAT_FREE:
		printf("vJoy Device %d is free\n", iInterface);
		break;
	case VJD_STAT_BUSY:
		printf("vJoy Device %d is already owned by another feeder\n\
Cannot continue\n", iInterface);
		return -3;
	case VJD_STAT_MISS:
		printf("vJoy Device %d is not installed or disabled\n\
Cannot continue\n", iInterface);
		return -4;
	default:
		printf("vJoy Device %d general error\nCannot continue\n", iInterface);
		return -1;
	};
}
//If UINT iInterface is existing, tries to accuire it.
int VJoy::accuireDevice(UINT iInterface) {
	// Acquire the target if not already owned
	if ((_status == VJD_STAT_OWN) || \
((_status == VJD_STAT_FREE) && (!AcquireVJD(iInterface))))
	{
		printf("Failed to acquire vJoy device number %d.\n", iInterface);
		return -1;
	}
	else
	{
		printf("Acquired: vJoy device number %d.\n", iInterface);
	}
}
//If UINT iInterface exist, enable FFB to device.
int VJoy::enableFFB(UINT iInterface) {
	// Acquire the target if not already owned
	BOOL Ffbstarted = FfbStart(iInterface);
	if (!Ffbstarted)
	{
		printf("Failed to start FFB on vJoy device number %d.\n", iInterface);
	}
	else
		printf("Started FFB on vJoy device number %d - OK\n", iInterface);
	return 0;
}
//When UINT iInterface is accuired, feeds vars X Y Z RX to Axises X Y Z RX.
void VJoy::feedDevice(UINT iInterface, INT X, INT Y, INT Z, INT RX, BOOL BUTTON1, BOOL BUTTON2, BOOL BUTTON3) {
	//Reports all axies to virtual joystick.
	_iReport.bDevice = iInterface;
	_iReport.wAxisX = X;
	_iReport.wAxisY = Y;
	_iReport.wAxisZ = Z;
	_iReport.wAxisXRot = RX;
	if (BUTTON1) _iReport.lButtons |= 0x1; else _iReport.lButtons &= 0xFE;
	if (BUTTON2) _iReport.lButtons |= 0x2; else _iReport.lButtons &= 0xFD;
	if (BUTTON3) _iReport.lButtons |= 0x4; else _iReport.lButtons &= 0xFB;
	if (!UpdateVJD(iInterface, (PVOID)&_iReport))
	{
		printf("Feeding vJoy device number %d failed - try to enable device then press enter\n", iInterface);
		getchar();
		AcquireVJD(iInterface);
	}
}


