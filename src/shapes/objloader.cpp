#include "shapes/objloader.hpp"

ObjLoader::ObjLoader(Vector3f size) : Shape(size), objLoaded(false) {}
ObjLoader::ObjLoader(const ObjLoader& other) : 
	Shape(other),  
	objLoaded(other.objLoaded)
{}

std::vector<std::string> ObjLoader::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::string item;
	int i = 0;
	while (std::getline(ss, item, delim)) {
		if (i > 0 || item.size() > 0) {
			elems.push_back(item);
		}
		i++;
	}
	return elems;
}

bool ObjLoader::initFileStream() {
	if (this->fileName.size() == 0) {
		throw std::runtime_error("ObjLoader::loadFile: No file name specified");
	}
	this->file.clear();

	try {
		this->file.open(this->fileName);
		if (file.fail()) {
			this->errorMessage = "ObjLoader::loadFile: Failled to open file " + this->fileName + " (" + strerror(errno) + ")";
			return false;
		}
	} catch (const std::exception &e) {
		this->errorMessage = "ObjLoader::loadFile: File stream error :" + std::string(e.what());
	}

	return true;
}

bool ObjLoader::loadMTL(std::ifstream &file) {
	std::string mtlFileName;
	std::getline(this->file, mtlFileName);

	std::string mtlFilePath = this->fileName.substr(0, this->fileName.find_last_of("/") + 1);
	mtlFilePath += mtlFileName;
	try {
		file.open(mtlFilePath);
		if (file.fail()) {
			this->errorMessage = "ObjLoader::parseMTL: Failled to open file " + mtlFilePath + " (" + strerror(errno) + ")";
			return false;
		}
	} catch (const std::exception &e) {
		this->errorMessage = "ObjLoader::parseMTL: File stream error :" + std::string(e.what());
		return false;
	}
	return true;
}

bool ObjLoader::parseMTL(std::string &lineType) {
	if (lineType != "mtllib") {
		this->errorMessage = "ObjLoader::parseMTL: Expected 'mtllib'";
		return false;
	}
	
	std::ifstream mtlFile;
	this->loadMTL(mtlFile);

	std::string line, materialName = "";
	while (std::getline(mtlFile, line)) {
		std::vector<std::string> elems = this->split(line, ' ');
		if (elems.size() == 0) {
			continue;
		}
		if (elems[0] == "#") {
			continue;
		}
		if (elems[0] == "newmtl") {
			materialName = elems[1];
		} else if (elems[0] == "Kd") {
			if (materialName.size() == 0) {
				this->errorMessage = "ObjLoader::parseMTL: Expected 'newmtl' before 'Kd'";
				return false;
			}
			try {
				this->materialColors[materialName] = sf::Color(
					std::stof(elems[1]) * 255,
					std::stof(elems[2]) * 255,
					std::stof(elems[3]) * 255
				);
			} catch (const std::exception &e) {
				this->errorMessage = "ObjLoader::parseMTL: Failed to parse color :" + std::string(e.what());
				return false;
			}
		}
	}

	return true;
}

bool ObjLoader::setMTL(std::string &lineType) {
	if (lineType != "usemtl") {
		this->errorMessage = "ObjLoader::setMTL: Expected 'usemtl'";
		return false;
	}

	std::string materialName;
	std::getline(this->file, materialName);

	if (this->materialColors.find(materialName) == this->materialColors.end()) {
		this->errorMessage = "ObjLoader::setMTL: Unknown material " + materialName;
		return false;
	}

	this->currentColor = this->materialColors[materialName];
	return true;
}

bool ObjLoader::parseVertex(std::string &lineType) {
	if (lineType == "v") {
		Vector3f vertex;
		this->file >> vertex.x >> vertex.y >> vertex.z;
		verticles.push_back(vertex);
		return true;
	}

	if (lineType == "vn") {
		Vector3f normal;
		this->file >> normal.x >> normal.y >> normal.z;
		normals.push_back(normal);
		return true;
	}

	if (lineType == "vt") {
		return true;
	}
	
	this->errorMessage = "ObjLoader::parseVertex: Unknown line beginning " + lineType;
	return false;
}

