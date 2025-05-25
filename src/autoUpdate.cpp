#include "../include/autoUpdate.h"
#include "../include/connection.h"
#include "../include/acceptingResponses.h"
#include "../include/sendingRequests.h"

#include "iostream"
#include <regex>
//#include <algorithm>
#include <fstream> // temp
#include <vector> // temp

namespace fs = std::filesystem;

//std::string currentDownloadingFile = "";

//const size_t blockSize = 4096;

json filesToDownload;

std::string AutoUpdate::getHashLatestBuildFromServer(){
	AcceptingResponses _acceptingResponses;
	SendingRequests _sendingRequests;

	//_sendingRequests.sendRequest();
	_sendingRequests.confirmRequest("fileHashRequest");
	std::string serverResponse = _acceptingResponses.acceptingServerResponseJson();

	return serverResponse;
}

json AutoUpdate::getHashFromCurrentClientFiles(){
	json actualHashes;

	for (const auto& entry : fs::recursive_directory_iterator(fs::current_path())) {
		if (entry.is_regular_file()) {
			// std::cout << entry.path() << " - " << hash_file(entry.path().string()) << std::endl;
			actualHashes["fileHashes"][entry.path().lexically_normal().filename().string()] = hash_file(entry.path().string());
		}
	}

	return actualHashes;
}

std::size_t AutoUpdate::hash_file(const std::string& filePath) {
	std::ifstream file(filePath, std::ios::binary);
	if (!file) return 0;

	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	return std::hash<std::string>{}(content);
}

int AutoUpdate::downloadFiles(){
	SendingRequests _sendingRequests;
	AcceptingResponses _acceptingResponses;

	std::cout << filesToDownload << std::endl;

	if (!filesToDownload.contains("filesToDownload") && filesToDownload["filesToDownload"].is_object()) {
		return 1;
	}

	std::cout << "downloadFile: " << filesToDownload.dump() << std::endl;

	for (auto& [fileName, filePath] : filesToDownload["filesToDownload"].items()) {
		std::cout << "downloadFile: " << filePath << std::endl;

		_sendingRequests.addParameter("filesToDownload", fileName, filePath);
		_sendingRequests.confirmRequest("downloadFile");

		_sendingRequests.clearRequest();

		//json acceptedJson = json::parse(_acceptingResponses.acceptingServerResponseJson());

		//std::string acceptedBytes = _acceptingResponses.acceptingFiles();

		/*if (std::filesystem::exists(filePath)) {
			std::filesystem::remove(filePath);
		}*/

		_acceptingResponses.acceptingFiles(filePath);

		Sleep(1000);
	}

	//_sendingRequests.confirmRequest("downloadFile");
	//_sendingRequests.clearRequest();

	return 0;
}

int AutoUpdate::processAndSaveFile(int fileCreateFlag, std::string filePath, std::string fileName){
	AcceptingResponses _acceptingResponses;

	//std::string acceptedBytes = _acceptingResponses.acceptingFiles();
	return 0;
}

int AutoUpdate::directoriesExist(json actualHashes){

	for (const auto& [directoryName, directorypath] : actualHashes["directoriesPaths"].items()) {
		if (!std::filesystem::exists(directorypath)) {
			std::cout << "Created directory: " << directorypath << std::endl;
			std::filesystem::create_directory(directorypath);
		}
	}

	return 0;

	//std::error_code ec;
	//std::cout << filePath << std::endl;

	/*if (std::filesystem::exists(filePath)) {
		return 0;
	}
	else {
		return 1;
	}*/
}
int AutoUpdate::comparisonHash(json clientHashes, json actualHashes){

	for (const auto& [filename, actualHash] : actualHashes["fileHashes"].items()) {
		if (clientHashes["fileHashes"].contains(filename) && std::filesystem::exists(actualHashes["filePaths"][filename])) {
			if (clientHashes["fileHashes"][filename] != actualHash) {
				filesToDownload["filesToDownload"][filename] = actualHashes["filePaths"][filename]; // needs to update
			}
		}
		else {
			filesToDownload["filesToDownload"][filename] = actualHashes["filePaths"][filename]; // needs to install
		}
	}

	//std::cout << filesToDownload.dump() << std::endl;

	return 0;
}

int AutoUpdate::checkUpdate(){
    AcceptingResponses _acceptingResponses;
	SendingRequests _sendingRequests;
	Connection _connection;

    _connection.connectToServer("5462", "localhost");

	json actualHashes = json::parse(getHashLatestBuildFromServer());
	json clientHashes = getHashFromCurrentClientFiles();
	 
	//_sendingRequests.addParameter("filesToDownload", "fileName", "hashSum.dump()");
	//_sendingRequests.confirmRequest("downloadFile");


	comparisonHash(clientHashes, actualHashes);

	directoriesExist(actualHashes);

	if (!filesToDownload.empty()) {
		std::cout << "(!) needed to update client" << std::endl;
		return 1;
	}

	if (filesToDownload.empty()) {
		std::cout << "(!) client has latest version" << std::endl;
		_connection.disconnectFromCurrentServer();
		return 0;
	}
}

int AutoUpdate::downloadUpdate(){
	Connection _connection;

	//_connection.connectToServer("5462", "localhost");
	//std::cout << "(!) downloading update" << std::endl;

	downloadFiles();
	//acceptingFileFromServer();

	_connection.disconnectFromCurrentServer();


	return 0;
}