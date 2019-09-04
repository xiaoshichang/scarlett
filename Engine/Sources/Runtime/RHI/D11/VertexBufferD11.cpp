#include "Runtime/RHI/D11/VertexBufferD11.h"
#include "Foundation/Assert.h"

#include <d3d11.h>
using namespace scarlett;

scarlett::VertexBufferD11::VertexBufferD11(unsigned int count, VertexFormat vf):
	VertexBuffer(count, vf), mVertexBuffer(nullptr)
{
}
