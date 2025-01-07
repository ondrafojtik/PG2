#pragma once

#include <iostream>
#include <filesystem>
#include "Model.h"

struct objLoader
{
	std::vector<std::string> paths;
	std::vector<Model*> models;

	void init()
	{
		std::string path = "src/res/models/test/";
		std::filesystem::create_directories(path);
		for (std::filesystem::path path : std::filesystem::directory_iterator(path))
		{
			std::string _path = path.u8string();
			//std::cout << _path << std::endl;
			paths.push_back(_path);
		}

		for (std::string s : paths)
		{
			Model* _model = new Model(s, 1);
			models.push_back(_model);
		}

	}




};