bool ObjLoader::parseFace(std::string &lineType) {
	std::string face;
	std::getline(this->file, face);
	auto vertex = split(face, ' ');
	if (vertex.size() != 3) {
		this->errorMessage = "ObjLoader::loadFile: Invalid face " + face;
		return false;
	}

	Triangle triangle;
	std::vector<std::string> vertexInfo;
	for (int i = 0; i < 3; i++) {
		vertexInfo = split(vertex[i], '/');
		try {
			triangle(i) = this->verticles[std::stoi(vertexInfo[0]) - 1];
		} catch (const std::invalid_argument &err) {
			this->errorMessage = "ObjLoader::loadFile: Invalid numerical value" + vertexInfo[0];
			return false;
		}
	}

	if (vertexInfo.size() == 2 && vertexInfo[1].size() > 0) {
		try {
			triangle.normal = this->normals[std::stoi(vertexInfo[1]) - 1];
		} catch (const std::invalid_argument &err) {
			this->errorMessage = "ObjLoader::loadFile: Invalid numerical value" + vertexInfo[1];
			return false;
		}
	}

	// add triangle to the list
	this->objTriangles.push_back(triangle);
	this->objColors.push_back(this->currentColor);
	return true;
}

void ObjLoader::loadObjFile(const std::string &fileName) {
	std::string lineStart;
	bool parserResult;

	this->fileName = fileName;
	parserResult = this->initFileStream();

	this->objLoaded = false;
	this->verticles.clear();
	this->errorLine = 0;
	this->currentColor = sf::Color::White;
	std::getline(this->file, lineStart, ' ');
	while (parserResult && !this->file.eof()) {
		this->errorLine++;
		while (lineStart[0] == '\n') {
			lineStart.erase(lineStart.begin());
		}
		switch (lineStart[0]) {
			case '#':
				this->file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				parserResult = true;
				break;
			case 'v':
				parserResult = this->parseVertex(lineStart);
				break;
			case 'f':
				parserResult = this->parseFace(lineStart);
				break;
			case 'm':
				parserResult = this->parseMTL(lineStart);
				break;
			case 'u':
				parserResult = this->setMTL(lineStart);
				break;
			case 'o':
			case 's': // ignore unsuported lines
				std::getline(this->file, lineStart);
				parserResult = true;
				break;
			default: // if unknown, set error
				this->errorMessage = "ObjLoader::loadFile: Unknown line beginning " + lineStart;
				parserResult = false;
				break;
		}
		std::getline(this->file, lineStart, ' ');
	}
	this->verticles.clear();
	this->objLoaded = parserResult;
	this->updateNeeded = parserResult;
	if (this->objLoaded)
		this->init();
}

void ObjLoader::shape_init() {
	if (!objLoaded) {
		throw std::runtime_error("ObjLoader::shape_init() called before ObjLoader::loadFile()");
	}
	this->triangles.clear();
	this->colors.clear();
	this->triangles.resize(this->objTriangles.size());
	this->colors.resize(this->objTriangles.size());
	for (unsigned int i = 0; i < this->objTriangles.size(); i++) {
		this->triangles[i] = this->objTriangles[i];
		this->triangles[i].applyTransform(this->rotationMatrix, this->size);
		this->colors[i] = this->objColors[i];
	}
}

void ObjLoader::shape_update() {
	if (!objLoaded) {
		throw std::runtime_error("ObjLoader::update() called before ObjLoader::loadFile()");
	}
	this->triangles.clear();
	this->triangles.resize(this->objTriangles.size());
	this->colors.clear();
	this->colors.resize(this->objTriangles.size());
	for (unsigned int i = 0; i < this->objTriangles.size(); i++) {
		this->triangles[i] = this->objTriangles[i];
		this->triangles[i].applyTransform(this->rotationMatrix, this->size);
		this->colors[i] = this->objColors[i];
	}
}

bool ObjLoader::isLoaded() const {
	return this->objLoaded;
}
std::string ObjLoader::getFileName() const {
	return this->fileName;
}
std::string ObjLoader::getErrorMessage() const {
	return this->errorMessage;
}

int ObjLoader::getErrorLine() const {
	return this->errorLine;
}