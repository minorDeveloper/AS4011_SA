#define CL_USE_DEPRECATED_OPENCL_2_0_APIS

#include <iostream>
#include <fstream>

#include <CL/cl.hpp>

#include "OpenCLProgram.h"

int main()
{


	cl_int err;
	auto program = CreateProgram("src/Separation.cl", err);

	auto context = program.getInfo<CL_PROGRAM_CONTEXT>();
	auto devices = context.getInfo<CL_CONTEXT_DEVICES>();
	auto& device = devices.front();


	const int bodies = 32;
	std::vector<cl_float4> pos;

	// Temp filling the vector for testing
	cl_float4 tempFloat;
	for (int i = 0; i < bodies; i++)
	{
		tempFloat.x = ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
		tempFloat.y = ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
		tempFloat.z = ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
		tempFloat.w = 0.0f;
		pos.push_back(tempFloat);
	}


	std::vector<cl_float4> sep(pos.size());

	cl::Kernel kernel(program, "Separation", &err);

	auto workGroupSize = kernel.getWorkGroupInfo<CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE>(device);



	cl::Buffer buf(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(cl_float4) * pos.size(), pos.data());
	cl::Buffer outBuf(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(cl_float4) * sep.size());

	kernel.setArg(0, buf);
	kernel.setArg(1, sizeof(cl_float3) * pos.size() * pos.size(), nullptr);
	kernel.setArg(2, outBuf);
	//kernel.setArg(2, countBuf);

	cl::CommandQueue queue(context, device);
	queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(pos.size(), pos.size()));
	queue.enqueueReadBuffer(outBuf, GL_TRUE, 0, sizeof(cl_float3) * sep.size(), sep.data());

	std::cin.get();
}