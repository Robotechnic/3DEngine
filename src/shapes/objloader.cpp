#include "shapes/objloader.hpp"

ObjLoader::ObjLoader(Vector3f size, Vector3f pos) : Shape(size, pos), objLoaded(false) {}
ObjLoader::ObjLoader(const ObjLoader& other) : 
	Shape(other),  
	objLoaded(other.objLoaded)
{}

bool ObjLoader::initFileStream() {
	if (this->fileName.size() == 0) {
		throw std::runtime_error("ObjLoader::loadFile: No file name specified");
	}
	this->file.clear();

	try {
		this->file.open(this->fileName);
		if (!file.is_open()) {
			this->errorMessage = "ObjLoader::loadFile: Could not open file " + this->fileName;
			return false;
		}
	} catch (const std::exception &e) {
		this->errorMessage = "ObjLoader::loadFile: File stream error :" + std::string(e.what());
	}
	return true;
}

std::vector<std::string> ObjLoader::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

bool ObjLoader::parseVertex() {
	char lineBegining;
	this->file >> lineBegining;
	if (lineBegining == ' ') {
		Vector3f vertex;
		this->file >> vertex.x >> vertex.y >> vertex.z;
		verticles.push_back(vertex);
		return true;
	}
	
	if (lineBegining == 't' or lineBegining == 'n') {
		return true;
	}
	
	this->errorMessage = "ObjLoader::loadFile: Unknown line beginning " + lineBegining;
	return false;
}

bool ObjLoader::parseFace() {
	std::string face;
	std::getline(this->file, face);
	auto vertex = split(face, ' ');
	if (vertex.size() != 3) {
		this->errorMessage = "ObjLoader::loadFile: Invalid face " + face;
		return false;
	}

	Triangle triangle;
	for (int i = 0; i < 3; i++) {
		auto vertexInfo = split(vertex[i], '/');
		try {
			triangle(i) = this->verticles[std::stoi(vertexInfo[0]) + 1];
		} catch (const std::invalid_argument &err) {
			this->errorMessage = "ObjLoader::loadFile: Invalid numerical value" + vertexInfo[0];
			return false;
		}
	}

	return true;
}

void ObjLoader::loadObjFile(const std::string &fileName) {
	char lineBegining;
	bool parserResult;

	this->fileName = fileName;
	parserResult = this->initFileStream();

	this->objLoaded = false;
	this->verticles.clear();
	this->errorLine = 0;
	this->file >> lineBegining;
	while (parserResult && !this->file.eof()) {
		this->errorLine++;
		switch (lineBegining) {
			case '#':
				this->file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				parserResult = true;
				break;
			case 'v':
				parserResult = this->parseVertex();
				break;
			case 'f':
				parserResult = this->parseFace();
				break;
		}
		this->file >> lineBegining;
	}
}

void ObjLoader::shape_init() {
	if (!objLoaded) {
		throw std::runtime_error("ObjLoader::shape_init() called before ObjLoader::loadFile()");
	}

}

void ObjLoader::shape_update() {
	if (!objLoaded) {
		throw std::runtime_error("ObjLoader::update() called before ObjLoader::loadFile()");
	}
	this->triangles.clear();
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