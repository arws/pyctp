#pragma once
#include <string>
#include <corecrt_io.h>
#include <direct.h>

namespace FileUtil
{
	static bool is_file_exist(std::string filename)
	{
		return _access(filename.c_str(), 0)? false: true;
	}

	static void create_direction(std::string path)
	{
		_mkdir(path.c_str());
	}


};
