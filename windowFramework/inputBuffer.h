#pragma once
#include <vector>
#include <string>
#include <memory>
#include <map>
#include "vertexBuffer.h"

//classe para armazenar um array composto por elementos 
// de tamanhos variados, sendo que cada elemento pode ser 
// constituído por multiplas variaveis (ou setores), mapeadas
// com um nome e um offset em relação ao pointer para o começo do elemento
//

class inputBuffer
{
public:
	//apaga os construtores que nao quer ter
	inputBuffer(inputBuffer&) = delete;
	void operator=(inputBuffer&) = delete;

	//estrutura para o input, fornece o nome de cada setor
	//e seu tamanho em bytes
	struct type
	{
		std::string type;
		char size;
	};

	//construtores padroes
	inputBuffer() = default;
	inputBuffer(const std::vector<type>& types);
	void create(const std::vector<type>& types);
	
	//getters
	std::unique_ptr<unsigned char*> data();
	std::vector<char> getElement(const int index, const std::string type) const;
	bool containsType(const std::string type) const;
	int getElementCount() const;
	int getSizeBytes() const;

	//reserva mais espaço, sem nenhuma informação
	void reserve(int dataSize);

	//setters
	void set(const void* data, int index, const std::string type);
	void setLast(const void* data, const std::string type);

	void createVertexBuffer(VertexBuffer& vb);



private:
	bool initialized = false;

	//array para armazenar todos os dados
	std::vector<unsigned char> buffer;

	//mapeia o nome do setor e seu tamanho e offset
	struct typeInfo { char size; int offset; };
	std::map<std::string, typeInfo> typeToOffset;

	int elementSize;

};