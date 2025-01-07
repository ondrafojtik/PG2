#pragma once
#include <fstream>
#include <iostream>

struct Map
{
	std::string s_grid;
	std::string path;

	std::fstream myFile;

	Map(std::string _path) : path(_path) { }
	void Reload()
	{
		myFile.open(path);
		myFile >> s_grid;
		myFile.close();
	}

	void Save(std::string grid)
	{
		myFile.open(path);
		myFile << grid;
		myFile.close();
	}

};

/*
	static const char* s_grid =
		"GGGGGFGGGGGGGGG"
		"GGGFFFGGGGGUGIG"
		"GGGGFFFGGGGVGGG"
		"GGGGGGGGGGGVGGG"
		"GGGGGGFFFGGDGGG"
		"GGGGGGGGGGGGGGG"
		"GGGGGGFFFGGGGGG"
		"GGGGGGFGFGGGGGG";*/

