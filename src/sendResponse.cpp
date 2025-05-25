#include "../include/sendResponse.h"
#include "../include/connectionHandler.h"

#include <mswsock.h>
#include <regex>

#pragma comment(lib, "mswsock.lib")

int SendResponse::sendJSON(json j, std::string responseType){
	ConnectionHandler _ConnectionHandler;

	if (j.empty()) {
		std::cout << "(!) json is empty. Sending json response is stopped." << std::endl;
		return 1;
	}

	j["responseInfo"]["responseType"] = responseType;

	send(_ConnectionHandler.getCurrentConnectClientSocket(), j.dump().c_str(), (int)j.dump().size(), 0);

    std::cout << "(!) Sended: " << j.dump() << std::endl;
	return 0;
}

int SendResponse::sendFile(std::string filePath, std::string fileName){
    //json messageWhenSending;
    ConnectionHandler _ConnectionHandler;

    //filePath = std::regex_replace(filePath, std::regex(R"(\\\\)"), R"(\)");
    //filePath = std::regex_replace(filePath, std::regex(R"(\")"), "");

    filePath = ".\\latestClientBuild\\" + filePath;

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
        std::cerr << "Failed to open file handle with error: " << GetLastError() << std::endl;
        return 1;
    }

    LARGE_INTEGER fileSize;
    if (!GetFileSizeEx(hFile, &fileSize)) {
        std::cerr << "Failed to get file size with error: " << GetLastError() << std::endl;
        CloseHandle(hFile);
        return 1;
    }

    std::cout << "File size: " << fileSize.QuadPart << " bytes" << std::endl;

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
        _ConnectionHandler.getCurrentConnectClientSocket(),
        hFile,
        fileSize.QuadPart,
        0,
        nullptr,
        &tfb,
        0
    )) {
        std::cerr << "TransmitFile failed with error: " << GetLastError() << std::endl;
    }

    //std::cout << 2 << std::endl;


    //messageWhenSending.clear();
    CloseHandle(hFile);

    //std::cout << 3 << std::endl;


    send(_ConnectionHandler.getCurrentConnectClientSocket(), "end", 3, 0);

    return 0;
}
