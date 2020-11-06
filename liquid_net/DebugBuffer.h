#pragma once
#include "main.h"
#include <iostream>
#include <string>

std::string GetByteStreamStr(ByteStream* bs);

class NullBuffer {
public:
	void Send(const std::string& message);
	void SendByteStream(ByteStream* message);
};

class StlBuffer
{
public:
	void Send(const std::string& message);
	void SendByteStream(ByteStream* message);
};

class FileBuffer {
public:
	void Send(const std::string& message);
	void SendByteStream(ByteStream* message);
};

