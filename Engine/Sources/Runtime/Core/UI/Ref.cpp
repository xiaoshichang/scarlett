#include "Ref.h"
#include "AutoReleasePool.h"

using namespace scarlett;

Ref::Ref()
	: _referenceCount(1)
{}


Ref::~Ref() {
	
}

void Ref::Retain() {
	if (_referenceCount <= 0) {

	}
	++_referenceCount;
}

void Ref::Release() {
	if (_referenceCount <= 0) {

	}
	--_referenceCount;

	if (_referenceCount == 0) {
		// release it
		delete this;
	}
}

Ref* Ref::AutoRelease() {
	AutoreleasePool::GetInstance()->AddObject(this);
	return this;
}

unsigned int Ref::GetReferenceCount() const
{
	return _referenceCount;
}