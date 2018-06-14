#include "socket.h"
#include <WS2tcpip.h>
#include <Windows.h>
#include <stdio.h>
#include <vector>
int sock = socket(AF_INET, SOCK_DGRAM, 0);
char* buffer[256];

typedef struct
{
	int x;
	int y;
	int z;
} Vec;
typedef struct
{
	float x;
	float y;
	float z;
} Vector;
struct OutSimPack
{
	unsigned	Time;		// time in milliseconds (to check order)
	Vector		AngVel;		// 3 floats, angular velocity vector
	float		Heading;	// anticlockwise from above (Z)
	float		Pitch;		// anticlockwise from right (X)
	float		Roll;		// anticlockwise from front (Y)
	Vector		Accel;		// 3 floats X, Y, Z
	Vector		Vel;		// 3 floats X, Y, Z
	Vec		Pos;		// 3 ints   X, Y, Z (1m = 65536)
	int			ID;			// optional - only if OutSim ID is specified
};

void initSocket() {
	
	struct sockaddr_in serverAddres;
	/* Address family = Internet */
	serverAddres.sin_family = AF_INET;
	/* Set port number, using htons function to use proper byte order */
	serverAddres.sin_port = htons(30000);
	/* Set the IP address to desired host to connect to */
	serverAddres.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (bind(sock, (struct sockaddr *) &serverAddres, sizeof(serverAddres)) < 0) {
		printf("Failed to bind socket");
	}
	else {
		printf("Binded to socket");
	}
}
void reciveBuffer() {
	recv(sock, *buffer, 256, 0);
}
int interpretData() {
	OutSimPack out;
	out.Time = (out.Time << 8+*buffer[0]) + (out.Time << 16 + *buffer[1]) + (out.Time << 32 + *buffer[2]) + (out.Time << 64 + *buffer[3]);
}
