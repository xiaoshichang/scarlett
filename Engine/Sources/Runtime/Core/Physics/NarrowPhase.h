#pragma once
#include <vector>

#include "Runtime/Core/Math/ScltMath.h"
#include "Runtime/Core/Physics/BoardPhase.h"
#include "Runtime/Core/Physics/Contact.h"


namespace scarlett
{
	class RigidBodyComponent;

	class INarrowPhase
	{
	public:
		virtual void UpdateManifolds(std::vector<std::shared_ptr<ContactManifold>>& collisions) = 0;
		virtual void CollideDetection(std::vector<RigidBodyPair>& rigidBodies, std::vector<std::shared_ptr<ContactManifold>>& collisions) = 0;

	};


	struct sResults
	{
		enum eStatus
		{
			Separated,   /* Shapes doesnt penetrate												*/
			Penetrating, /* Shapes are penetrating												*/
			GJK_Failed,  /* GJK phase fail, no big issue, shapes are probably just 'touching'	*/
			EPA_Failed   /* EPA phase fail, bigger problem, need to save parameters, and debug	*/
		} status;

		Vector3f witnessInGlobal[2];
		Vector3f witnessesInFirstLocal[2];
		Vector3f normal;
		float distance;
	};

	struct MinkowskiDiff
	{
		RigidBodyComponent* box1;
		RigidBodyComponent* box2;

		inline Vector3f Support1(Vector3f& dir);
		inline Vector3f Support2(Vector3f& dir);
		Vector3f Support(Vector3f& dir);
		Vector3f Support(Vector3f& dir, int idx);
	};

	class NarrowPhaseGJKEPA: public INarrowPhase
	{
		virtual void UpdateManifolds(std::vector<std::shared_ptr<ContactManifold>>& collisions);
		virtual void CollideDetection(std::vector<RigidBodyPair>& rigidBodies, std::vector<std::shared_ptr<ContactManifold>>&);

		void InitializeMinkowskiDiff(RigidBodyPair& pair, sResults& result, MinkowskiDiff& diff);
		bool Penetration(RigidBodyPair& pair, Vector3f& guess, sResults& result);
		void GenerateTangents(ContactPoint& contactPoint);

	};

}