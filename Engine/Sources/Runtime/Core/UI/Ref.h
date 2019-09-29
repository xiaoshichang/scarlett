#pragma once

namespace scarlett {
	
	class Ref {
	public:
		void Retain();
		void Release();

		Ref* AutoRelease();
		unsigned int GetReferenceCount() const;

	protected:
		Ref();
	public:
		virtual ~Ref();

	protected:
		unsigned int _referenceCount;

		friend class AutoreleasePool;

	};


}