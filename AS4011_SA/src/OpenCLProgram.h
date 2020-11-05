#pragma once

#include <CL/cl.hpp>

cl::Program CreateProgram(const std::string& file, cl_int& err);