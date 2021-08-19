#include "Configuration.h"

Configuration::Configuration()
{
}

Configuration::Configuration(int errorCorrection, std::string input, std::string output) : errorCorrection(errorCorrection), input(input), output(output)
{
}

Configuration::~Configuration()
{
}

void Configuration::setErrorCorrection(int errorCorrection)
{
	this->errorCorrection = errorCorrection;
}

void Configuration::setInput(std::string input)
{
	this->input = input;
}

void Configuration::setOutput(std::string output)
{
	this->output = output;
}

int Configuration::getErrorCorrection()
{
	return errorCorrection;
}

std::string Configuration::getInput()
{
	return input;
}

std::string Configuration::getOutput()
{
	return output;
}
