#include "Model.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// Model constructor
Model::Model() {
	transform = glm::mat4(1.0f);
}
Model::Model(const char* objFile) {
	transform = glm::mat4(1.0f);
	Model::LoadObj(objFile);
}
Model::Model(std::vector<Mesh*> meshes) {
	Model::meshs = std::vector<Mesh*>(meshes.begin(), meshes.end());
	Model::transform = glm::mat4(1.0f);
}

// load model from obj file
void Model::LoadObj(const char* objFile) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn;
	std::string err;

	std::string base_dir = std::string(objFile);
	size_t found = base_dir.find_last_of("/\\");
	base_dir = base_dir.substr(0, found);

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, objFile,	base_dir.c_str());
	if (!warn.empty()) {
		std::cout << "WARN: " << warn << std::endl;
	}
	if (!err.empty()) {
		std::cerr << err << std::endl;
	}

	// load all attribute
	//		load positions
	std::vector<glm::vec3> positions;
	for (int positionsIdx = 0; positionsIdx < attrib.vertices.size(); positionsIdx += 3) {
		glm::vec3 position(attrib.vertices[positionsIdx + 0],
			attrib.vertices[positionsIdx + 1],
			attrib.vertices[positionsIdx + 2]);

		positions.push_back(position);
	}
	//		load normals
	std::vector<glm::vec3> normals;
	for (int normalIdx = 0; normalIdx < attrib.normals.size(); normalIdx += 3) {
		glm::vec3 newNormal(attrib.normals[normalIdx + 0],
			attrib.normals[normalIdx + 1],
			attrib.normals[normalIdx + 2]);

		normals.push_back(newNormal);
	}
	//		load texCoords
	std::vector<glm::vec2> texCoords;
	for (int texCoordsIdx = 0; texCoordsIdx < attrib.texcoords.size(); texCoordsIdx += 2) {
		glm::vec2 newTexCoord(attrib.texcoords[texCoordsIdx + 0],
			attrib.texcoords[texCoordsIdx + 1]);

		texCoords.push_back(newTexCoord);
	}

	// setting up default color
	const glm::vec4 colors[] = { glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
		glm::vec4(1.0f, 0.5f, 0.5f, 1.0f),
		glm::vec4(1.0f, 1.0f, 0.5f, 1.0f) ,
		glm::vec4(0.5f, 1.0f, 0.5f, 1.0f) ,
		glm::vec4(0.5f, 1.0f, 1.0f, 1.0f) ,
		glm::vec4(0.5f, 0.5f, 1.0f, 1.0f) ,
		glm::vec4(1.0f, 0.5f, 1.0f, 1.0f) ,
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
	};

	printf("%5u vertices, %5u indices, %5u textCoords\n", positions.size(), normals.size(), texCoords.size());

	// load meshs
	
	std::map <int, Material*> loadedMaterialIdTable; // to save loaded material index
	// clear to load new meshs
	Model::meshs.clear();
	// for each mesh in file
	for (unsigned int meshIdx = 0; meshIdx < shapes.size(); meshIdx++) {
		printf("  Mesh-%03u: ", meshIdx);
		std::cout << shapes[meshIdx].name << std::endl;
		const tinyobj::mesh_t& curMesh = shapes[meshIdx].mesh;

		// load materials
		std::map<int, Material*>::iterator textureTableIt;
		int materialIdx = curMesh.material_ids[0];
		std::cout << "load material" << curMesh.material_ids[0] << std::endl;
		if (materialIdx >= 0) {
			// check if MaterialId has load preversly
			textureTableIt = loadedMaterialIdTable.find(materialIdx);
			const tinyobj::material_t& currMaterial = materials[materialIdx];
			if (textureTableIt == loadedMaterialIdTable.end()) {
				// load new texture
				std::string texturePath;
				Material* newMaterial = new Material();

				// load diffuse texture
				if (!currMaterial.diffuse_texname.empty()) {
					texturePath = base_dir + "/" + currMaterial.diffuse_texname;
					newMaterial->LoadTextures(Material::TYPE_DIFFUSE, texturePath.c_str());
				}

				// load ambient texture
				if (!currMaterial.ambient_texname.empty()) {
					texturePath = base_dir + "/" + currMaterial.ambient_texname;
					newMaterial->LoadTextures(Material::TYPE_AMBIENT, texturePath.c_str());
				}

				// load specular texture
				if (!currMaterial.specular_texname.empty()) {
					texturePath = base_dir + "/" + currMaterial.specular_texname;
					newMaterial->LoadTextures(Material::TYPE_SPECULAR, texturePath.c_str());
				}

				//// load specular texture
				//if (!currMaterial.normal_texname.empty() ||) {
				//	texturePath = base_dir + "/" + currMaterial.normal_texname;
				//	newMaterial->LoadTextures(Material::TYPE_SPECULAR, texturePath.c_str());
				//}

				// save material
				Model::materials.push_back(newMaterial);
				// save material index table
				loadedMaterialIdTable[materialIdx] = newMaterial;
			}
			else {
				// the texture has been loaded
				Model::materials.push_back(loadedMaterialIdTable[materialIdx]);
			}
		}


		// create vertices and indices for new mesh
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;

		for (int j = 0; j < curMesh.indices.size(); j++) {
			// build Vertex from attributes
			glm::vec3 position = positions[curMesh.indices[j].vertex_index];

			glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);
			if(curMesh.indices[j].normal_index >= 0)
				normal = normals[curMesh.indices[j].normal_index];

			glm::vec2 texCoord = glm::vec2(0.0f, 0.0f);
			if (curMesh.indices[j].texcoord_index >= 0)
				texCoord = texCoords[curMesh.indices[j].texcoord_index];

			Vertex newVertex;
			newVertex.position = position;
			newVertex.normal = normal;
			newVertex.color = colors[meshIdx %8];
			newVertex.texCoord = texCoord;

			// check if vertex exist in vertices
			std::vector<Vertex>::iterator vertwxIt = std::find(vertices.begin(), vertices.end(), newVertex);
			if (vertwxIt != vertices.end()) {
				// append index only
				unsigned int existedIndex = std::distance(vertices.begin(), vertwxIt);
				indices.push_back(existedIndex);
			}
			else {
				// append new vertex and index
				indices.push_back(vertices.size());
				vertices.push_back(newVertex);
			}
		}
		printf("    %-5u vertices, %-5u indices, %-5u textures\n", vertices.size(), indices.size(), textures.size());

		// calculate tangent
		// https://www.cs.upc.edu/~virtual/G/1.%20Teoria/06.%20Textures/Tangent%20Space%20Calculation.pdf
		glm::vec3* tan1 = new glm::vec3[vertices.size() * 2];
		glm::vec3* tan2 = tan1 + vertices.size();
		memset(tan1,0, vertices.size() * sizeof(glm::vec3) * 2);
		for (unsigned int j = 0; j < indices.size(); j+=3) {
			unsigned int i1 = indices[0];
			unsigned int i2 = indices[1];
			unsigned int i3 = indices[2];

			const glm::vec3& v1 = vertices[i1].position;
			const glm::vec3& v2 = vertices[i2].position;
			const glm::vec3& v3 = vertices[i3].position;

			const glm::vec2& w1 = vertices[i1].texCoord;
			const glm::vec2& w2 = vertices[i2].texCoord;
			const glm::vec2& w3 = vertices[i3].texCoord;

			float x1 = v2.x - v1.x;
			float x2 = v3.x - v1.x;
			float y1 = v2.y - v1.y;
			float y2 = v3.y - v1.y;
			float z1 = v2.z - v1.z;
			float z2 = v3.z - v1.z;

			float s1 = w2.x - w1.x;
			float s2 = w3.x - w1.x;
			float t1 = w2.y - w1.y;
			float t2 = w3.y - w1.y;

			float r = 1.0F / (s1 * t2 - s2 * t1);
			glm::vec3 sdir((t2* x1 - t1 * x2)* r, (t2* y1 - t1 * y2)* r, (t2* z1 - t1 * z2)* r);
			glm::vec3 tdir((s1* x2 - s2 * x1)* r, (s1* y2 - s2 * y1)* r, (s1* z2 - s2 * z1)* r);

			tan1[i1] += sdir;
			tan1[i2] += sdir;
			tan1[i3] += sdir;

			tan2[i1] += tdir;
			tan2[i2] += tdir;
			tan2[i3] += tdir;
		}
		//std::vector<glm::vec4> tangent(vertices.size());
		for (unsigned int j = 0; j < vertices.size(); j++)
		{
			const glm::vec3& n = vertices[j].normal;
			const glm::vec3& t = tan1[j];

			// Gram-Schmidt orthogonalize
			glm::vec4 tangent = glm::vec4(glm::normalize(t - n * glm::dot(n, t)), 0.0f);

			// Calculate handedness
			tangent[3] = (glm::dot(glm::cross(n, t), tan2[j]) < 0.0F) ? -1.0F : 1.0F;
			
			vertices[j].tangent = tangent;
		}

		delete[] tan1;
		// create new Mesh by vertices and indices
		Mesh* newMesh = new Mesh(vertices, indices);
		Model::meshs.push_back(newMesh);
	}

	return;
}

