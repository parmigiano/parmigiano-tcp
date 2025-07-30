#include "../include/autoUpdate.h"
#include "../include/connection.h"
#include "../include/acceptingResponses.h"
#include "../include/sendingRequests.h"
#include "../include/config.h"

#include <string>
#include <iostream>
#include <regex>
#include <fstream>
#include <libloaderapi.h>

std::map <std::string, std::string> filesToDownload;
bool newMainExe = false;

AutoUpdate::AutoUpdate(){
	_AcceptingResponses = std::make_shared<AcceptingResponses>();
	_SendingRequests = std::make_shared<SendingRequests>();
	_Connection = std::make_shared<Connection>();
	_Response = std::make_shared<ResponseStruct::Response>();

	GOOGLE_PROTOBUF_VERIFY_VERSION;
}

AutoUpdate::~AutoUpdate(){
	google::protobuf::ShutdownProtobufLibrary();
}

std::string AutoUpdate::getHashLatestBuildFromServer(){

	_SendingRequests->confirmRequest("fileHashRequest");
	std::string serverResponse = _AcceptingResponses->acceptingServerResponseJson();

	return serverResponse;
}

std::size_t AutoUpdate::hash_file(const std::string& filePath) {
	std::ifstream file(filePath, std::ios::binary);
	if (!file) return 0;

	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	return std::hash<std::string>{}(content);
}

int AutoUpdate::downloadFiles(){

	std::cout << "config exe name: " << Config::get_instance()->nameOfMainExeFile << std::endl;

	for (auto& [fileName, filePath] : filesToDownload) {
		// std::cout << fileName << filePath << std::endl;

		//std::string alo = GetModuleFileName.c_str();

		std::cout << "downloadFile path: " << filePath << std::endl;
		std::cout << "downloadFile name: " << filePath << std::endl;
		std::cout << std::endl;

		_SendingRequests->addFileInfoToRequest(fileName, filePath);
		_SendingRequests->confirmRequest("downloadFile");

		if (fileName == _Response->mutable_nameofmainexefile()->filename()) {
			_AcceptingResponses->acceptingFiles("temp_" + filePath);

			std::cout << "detected new main file" << std::endl;
			newMainExe = true;
			// Config::get_instance()->newNameOfMainExeFile = fileName;
			continue;
		}

		_AcceptingResponses->acceptingFiles(filePath);

		Sleep(1000);
	}

	return 0;
}

int AutoUpdate::startUpdater(){
	std::string exeParams("start updater ");
	exeParams.append(Config::get_instance()->nameOfMainExeFile + " " + _Response->mutable_nameofmainexefile()->filename() + " temp_" + _Response->mutable_nameofmainexefile()->filename());

	system(exeParams.c_str());

	exit(0);

	return 0;
}

int AutoUpdate::directoriesExist(std::shared_ptr<ResponseStruct::Response> actualFilesInfo){
	std::cout << "Dirs check" << std::endl;
	std::cout << actualFilesInfo->dirinfo_size() << std::endl;


	for (int i = 0; i < actualFilesInfo->dirinfo_size(); i++) {
		const ResponseStruct::DirInfo& fileInfo = actualFilesInfo->dirinfo(i);
		std::cout << fileInfo.dirpath() << std::endl;

		if (!std::filesystem::exists(fileInfo.dirpath())) {
			std::cout << "Created directory: " << fileInfo.dirpath() << std::endl;
			std::filesystem::create_directory(fileInfo.dirpath());
		}
	}

	return 0;
}

int AutoUpdate::comparisonHash(std::shared_ptr<ResponseStruct::Response> actualFilesInfo){

	for (int i = 0; i < actualFilesInfo->fileinfo_size(); i++) {
		const ResponseStruct::FileInfo& fileInfo = actualFilesInfo->fileinfo(i);

		if (!std::filesystem::exists(fileInfo.filepath()) || fileInfo.filehash() != std::to_string(hash_file(fileInfo.filepath()))) {
			filesToDownload[fileInfo.filename()] = fileInfo.filepath();
		}
	}

	return 0;
}

int AutoUpdate::checkUpdate(){
    _Connection->connectToServer("5462", "localhost");

	std::string data = getHashLatestBuildFromServer();

	/*ResponseStruct::Response actualFilesInfo;
	actualFilesInfo.ParseFromString(data);*/

	_Response->ParseFromString(data);

	comparisonHash(_Response);
	directoriesExist(_Response);

	if (!filesToDownload.empty()) { // files info isnt empty
		std::cout << "(!) needed to update client" << std::endl;

		return 1;
	}

	if (filesToDownload.empty()) { // files info is empty
		std::cout << "(!) client has latest version" << std::endl;
		_Connection->disconnectFromCurrentServer();

		return 0;
	}
}

int AutoUpdate::downloadUpdate() {
	Connection _connection;

	if (downloadFiles() == 0) {
		std::cout << "successful download update" << std::endl;
		_connection.disconnectFromCurrentServer();
		filesToDownload.clear();

		if (newMainExe == true) {
			startUpdater();
		}

		return 0;
	}
	else {
		std::cout << "failed download update" << std::endl;
		_connection.disconnectFromCurrentServer();
		filesToDownload.clear();

		return 1;
	}
}