#include "IndexBuffer.h"

namespace HBL {

	void IndexBuffer::Make_Indecies(uint32_t size)
	{
		index_buffer = (uint32_t*)malloc((size / 4) * 6 * sizeof(uint32_t));
		int w = 0;
		for (int k = 0; k < (size / 4) * 6; k += 6) {
			index_buffer[index++] = 0 + w;
			index_buffer[index++] = 3 + w;
			index_buffer[index++] = 2 + w;
			index_buffer[index++] = 2 + w;
			index_buffer[index++] = 1 + w;
			index_buffer[index++] = 0 + w;
			w += 4;
		}

	}

	void IndexBuffer::Clean()
	{
		if (index_buffer != NULL) free(index_buffer);
		index = 0;
	}

	uint32_t* IndexBuffer::Get_Index_Buffer()
	{
		return index_buffer;
	}

}