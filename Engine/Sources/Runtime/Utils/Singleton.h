#pragma once

namespace scarlett {
	template <typename T>
	class Singleton {
	private:
		static T* pInstance;

	public:
		static T* GetInstance() {
			if (nullptr == pInstance) {
				pInstance = new T();
			}
			return pInstance;
		}

	};


	template <typename T>
	T* Singleton<T>::pInstance = nullptr;
}

