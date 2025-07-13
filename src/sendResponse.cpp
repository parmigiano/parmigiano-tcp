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
    
    _Response = std::make_shared<ResponseStruct::Response>();
}

void SendResponse::setReponseType(std::string responseType){
    if (responseType == "filesHashes") {
        _Response->mutable_responseinfo()->set_type(ResponseStruct::ResponseInfo::filesHashes);
    }
    else if (responseType == "downloadFile") {
        _Response->mutable_responseinfo()->set_type(ResponseStruct::ResponseInfo::downloadFile);
    }
    else {
        _Logger->addLog("WARN", "unknown response type", 2);
    }
}

void SendResponse::addFileInfo(std::string hash, std::string path, std::string name){
    auto* fileInfo = _Response->add_fileinfo();

    fileInfo->set_filehash(hash);
    fileInfo->set_filepath(path);
    fileInfo->set_filename(name);
}

void SendResponse::addDirInfo(std::string dirPath){
    auto* dirInfo = _Response->add_dirinfo();

    dirInfo->set_dirpath(dirPath);
}

void SendResponse::clearResponse(){
    _Response->Clear();
}

int SendResponse::sendResponse(SOCKET clientSocket){
    std::string data;

    if (!_Response->SerializeToString(&data)) {
        std::cerr << "Filed to serialize request" << std::endl;
    }

    if (_Response->responseinfo().type() == ResponseStruct::ResponseInfo::unknown) {
        std::cerr << "Request will not sending with type requestInfo->requestType: unknown" << std::endl;
        return 1;
    }

    if (send(clientSocket, data.c_str(), (int)data.size(), 0) == SOCKET_ERROR) {
        std::cerr << "Send failed ith error: " << WSAGetLastError() << std::endl;
    }

    clearResponse();

	return 0;
}

int SendResponse::sendFile(std::string filePath, std::string fileName, SOCKET clientSocket){
    //json messageWhenSending;

    //filePath = std::regex_replace(filePath, std::regex(R"(\\\\)"), R"(\)");
    //filePath = std::regex_replace(filePath, std::regex(R"(\")"), "");

    //filePath = ".\\latestClientBuild\\" + filePath;
    filePath = _Config->buildDir + "/" + filePath;

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