//// load model from gltf-2.0 file
//void Model::LoadGltf(const char* gltfFile) {
//	tinygltf::TinyGLTF loader;
//	std::string err;
//	std::string warn;
//	tinygltf::Model model;
//
//	bool res = loader.LoadASCIIFromFile(&model, &err, &warn, gltfFile);
//	if (!warn.empty()) {
//		std::cout << "WARN: " << warn << std::endl;
//	}
//
//	if (!err.empty()) {
//		std::cout << "ERR: " << err << std::endl;
//	}
//
//	if (!res)
//		std::cout << "Failed to load glTF: " << gltfFile << std::endl;
//	else
//		std::cout << "Loaded glTF: " << gltfFile << std::endl;
//
//
//	const tinygltf::Scene& scene = model.scenes[model.defaultScene];
//	for (size_t i = 0; i < scene.nodes.size(); ++i) {
//		assert((scene.nodes[i] >= 0) && (scene.nodes[i] < model.nodes.size()));
//		LoadGltf_bindModelNodes(model, model.nodes[scene.nodes[i]]);
//	}
//}
//
//void Model::LoadGltf_bindModelNodes(tinygltf::Model& model, tinygltf::Node& node) {
//	if ((node.mesh >= 0) && (node.mesh < model.meshes.size())) {
//		LoadGltf_bindMesh(model, model.meshes[node.mesh]);
//	}
//
//	for (size_t i = 0; i < node.children.size(); i++) {
//		assert((node.children[i] >= 0) && (node.children[i] < model.nodes.size()));
//		LoadGltf_bindModelNodes(model, model.nodes[node.children[i]]);
//	}
//}
//
//void Model::LoadGltf_bindMesh(tinygltf::Model& model, tinygltf::Mesh& mesh) {
//
//}

