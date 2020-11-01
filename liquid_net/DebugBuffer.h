#pragma once
#include "main.h"
#include <iostream>
#include <string>

class NullBuffer {
public:
	void Send(std::string message);
	void SendByteStream(ByteStream* message);
};

class StlBuffer
{
public:
	void Send(std::string message);
	void SendByteStream(ByteStream* message);
};

class FileBuffer {
public:
	void Send(std::string message);
	void SendByteStream(ByteStream* message);
};

