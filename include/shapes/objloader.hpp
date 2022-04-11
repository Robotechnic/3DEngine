#pragma once

#include <SFML/Graphics/Color.hpp>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>

#include "shapes/shape.hpp"
#include "shapes/triangle.hpp"

class ObjLoader : public Shape{
	public:
		ObjLoader(Vector3f size = Vector3f(1,1,1));
		ObjLoader(const ObjLoader& other);

		void loadObjFile(const std::string &filename);
		
		bool isLoaded() const;
		std::string getFileName() const;
		std::string getErrorMessage() const;
		int getErrorLine() const;

	private:
		std::string errorMessage;
		int errorLine;
		void shape_init() override;
		void shape_update() override;

		bool objLoaded;
		std::vector<Triangle> objTriangles;
		std::vector<sf::Color> objColors;
		std::string fileName;
		std::ifstream file;
		std::vector<Vector3f> verticles, normals;
		std::map<std::string, sf::Color> materialColors;
		sf::Color currentColor;
		std::vector<std::string> split(const std::string &s, char delim);
		bool initFileStream();
		bool parseVertex(std::string &lineType);
		bool parseFace(std::string &lineType);
		bool parseMTL(std::string &lineType);
		bool loadMTL(std::ifstream &file);
		bool setMTL(std::string &lineType);
};