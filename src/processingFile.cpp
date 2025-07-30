#include "../include/processingFile.h"

#include <filesystem>
#include <iostream>

void ProcessingFile::replaceTempToMain(int argc, char** argv){

	if (argc == 1) {
		printf("no arguments\n");
		return;
	}

	std::string currentExeFile = argv[1];
	std::string newExeFileName = argv[2];
	std::string tempExeFileName = argv[3];

	if (!std::filesystem::exists(currentExeFile) && !std::filesystem::exists(tempExeFileName)) {
		printf("transferred files name not exists\n");
		return;
	}

	try {
		std::filesystem::remove(currentExeFile);
		std::filesystem::rename(tempExeFileName, newExeFileName);

		std::string ñommand("start ");
		ñommand.append(newExeFileName);

		system(ñommand.c_str());

	} catch(std::exception ex) {
		exit(1);
	}
	
	exit(0);
}