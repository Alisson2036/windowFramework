#include "objLoader.h"


void objLoader::fromFile(std::string fileName)
{
	std::ifstream file(fileName);

	enum state {
		lineStart,
		comment,
		vertex,
		vertexNormal,
		textureCoord,
		face
	};

	//criação dos buffers
	std::string buf = "";
	std::vector<float> bufNum;
	state current = lineStart;
	//buffers para a face
	char currentVertex = 0;
	objLoader::face tempFace;


	//loop para ler o arquivo
	while (file.good())
	{
		char last = file.get();

		switch (current)
		{

			//INICIO DA LINHA-------------------------------------------------
		case lineStart:
			//identificar o tipo de informação contida na linha
			buf += last;

			if (last == '\n')
			{
				//caso seja o fim da linha
				buf = "";
				break;
			}
			//comentario
			if (buf == "#")
			{
				buf = "";
				current = comment;
			}
			//posicao de um vertice
			if (buf == "v ")
			{
				buf = "";
				current = vertex;
			}
			//vertex normal
			if (buf == "vn ")
			{
				buf = "";
				current = vertexNormal;
			}
			//coordenada de textura
			if (buf == "vt ")
			{
				buf = "";
				current = textureCoord;
			}
			//indices de uma face
			if (buf == "f ")
			{
				buf = "";
				current = face;
			}
			break;


			//COMENARIOS-----------------------------------------------------
		case comment:
			if (last == '\n')
				current = lineStart;
			break;


			//VERTEX----------------------------------------------------------
		case vertex:
			//coloca os vertices no buffer
			if (last == ' ' || last == '\n')
			{
				bufNum.push_back(std::stof(buf));
				buf = "";
			}
			else
				buf += last;
			//
			if (last == '\n')
			{
				vertices.push_back(objLoader::vertex{ bufNum[0],bufNum[1],bufNum[2] });
				bufNum.clear();
				buf = "";
				current = lineStart;
				break;
			}

			break;


			//VERTEX NORMAL---------------------------------------------------
		case vertexNormal:
			//coloca os vertices no buffer
			if (last == ' ' || last == '\n')
			{
				bufNum.push_back(std::stof(buf));
				buf = "";
			}
			else
				buf += last;
			//
			if (last == '\n')
			{
				normals.push_back(objLoader::vertexNormal{ bufNum[0],bufNum[1],bufNum[2] });
				bufNum.clear();
				buf = "";
				current = lineStart;
				break;
			}

			break;


			//TEXTURE COORD---------------------------------------------------
		case textureCoord:
			//coloca os vertices no buffer

			if (last == ' ' || last == '\n')
			{
				bufNum.push_back(std::stof(buf));
				buf = "";
			}
			else
				buf += last;
			//
			if (last == '\n')
			{
				texCoord.push_back(objLoader::textureCoord{ bufNum[0],bufNum[1] });
				bufNum.clear();
				buf = "";
				current = lineStart;
				break;
			}

			break;


			//FACE------------------------------------------------------------
		case face:

			//armazena os 3 indices de cada vertice da face
			if (last == ' ' || last == '/' || last == '\n')
			{
				bufNum.push_back(std::stof(buf));
				buf = "";
			}
			else
				buf += last;
			//armazena cada um dos vertices
			if (last == ' ' || last == '\n')
			{
				tempFace.vertexIndex[currentVertex] = (int)bufNum[0];
				tempFace.textureIndex[currentVertex] = (int)bufNum[1];
				tempFace.normalIndex[currentVertex] = (int)bufNum[2];

				bufNum.clear();


				currentVertex += 1;
			}

			if (last == '\n')
			{
				faces.push_back(tempFace);
				currentVertex = 0;
				current = lineStart;
				break;
			}

			break;
		}
	}
}