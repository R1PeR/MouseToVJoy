
#include "vjoy.h"

int vJoy::testDriver() {

	printf("Assetto Corsa Mouse to vJoy Feeder\n");
	printf("==================================\n");
	printf("Author: R1per\n");
	printf("Version: 1.4\n");
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
	printf("OK - Driver and DLL match\n", VerDrv);


}

int vJoy::testVirtualDevices(UINT iInterface) {
	// Get the state of the requested device (iInterface)
	status = GetVJDStatus(iInterface);
	switch (status)
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

int vJoy::accuireDevice(UINT iInterface) {
	// Acquire the target if not already owned
	if ((status == VJD_STAT_OWN) || \
		((status == VJD_STAT_FREE) && (!AcquireVJD(iInterface))))
	{
		printf("Failed to acquire vJoy device number %d.\n", iInterface);
		return -1;
	}
	else
		printf("Acquired: vJoy device number %d.\n", iInterface);
}

void vJoy::feedDevice(UINT iInterface, INT X, INT Y, INT Z, INT RX) {
	iReport.bDevice = iInterface;
	iReport.wAxisX = X;
	iReport.wAxisY = Y;
	iReport.wAxisZ = Z;
	iReport.wAxisXRot = RX;
	if (!UpdateVJD(iInterface, (PVOID)&iReport))
	{
		printf("Feeding vJoy device number %d failed - try to enable device then press enter\n", iInterface);
		getchar();
		AcquireVJD(iInterface);
	}
}
