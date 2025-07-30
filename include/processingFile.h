#ifndef PROCESSINGFILE_H
#define PROCESSINGFILE_H

class ProcessingFile {
private:
public:
	~ProcessingFile() = default;

	void replaceTempToMain(int argc, char** argv);
};

#endif 