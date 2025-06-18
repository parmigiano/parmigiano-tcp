#include "../include/sendResponse.h"
//#include "../include/connectionHandler.h"

#include <mswsock.h>

#include <regex>

#pragma comment(lib, "mswsock.lib")

//SendResponse _SendResponse;
//ConnectionHandler _ConnectionHandler;

SendResponse::SendResponse(){
    _Logger = Logger::get_instance();
    _Config = Config::get_instance();
    
    //_ConnectionHandler = std::make_shared<ConnectionHandler>();
}

//SendResponse::~SendResponse(){
//    delete _Logger;
//    delete _Config;
//
//    if (_ConnectionHandler != nullptr) delete _ConnectionHandler;
//}

int SendResponse::sendJSON(json j, std::string responseType, SOCKET clientSocket){
	if (j.empty()) {
        _Logger->addLog("INFO", "json is empty. Sending json response is stopped.", 2);
		return 1;
	}

	j["responseInfo"]["responseType"] = responseType;

    //std::cout << j.dump() << std::endl;
    //std::cout << _ConnectionHandler.getCurrentConnectClientSocket() << std::endl;
    //std::cout << responseType << std::endl;

	send(clientSocket, j.dump().c_str(), (int)j.dump().size(), 0);

    //std::cout << "(!) Sended: " << j.dump() << std::endl;
	return 0;
}

int SendResponse::sendFile(std::string filePath, std::string fileName, SOCKET clientSocket){
    //json messageWhenSending;

    //filePath = std::regex_replace(filePath, std::regex(R"(\\\\)"), R"(\)");
    //filePath = std::regex_replace(filePath, std::regex(R"(\")"), "");

    //filePath = ".\\latestClientBuild\\" + filePath;
    filePath = _Config->lastClientBuildDir + "\\" + filePath;

    HANDLE hFile = CreateFileA(
        filePath.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        _Logger->addLog("ERROR", "Failed to open file handle with error: " + GetLastError(), 2);
        return 1;
    }

    LARGE_INTEGER fileSize;
    if (!GetFileSizeEx(hFile, &fileSize)) {
        _Logger->addLog("ERROR", "Failed to get file size with error: " + GetLastError(), 2);
        CloseHandle(hFile);
        return 1;
    }

    //std::cout << "File size: " << fileSize.QuadPart << " bytes" << std::endl;
    //_Logger.addLog("INFO", "File size (bytes): " + fileSize.QuadPart, 1);

    TRANSMIT_FILE_BUFFERS tfb;

    /*messageWhenSending["responseInfo"]["sendindStatus"] = "start";
    messageWhenSending["fileInfo"]["filePath"] = filePath;
    messageWhenSending["fileInfo"]["fileName"] = fileName;
    std::string headStr = messageWhenSending.dump();*/

    tfb.Head = nullptr;
    tfb.HeadLength = 0;

    /*messageWhenSending["responseInfo"]["sendindStatus"] = "end";
    std::string tailStr = messageWhenSending.dump();*/

    tfb.Tail = nullptr;
    tfb.TailLength = 0;

    //std::cout << 1 << std::endl;

    if (!TransmitFile(
        clientSocket,
        hFile,
        fileSize.QuadPart,
        0,
        nullptr,
        &tfb,
        0
    )) {
        _Logger->addLog("ERROR", "TransmitFile failed with error: " + GetLastError(), 2);
    }
    else {
        _Logger->addLog("INFO", "File successfully sended: " + filePath, 2);
    }

    //std::cout << 2 << std::endl;


    //messageWhenSending.clear();
    CloseHandle(hFile);

    //std::cout << 3 << std::endl;

    send(clientSocket, "end", 3, 0);

    return 0;
}
