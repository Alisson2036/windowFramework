#include "inputBuffer.h"

inputBuffer::inputBuffer(std::vector<type> types)
{
	buffer = {};

	elementSize = 0;
	for (type t : types)
	{
		typeInfo inf = { t.size, elementSize };
		std::pair<std::string, typeInfo> pair(t.type, inf);
		typeToOffset.insert(pair);
		elementSize += t.size;
	}

}

std::unique_ptr<unsigned char*> inputBuffer::data()
{
	return std::make_unique<unsigned char*>(buffer.data());
}

std::vector<char> inputBuffer::getElement(const int index, const std::string type) const
{
	std::vector<char> data;

	typeInfo inf = typeToOffset.at(type);
	int bufferIndex = index * elementSize + inf.offset;

	for (int i = 0; i < inf.size; i++)
	{
		data.push_back(buffer[bufferIndex + i]);
	}

	return data;
}

bool inputBuffer::containsType(const std::string type) const
{
	return typeToOffset.contains(type);
}

int inputBuffer::getElementCount()
{
	return (int)buffer.size() / elementSize;
}

int inputBuffer::getSizeBytes()
{
	return buffer.size();
}

void inputBuffer::push()
{
	buffer.resize(buffer.size() + elementSize, 0);
}

void inputBuffer::set(const void* data, int index, const std::string type)
{
	typeInfo inf = typeToOffset.at(type);
	int bufferIndex = index * elementSize + inf.offset;

	for (unsigned char i = 0; i < inf.size; i++)
	{
		buffer[bufferIndex + i] = reinterpret_cast<const char*>(data)[i];
	}

}

void inputBuffer::setLast(const void* const data, const std::string type)
{
	this->set(data, this->getElementCount() - 1, type);
}

void inputBuffer::createVertexBuffer(VertexBuffer& vb)
{
	vb.create(
		buffer.data(),
		getElementCount(),
		elementSize
	);
}
