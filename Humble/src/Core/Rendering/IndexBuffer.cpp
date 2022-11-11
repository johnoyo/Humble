#include "IndexBuffer.h"

namespace HBL {

	void IndexBuffer::Make_Indecies(uint32_t size)
	{
		m_IndexBuffer = (uint32_t*)malloc((size / 4u) * 6u * sizeof(uint32_t));
		int w = 0;
		for (int k = 0; k < (size / 4) * 6; k += 6) {
			m_IndexBuffer[m_Index++] = 0 + w;
			m_IndexBuffer[m_Index++] = 3 + w;
			m_IndexBuffer[m_Index++] = 2 + w;
			m_IndexBuffer[m_Index++] = 2 + w;
			m_IndexBuffer[m_Index++] = 1 + w;
			m_IndexBuffer[m_Index++] = 0 + w;
			w += 4;
		}

	}

	void IndexBuffer::Clean()
	{
		if (m_IndexBuffer != NULL) free(m_IndexBuffer);
		m_Index = 0;
	}

	uint32_t* IndexBuffer::Get_Index_Buffer()
	{
		return m_IndexBuffer;
	}

}