// load model by instance
void Model::Instance(Model& refModel) {
	Model::meshs = std::vector<Mesh*>(refModel.meshs.begin(), refModel.meshs.end());
	Model::textures = std::vector<Texture*>(refModel.textures.begin(), refModel.textures.end());
}

// update transform by position, scale, orientation, up
void Model::UpdateTransform() {
	transform = glm::mat4(1.0f);

	// rotate
	glm::vec3 new_z = glm::normalize(orientation);
	glm::vec3 new_x = glm::normalize(glm::cross(orientation, up));
	glm::vec3 new_y = glm::normalize(glm::cross(new_x, new_z));
	transform = glm::mat4(
		new_x.x, new_x.y, new_x.z, 0.0f,
		new_y.x, new_y.y, new_y.z, 0.0f,
		new_z.x, new_z.y, new_z.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	) * transform;

	// scale
	transform = glm::scale(scale) * transform;

	// translate
	transform = glm::translate(position) * transform;
}

void Model::UpdateTransform(glm::mat4& ref) {
	Model::transform = ref;
}

void  Model::UpdateTransform(glm::vec3& position, glm::vec3& scale, glm::vec3& orientation, glm::vec3& up) {
	Model::position = position;
	Model::scale = scale;
	Model::orientation = orientation;
	Model::up = up;
	UpdateTransform();
}

// draw model
void Model::Draw() {
	for (unsigned int meshIdx = 0; meshIdx < Model::meshs.size(); meshIdx++) {
		Model::meshs[meshIdx]->Draw();
	}
}
void Model::Draw(Shader& shader) {
	shader.setUniform("model", transform);
	for (unsigned int meshIdx = 0; meshIdx < Model::meshs.size(); meshIdx++) {
		if(materials[meshIdx])
			materials[meshIdx]->Use(shader);
		Model::meshs[meshIdx]->Draw();
		Texture::UnBind();
	}
}

// delete model
void Model::Delete() {
	for (unsigned int meshIdx = 0; meshIdx < Model::meshs.size(); meshIdx++) {
		Model::meshs[meshIdx]->Delete();
		delete Model::meshs[meshIdx];
	}
	Model::meshs.clear();
}


// Material
const int Material::TYPE_DIFFUSE = 1;
const int Material::TYPE_AMBIENT = 2;
const int Material::TYPE_SPECULAR = 3;
const int Material::TYPE_NORMAL = 4;

Material::Material() {
	Material::texDiffuse = NULL;
	Material::texAmbient = NULL;
	Material::texSpecular = NULL;
}

void Material::LoadTextures(int type, const char* fileName) {
	if (type == Material::TYPE_DIFFUSE) {
		Material::texDiffuse = new Texture(fileName);
		return;
	}
	if (type == Material::TYPE_AMBIENT) {
		Material::texAmbient = new Texture(fileName);
		return;
	}
	if (type == Material::TYPE_SPECULAR) {
		Material::texSpecular = new Texture(fileName);
		return;
	}
	if (type == Material::TYPE_NORMAL) {
		Material::texNormal = new Texture(fileName);
		return;
	}
}
void Material::Use(Shader& shader) {
	if (Material::texDiffuse)
		Material::texDiffuse->BindAndUse(shader, "texDiffuse");
	if (Material::texAmbient)
		Material::texAmbient->BindAndUse(shader, "texAmbient");
	if (Material::texSpecular)
		Material::texSpecular->BindAndUse(shader, "texSpecular");
	if (Material::texNormal)
		Material::texNormal->BindAndUse(shader, "texNormal");
}
// delete textures
void Material::Delete() {
	if (Material::texDiffuse) {
		delete Material::texDiffuse;
		Material::texDiffuse = NULL;
	}
	if (Material::texAmbient) {
		delete Material::texAmbient;
		Material::texAmbient = NULL;
	}
	if (Material::texSpecular) {
		delete Material::texSpecular;
		Material::texSpecular = NULL;
	}
}