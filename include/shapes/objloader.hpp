#pragma once

#include <SFML/Graphics/Color.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "shapes/shape.hpp"
#include "shapes/triangle.hpp"

class ObjLoader : public Shape{
	public:
		ObjLoader(Vector3f size = Vector3f(1,1,1), Vector3f pos = Vector3f());
		ObjLoader(const ObjLoader& other);

		void loadObjFile(const std::string &filename);
		
		bool isLoaded() const;
		std::string getFileName() const;
		std::string getErrorMessage() const;
		int getErrorLine() const;

	private:
		std::string errorMessage;
		int errorLine;
		std::vector<Triangle> objTriangles;
		bool objLoaded;

		void shape_init() override;
		void shape_update() override;

		std::vector<std::string> split(const std::string &s, char delim);
		std::string fileName;
		std::ifstream file;
		std::vector<Vector3f> verticles;
		bool initFileStream();
		bool parseVertex();
		bool parseFace();
};