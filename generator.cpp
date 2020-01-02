#include <cstdio>
#include <cstring>
#include <filesystem>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;

bool starts_with(const std::string& str, const std::string& prefix)
{
	return str.size() >= prefix.size() && std::equal(prefix.begin(), prefix.end(), str.begin());
}

void ensure_out_folder(const fs::path& p)
{
	create_directories(p.parent_path());
}

void generate(const fs::path& outPath, const fs::path& protoPath)
{
	fs::path cppPath = (outPath / protoPath.filename()).replace_extension(".pb.cc");
	fs::path hppPath = (outPath / protoPath.filename()).replace_extension(".pb.h");

	// std::cout << "cppPath: " << cppPath << std::endl;
	// std::cout << "hppPath: " << hppPath << std::endl;

	ensure_out_folder(cppPath);
	ensure_out_folder(hppPath);

	std::ifstream fin(protoPath);
	std::ofstream fCppOut(cppPath);
	std::ofstream fHppOut(hppPath);

	const std::string delim = "<<<";

	std::ofstream* pCurOut = &fHppOut;
	std::ofstream* pNextOut = &fCppOut;

	// std::cout << "reading...: " << protoPath << std::endl;

	std::string str;
	while (std::getline(fin, str))
	{
		// std::cout << "read: " << str << std::endl;
		if (starts_with(str, delim))
		{
			std::swap(pCurOut, pNextOut);
		}
		else
		{
			(*pCurOut) << str << '\n';
		}
	}
}

int main(int argc, char** argv)
{
	fs::path outPath;
	std::vector<fs::path> files;

	for (int i = 1; i < argc; ++i)
	{
		if (strcmp("--cpp_out", argv[i]) == 0)
		{
			outPath = argv[++i];
		}
		else
		{
			files.emplace_back(argv[i]);
		}
	}

	// std::cout << "cpp_out: " << outPath << std::endl;
	for (const auto& f : files)
	{
		// std::cout << f << std::endl;
		generate(outPath, f);
	}

	return 0;
}
