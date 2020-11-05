#define CL_USE_DEPRECATED_OPENCL_2_0_APIS

#include <CL/cl.hpp>
#include <string>
#include <fstream>
#include <iostream>

cl::Program CreateProgram(const std::string& file, cl_int& err)
{
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);

	_ASSERT(platforms.size() > 0);

	auto platform = platforms.front();
	std::vector<cl::Device> devices;
	platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

	_ASSERT(devices.size() > 0);
	std::cout << devices.size() << std::endl;

	cl::Device device = devices.front();


	// Load our cl file
	std::ifstream helloworldfile(file);
	std::string src((std::istreambuf_iterator<char>(helloworldfile)), (std::istreambuf_iterator<char>()));


	// Add the file to the cl sources vector
	cl::Program::Sources sources(1, std::make_pair(src.c_str(), src.length() + 1));

	// Make a program using this source

	cl::Context context(device);
	cl::Program program(context, sources);


	err = program.build("-cl-std=CL1.2");
	return program;
}