#include "inputBuffer.h"

inputBuffer::inputBuffer(const std::vector<type>& types)
{
	create(types);
}

void inputBuffer::create(const std::vector<type>& types)
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

	initialized = true;
}

std::unique_ptr<unsigned char*> inputBuffer::data()
{
	if (!initialized) _throwMsg("Class not initialized");
	return std::make_unique<unsigned char*>(buffer.data());
}

std::vector<char> inputBuffer::getElement(const int index, const std::string type) const
{
	if (!initialized) _throwMsg("Class not initialized");
	if (!containsType(type)) return {};

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

int inputBuffer::getElementCount() const
{
	if (!initialized) _throwMsg("Class not initialized");
	return (int)buffer.size() / elementSize;
	
}

int inputBuffer::getSizeBytes() const
{
	return buffer.size();
}

void inputBuffer::reserve(int dataSize)
{
	if (!initialized) _throwMsg("Class not initialized");
	int diff = dataSize - getElementCount();
	if (diff < 0) _throwMsg("Trying to erase data");
	
	buffer.resize(buffer.size() + (elementSize * diff), 0);
}

void inputBuffer::set(const void* data, int index, const std::string type)
{
	if (!initialized) _throwMsg("Class not initialized");
	if (!containsType(type)) return;

	typeInfo inf = typeToOffset.at(type);
	int bufferIndex = (index * elementSize) + inf.offset;

	for (unsigned char i = 0; i < inf.size; i++)
	{
		buffer[bufferIndex + i] = reinterpret_cast<const char*>(data)[i];
	}

}

void inputBuffer::setArray(const void* data, int elementCount, std::string type)
{
    if (!initialized) _throwMsg("Class not initialized");
    if (!containsType(type)) return;

    typeInfo inf = typeToOffset.at(type);

    // Garante que há espaço suficiente no buffer
    int requiredElements = elementCount;
    if (getElementCount() < requiredElements)
        reserve(requiredElements);

    const char* src = reinterpret_cast<const char*>(data);

    for (int i = 0; i < elementCount; ++i)
    {
        int bufferIndex = (i * elementSize) + inf.offset;
        for (int j = 0; j < inf.size; ++j)
        {
            buffer[bufferIndex + j] = src[i * inf.size + j];
        }
    }
}

void inputBuffer::setLast(const void* const data, const std::string type)
{
	this->set(data, this->getElementCount() - 1, type);
}

void inputBuffer::createVertexBuffer(VertexBuffer& vb)
{
	if (!initialized) _throwMsg("Class not initialized");

	vb.create(
		buffer.data(),
		getElementCount(),
		elementSize
	);
}
