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
		
		if (current.perVertex)
			inputParams.push_back({ semantic, current.size });
		else
			hasInstancedData = true;

		try
		{
			desc.push_back({ 
				semantic.c_str(), 
				0, 
				current.format, 
				current.perVertex ? 0u : 1u,
				current.perVertex ? D3D11_APPEND_ALIGNED_ELEMENT : 0u,
				current.perVertex ? D3D11_INPUT_PER_VERTEX_DATA : D3D11_INPUT_PER_INSTANCE_DATA, 
				current.perVertex ? 0u : 1u
			});
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

bool shader::hasPerInstanceData()
{
	return hasInstancedData;
}
