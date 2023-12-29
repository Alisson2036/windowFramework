#include "shader.h"

shader::shader(const wchar_t* vertexShader, const wchar_t* pixelShader)
{
	create(vertexShader, pixelShader);
}

void shader::create(const wchar_t* vertexShader, const wchar_t* pixelShader)
{
	//CRIA PIXEL SHADER
	vs.create(vertexShader);

	//CRIA PIXEL SHADER
	ps.create(pixelShader);

	std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
	ShaderDesc sd(vs.getBlob());

	//checando os semantic names do vertex shader e criando o input layout correto
	//e o array inputParams para ser usado pelo inputBuffer
	for (std::string& semantic : sd.inputParams)
	{
		format current = layouts.at(semantic);
		inputParams.push_back({ semantic, current.size });

		try
		{
			desc.push_back({ semantic.c_str(), 0, current.format, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}
		catch (...)
		{
			_throwMsg("Shader input semantic name doesn't match any input type");
		}
	}



	//CRIA INPUT LAYOUT
	il.create(&vs, desc, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	initialized = true;
}

void shader::bind()
{
	if (!initialized) _throwMsg("Class not initialized");

	if (vs.isInitialized()) vs.bind();
	if (ps.isInitialized()) ps.bind();
	if (il.isInitialized()) il.bind();
}
