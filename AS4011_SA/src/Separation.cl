//#pragma OPENCL EXTENSION cl_amd_printf : enable


__kernel void Separation(__global float4* pos, __local float3* tempSep, __global float4* sep) //, __global int* count)
{
	const int targetBody = get_global_id(0);
	const int otherBody = get_global_id(1);
	const int bodies = get_global_size(1);

	tempSep[bodies * targetBody + otherBody].x = pos[otherBody].x - pos[targetBody].x;
	tempSep[bodies * targetBody + otherBody].y = pos[otherBody].y - pos[targetBody].y;
	tempSep[bodies * targetBody + otherBody].z = pos[otherBody].z - pos[targetBody].z;
	
	barrier(CLK_LOCAL_MEM_FENCE);

	// Piecewise addition
	// Want to add pairs of numbers for each body until we are left with just one value
	// This value then gets dumped out!
	for (int positionInBody = bodies >> 1; positionInBody > 0; positionInBody >>= 1)
	{
		if(otherBody < positionInBody)
			tempSep[otherBody] += localData[otherBody + positionInBody];

		barrier(CLK_LOCAL_MEM_FENCE);
	}

	if(otherBody == 0)
		sep[targetBody] = tempSep[bodies * targetBody];
	
}