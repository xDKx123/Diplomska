#pragma once

#include <string>

class Configuration
{
private:
	int errorCorrection;
	std::string input, output;

public:
	Configuration();
	Configuration(int errorCorrection, std::string input, std::string output);
	~Configuration();

	void setErrorCorrection(int errorCorrection);
	void setInput(std::string input);
	void setOutput(std::string output);

	int getErrorCorrection();
	std::string getInput();
	std::string getOutput();
};

