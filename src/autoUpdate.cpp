#include "../include/autoUpdate.h"
#include "../include/connection.h"
#include "../include/acceptingResponses.h"
#include "../include/sendingRequests.h"

#include <string>
#include <iostream>
#include <regex>
#include <fstream>

std::map <std::string, std::string> filesToDownload; 

AutoUpdate::AutoUpdate(){
	_AcceptingResponses = std::make_shared<AcceptingResponses>();
	_SendingRequests = std::make_shared<SendingRequests>();
	_Connection = std::make_shared<Connection>();

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

	for (auto& [fileName, filePath] : filesToDownload) {
		std::cout << fileName << filePath << std::endl;

		std::cout << "downloadFile: " << filePath << std::endl;

		_SendingRequests->addFileInfoToRequest(fileName, filePath);
		_SendingRequests->confirmRequest("downloadFile");

		_AcceptingResponses->acceptingFiles(filePath);

		Sleep(1000);
	}

	filesToDownload.clear();

	return 0;
}

int AutoUpdate::directoriesExist(ResponseStruct::Response actualFilesInfo){
	std::cout << "Dirs check" << std::endl;
	std::cout << actualFilesInfo.dirinfo_size() << std::endl;

	for (int i = 0; i < actualFilesInfo.dirinfo_size(); i++) {
		const ResponseStruct::DirInfo& fileInfo = actualFilesInfo.dirinfo(i);
		std::cout << fileInfo.dirpath() << std::endl;

		if (!std::filesystem::exists(fileInfo.dirpath())) {
			std::cout << "Created directory: " << fileInfo.dirpath() << std::endl;
			std::filesystem::create_directory(fileInfo.dirpath());
		}
	}

	return 0;
}

int AutoUpdate::comparisonHash(ResponseStruct::Response actualFilesInfo){

	for (int i = 0; i < actualFilesInfo.fileinfo_size(); i++) {
		const ResponseStruct::FileInfo& fileInfo = actualFilesInfo.fileinfo(i);

		std::cout << fileInfo.filename() << "\t" << fileInfo.filepath() << "\t" << fileInfo.filehash() << std::endl; // debug

		if (!std::filesystem::exists(fileInfo.filename()) || fileInfo.filehash() != std::to_string(hash_file(fileInfo.filepath()))) {
			filesToDownload[fileInfo.filename()] = fileInfo.filepath();
		}
	}

	return 0;
}

int AutoUpdate::checkUpdate(){
    _Connection->connectToServer("5462", "localhost");

	std::string data = getHashLatestBuildFromServer();

	ResponseStruct::Response actualFilesInfo;
	actualFilesInfo.ParseFromString(data);

	comparisonHash(actualFilesInfo); 
	directoriesExist(actualFilesInfo); 

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

		return 0;
	}
	else {
		std::cout << "failed download update" << std::endl;
		_connection.disconnectFromCurrentServer();

		return 1;
	}
}