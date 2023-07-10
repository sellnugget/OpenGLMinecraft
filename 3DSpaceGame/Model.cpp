#include "Model.h"
#include "AssetLoader.h"

Vertex* Model::GetMeshData()
{

	Vertex* vertex = new Vertex[VertIndices.size()];
	for (unsigned int i = 0; i < VertIndices.size(); i++) {
		Vertex current;
		current.normal = Normals[NormIndices[i]];
		current.position = Vertices[VertIndices[i]];
		current.texcord = TexCords[TexIndices[i]];
		vertex[i] = current;
	}
	return vertex;
}

void Model::LoadObjModel(std::string FileName)
{
	std::string Data = Asset::LoadTextAsset(FileName);
	


	std::stringstream stream;

	stream << Data;
	std::string Line = "";
	while (std::getline(stream, Line)) {

		if (Line[0] == '\0') {
			break;
		}
		std::string Command = Line.substr(0, 2);
		
		
		std::stringstream LineInput;

		LineInput << Line.substr(2, Line.size() - 2);

		//when we read a vertex
		if (Command == "v ") {
			//reads the vertex and stores it
			float x;
			float y;
			float z;
			LineInput >> x;
			LineInput >> y;
			LineInput >> z;
			Vertices.push_back(glm::vec3(x, y, z));

		}
		//when we read a face
		else if (Command == "f ")
		{
			//creates a vertex using the indexes for one triangle

			for (int i = 0; i < 3; i++) {
				
				int vert = 0;
				int norm = 0;
				int tex = 0;

				std::string vertex;

				LineInput >> vertex;

				int values[3];
				int cord = 0;
				for (int i = 0; i < 3; i++) {
					
					//builds int 
					std::string currentint;
					
					while (cord < vertex.size() && vertex[cord] != '/') {
						currentint += vertex[cord];
						cord++;
					}
					values[i] = std::stoi(currentint) - 1;
					cord++;
				}
				vert = values[0];
				tex = values[1];
				norm = values[2];
				
				
				VertIndices.push_back(vert);
				TexIndices.push_back(tex);
				NormIndices.push_back(norm);
			}
			std::vector<int> Vert;
			std::vector<int> Tex;
			std::vector<int> Norm;
			for (int i = 0; i < 3; i++) {
				Vert.push_back(VertIndices[VertIndices.size() - 1]);
				VertIndices.pop_back();
				Tex.push_back(TexIndices[TexIndices.size() - 1]);
				TexIndices.pop_back();
				Norm.push_back(NormIndices[NormIndices.size() - 1]);
				NormIndices.pop_back();
			}
			for (int i = 0; i < 3; i++) {
				VertIndices.push_back(Vert[i]);
				TexIndices.push_back(Tex[i]);
				NormIndices.push_back(Norm[i]);
			}
			
		}
		//this is when the length of the command is longer then 1 char
		else {

			//we have to change the stream to read 1 index further along
			LineInput.clear();
			LineInput << Line.substr(3, Line.size() - 3);

			//reads a face normal
			if (Command == "vn")
			{
				//read normal information
				float x;
				float y;
				float z;
				LineInput >> x;
				LineInput >> y;
				LineInput >> z;
				Normals.push_back(glm::vec3(x, y, z));

			}
			//reads a vertex texture
			else if (Command == "vt") {
				float x;
				float y;
				LineInput >> x;
				LineInput >> y;
				TexCords.push_back(glm::vec2(x,y));
			}
		}
		
		

	}
	

	
	
}
