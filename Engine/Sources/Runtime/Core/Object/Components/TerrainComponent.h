#pragma once
#include <unordered_map>
#include <vector>
#include "Runtime//Core/Math/ScltMath.h"
#include "Runtime/Interface/IComponent.h"
#include "Runtime/RHI/Mesh.h"


namespace scarlett {

	class CameraComponent;

	class TerrainUnit {
	public:
		TerrainUnit();
		~TerrainUnit();

		void UpdateVisibility(CameraComponent* camera);
		void UpdateLod(CameraComponent* camera);
		void SetOffset(const Vector3f& offset);

		void InitalizeRenderResourceHigh();
		void InitalizeRenderResourceLow();

		void InitializeRenderResource();
		void FinalizeRenderResource();

	public:
		Vector3f		mOffset;
		TerrainLod		mLod;
		bool			mVisible;

		std::shared_ptr<IMesh>		mMeshHigh;
		std::shared_ptr<IMesh>		mMeshLow;
	};

	class TerrainComponent : public IComponent {
	public:
		TerrainComponent();
		virtual ~TerrainComponent();

		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;
		virtual void Tick() noexcept;

		void LoadResource();

		// update visibility of all terrain unit
		void UpdateVisibility(CameraComponent* camera);

		// update lod of all terrain unit
		void UpdateLod(CameraComponent* camera);

		void Render();

	private:
		std::vector<std::shared_ptr<TerrainUnit>> mUnits;
	};

}