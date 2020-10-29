#include "main.h"



int main() {
	ByteStream bs;
	bs.Write(10).Write(7).Write(7.4).Write('A').Write('B').Write('C');
	bs.Write(5);
	bs.Write(-4);
	unsigned int len;
	unsigned char* data = bs.GetData(&len);	
	for (int i = 0; i < len; i++) {
		printf("%.2X ", data[i]);
	}
	return 0;
}