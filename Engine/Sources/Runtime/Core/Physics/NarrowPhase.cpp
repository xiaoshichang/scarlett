#include "NarrowPhase.h"
#include "Runtime/Core/Object/Entity.h"
#include "Runtime/Core/Object/Components/RigidBodyComponent.h"
#include "Runtime/Core/Object/Components/TransformComponent.h"


#define GJK_MAX_ITERATIONS 128
#define GJK_ACCURACY ((float)0.0001)
#define GJK_MIN_DISTANCE ((float)0.0001)
#define GJK_DUPLICATED_EPS ((float)0.0001)
#define GJK_SIMPLEX2_EPS ((float)0.0)
#define GJK_SIMPLEX3_EPS ((float)0.0)
#define GJK_SIMPLEX4_EPS ((float)0.0)


using namespace scarlett;

typedef unsigned int U;
typedef unsigned char U1;
typedef MinkowskiDiff tShape;


/*
先把dir从世界坐标转换成局部坐标，然后求解局部支撑点，再转回世界坐标
*/
inline Vector3f scarlett::MinkowskiDiff::Support1(Vector3f & dir)
{
	TransformComponent* transform = box1->GetMaster()->GetComponent<TransformComponent>();
	Vector3f localDir = transform->GlobalVectorToLocalVector(dir);
	Vector3f localSupportPoint = box1->GetAABB()->LocalGetSupportVertex(localDir);
	Vector3f globalSupportPoint = transform->LocalPointToGlobalPoint(localSupportPoint);
	return globalSupportPoint;
}

inline Vector3f scarlett::MinkowskiDiff::Support2(Vector3f & dir)
{
	TransformComponent* transform = box2->GetMaster()->GetComponent<TransformComponent>();
	Vector3f localDir = transform->GlobalVectorToLocalVector(dir);
	Vector3f localSupportPoint = box2->GetAABB()->LocalGetSupportVertex(localDir);
	Vector3f globalSupportPoint = transform->LocalPointToGlobalPoint(localSupportPoint);
	return globalSupportPoint;
}

Vector3f scarlett::MinkowskiDiff::Support(Vector3f& dir)
{
	return (Support1(dir) - Support2(dir * -1));
}


struct GJK
{
	struct sSV
	{
		Vector3f d, w;
	};
	struct sSimplex
	{
		sSV* c[4];
		float p[4];
		U rank;
	};
	struct eStatus
	{
		enum _
		{
			Valid,
			Inside,
			Failed
		};
	};
	/* Fields		*/
	MinkowskiDiff m_shape;
	Vector3f m_ray;
	float m_distance;
	sSimplex m_simplices[2];
	sSV m_store[4];
	sSV* m_free[4];
	U m_nfree;
	U m_current;
	sSimplex* m_simplex;
	eStatus::_ m_status;
	/* Methods		*/
	GJK()
	{
		Initialize();
	}
	void Initialize()
	{
		m_ray = Vector3f(0, 0, 0);
		m_nfree = 0;
		m_status = eStatus::Failed;
		m_current = 0;
		m_distance = 0;
	}
	eStatus::_ Evaluate(const tShape& shapearg, const Vector3f& guess)
	{
		U iterations = 0;
		float sqdist = 0;
		float alpha = 0;
		Vector3f lastw[4];
		U clastw = 0;
		/* Initialize solver		*/
		m_free[0] = &m_store[0];
		m_free[1] = &m_store[1];
		m_free[2] = &m_store[2];
		m_free[3] = &m_store[3];
		m_nfree = 4;
		m_current = 0;
		m_status = eStatus::Valid;
		m_shape = shapearg;
		m_distance = 0;
		/* Initialize simplex		*/
		m_simplices[0].rank = 0;
		m_ray = guess;
		const float sqrl = m_ray.length2();
		appendvertice(m_simplices[0], sqrl > 0 ?  m_ray * -1 : Vector3f(1, 0, 0));
		m_simplices[0].p[0] = 1;
		m_ray = m_simplices[0].c[0]->w;
		sqdist = sqrl;
		lastw[0] =
			lastw[1] =
			lastw[2] =
			lastw[3] = m_ray;
		/* Loop						*/
		do
		{
			const U next = 1 - m_current;
			sSimplex& cs = m_simplices[m_current];
			sSimplex& ns = m_simplices[next];
			/* Check zero							*/
			const float rl = m_ray.length();
			if (rl < GJK_MIN_DISTANCE)
			{ /* Touching or inside				*/
				m_status = eStatus::Inside;
				break;
			}
			/* Append new vertice in -'v' direction	*/
			appendvertice(cs,  m_ray * -1);
			const Vector3f& w = cs.c[cs.rank - 1]->w;
			bool found = false;
			for (U i = 0; i < 4; ++i)
			{
				Vector3f diff;
				VectorSub(diff, w, lastw[i]);
				if (diff.length2() < GJK_DUPLICATED_EPS)
				{
					found = true;
					break;
				}
			}
			if (found)
			{ /* Return old simplex				*/
				removevertice(m_simplices[m_current]);
				break;
			}
			else
			{ /* Update lastw					*/
				lastw[clastw = (clastw + 1) & 3] = w;
			}
			/* Check for termination				*/
			const float omega = DotProduct(m_ray, w) / rl;
			alpha = omega > alpha ? omega : alpha;
			if (((rl - alpha) - (GJK_ACCURACY * rl)) <= 0)
			{ /* Return old simplex				*/
				removevertice(m_simplices[m_current]);
				break;
			}
			/* Reduce simplex						*/
			float weights[4];
			U mask = 0;
			switch (cs.rank)
			{
			case 2:
				sqdist = projectorigin(cs.c[0]->w,
					cs.c[1]->w,
					weights, mask);
				break;
			case 3:
				sqdist = projectorigin(cs.c[0]->w,
					cs.c[1]->w,
					cs.c[2]->w,
					weights, mask);
				break;
			case 4:
				sqdist = projectorigin(cs.c[0]->w,
					cs.c[1]->w,
					cs.c[2]->w,
					cs.c[3]->w,
					weights, mask);
				break;
			}
			if (sqdist >= 0)
			{ /* Valid	*/
				ns.rank = 0;
				m_ray = Vector3f(0, 0, 0);
				m_current = next;
				for (U i = 0, ni = cs.rank; i < ni; ++i)
				{
					if (mask & (1 << i))
					{
						ns.c[ns.rank] = cs.c[i];
						ns.p[ns.rank++] = weights[i];
						m_ray = m_ray + cs.c[i]->w * weights[i];
					}
					else
					{
						m_free[m_nfree++] = cs.c[i];
					}
				}
				if (mask == 15) m_status = eStatus::Inside;
			}
			else
			{ /* Return old simplex				*/
				removevertice(m_simplices[m_current]);
				break;
			}
			m_status = ((++iterations) < GJK_MAX_ITERATIONS) ? m_status : eStatus::Failed;
		} while (m_status == eStatus::Valid);
		m_simplex = &m_simplices[m_current];
		switch (m_status)
		{
		case eStatus::Valid:
			m_distance = m_ray.length();
			break;
		case eStatus::Inside:
			m_distance = 0;
			break;
		default:
		{
		}
		}
		return (m_status);
	}
	bool EncloseOrigin()
	{
		switch (m_simplex->rank)
		{
		case 1:
		{
			for (U i = 0; i < 3; ++i)
			{
				Vector3f axis = Vector3f(0, 0, 0);
				axis[i] = 1;
				appendvertice(*m_simplex, axis);
				if (EncloseOrigin()) return (true);
				removevertice(*m_simplex);
				appendvertice(*m_simplex, axis * -1);
				if (EncloseOrigin()) return (true);
				removevertice(*m_simplex);
			}
		}
		break;
		case 2:
		{
			const Vector3f d = m_simplex->c[1]->w - m_simplex->c[0]->w;
			for (U i = 0; i < 3; ++i)
			{
				Vector3f axis = Vector3f(0, 0, 0);
				axis[i] = 1;
				Vector3f p = CrossProduct(d, axis);
				if (p.length2() > 0)
				{
					appendvertice(*m_simplex, p);
					if (EncloseOrigin()) return (true);
					removevertice(*m_simplex);
					appendvertice(*m_simplex, p * -1);
					if (EncloseOrigin()) return (true);
					removevertice(*m_simplex);
				}
			}
		}
		break;
		case 3:
		{
			Vector3f n = CrossProduct(m_simplex->c[1]->w - m_simplex->c[0]->w,
				m_simplex->c[2]->w - m_simplex->c[0]->w);
			if (n.length2() > 0)
			{
				appendvertice(*m_simplex, n);
				if (EncloseOrigin()) return (true);
				removevertice(*m_simplex);
				appendvertice(*m_simplex, n * -1);
				if (EncloseOrigin()) return (true);
				removevertice(*m_simplex);
			}
		}
		break;
		case 4:
		{
			if (abs(det(m_simplex->c[0]->w - m_simplex->c[3]->w,
				m_simplex->c[1]->w - m_simplex->c[3]->w,
				m_simplex->c[2]->w - m_simplex->c[3]->w)) > 0)
				return (true);
		}
		break;
		}
		return (false);
	}
	/* Internals	*/
	void getsupport(Vector3f& d, sSV& sv)
	{
		sv.d = d * (1 / d.length());
		sv.w = m_shape.Support(sv.d);
	}
	void removevertice(sSimplex& simplex)
	{
		m_free[m_nfree++] = simplex.c[--simplex.rank];
	}
	void appendvertice(sSimplex& simplex, Vector3f& v)
	{
		simplex.p[simplex.rank] = 0;
		simplex.c[simplex.rank] = m_free[--m_nfree];
		getsupport(v, *simplex.c[simplex.rank++]);
	}
	static float det(const Vector3f& a, const Vector3f& b, const Vector3f& c)
	{
		return (a.y * b.z * c.x + a.z * b.x * c.y -
			a.x * b.z * c.y - a.y * b.x * c.z +
			a.x * b.y * c.z - a.z * b.y * c.x);
	}
	static float projectorigin(Vector3f& a,
		Vector3f& b,
		float* w, U& m)
	{
		Vector3f d = b - a;
		float l = d.length2();
		if (l > GJK_SIMPLEX2_EPS)
		{
			const float t(l > 0 ? -DotProduct(a, d) / l : 0);
			if (t >= 1)
			{
				w[0] = 0;
				w[1] = 1;
				m = 2;
				return (b.length2());
			}
			else if (t <= 0)
			{
				w[0] = 1;
				w[1] = 0;
				m = 1;
				return (a.length2());
			}
			else
			{
				w[0] = 1 - (w[1] = t);
				m = 3;
				return ((a + d * t).length2());
			}
		}
		return (-1);
	}
	static float projectorigin(Vector3f& a,
		Vector3f& b,
		Vector3f& c,
		float* w, U& m)
	{
		static const U imd3[] = { 1, 2, 0 };
		Vector3f* vt[] = { &a, &b, &c };
		Vector3f dl[] = { a - b, b - c, c - a };
		Vector3f n = CrossProduct(dl[0], dl[1]);
		const float l = n.length2();
		if (l > GJK_SIMPLEX3_EPS)
		{
			float mindist = -1;
			float subw[2] = { 0.f, 0.f };
			U subm(0);
			for (U i = 0; i < 3; ++i)
			{
				if (DotProduct(*vt[i], CrossProduct(dl[i], n)) > 0)
				{
					const U j = imd3[i];
					const float subd(projectorigin(*vt[i], *vt[j], subw, subm));
					if ((mindist < 0) || (subd < mindist))
					{
						mindist = subd;
						m = static_cast<U>(((subm & 1) ? 1 << i : 0) + ((subm & 2) ? 1 << j : 0));
						w[i] = subw[0];
						w[j] = subw[1];
						w[imd3[j]] = 0;
					}
				}
			}
			if (mindist < 0)
			{
				const float d = DotProduct(a, n);
				const float s = sqrt(l);
				Vector3f p = n * (d / l);
				mindist = p.length2();
				m = 7;
				w[0] = (CrossProduct(dl[1], b - p)).length() / s;
				w[1] = (CrossProduct(dl[2], c - p)).length() / s;
				w[2] = 1 - (w[0] + w[1]);
			}
			return (mindist);
		}
		return (-1);
	}
	static float projectorigin(Vector3f& a,
		Vector3f& b,
		Vector3f& c,
		Vector3f& d,
		float* w, U& m)
	{
		static const U imd3[] = { 1, 2, 0 };
		Vector3f* vt[] = { &a, &b, &c, &d };
		Vector3f dl[] = { a - d, b - d, c - d };
		float vl = det(dl[0], dl[1], dl[2]);
		bool ng = (vl * DotProduct(a, CrossProduct(b - c, a - b))) <= 0;
		if (ng && (abs(vl) > GJK_SIMPLEX4_EPS))
		{
			float mindist = -1;
			float subw[3] = { 0.f, 0.f, 0.f };
			U subm(0);
			for (U i = 0; i < 3; ++i)
			{
				const U j = imd3[i];
				const float s = vl * DotProduct(d, CrossProduct(dl[i], dl[j]));
				if (s > 0)
				{
					const float subd = projectorigin(*vt[i], *vt[j], d, subw, subm);
					if ((mindist < 0) || (subd < mindist))
					{
						mindist = subd;
						m = static_cast<U>((subm & 1 ? 1 << i : 0) +
							(subm & 2 ? 1 << j : 0) +
							(subm & 4 ? 8 : 0));
						w[i] = subw[0];
						w[j] = subw[1];
						w[imd3[j]] = 0;
						w[3] = subw[2];
					}
				}
			}
			if (mindist < 0)
			{
				mindist = 0;
				m = 15;
				w[0] = det(c, b, d) / vl;
				w[1] = det(a, c, d) / vl;
				w[2] = det(b, a, d) / vl;
				w[3] = 1 - (w[0] + w[1] + w[2]);
			}
			return (mindist);
		}
		return (-1);
	}
};


/*
rigidBodiesPairs 所有可能发生碰撞的刚体对
collisions 实际的碰撞结果
*/
void scarlett::NarrowPhaseGJKEPA::CollideDetection(std::vector<RigidBodyPair>& rigidBodiesPairs, std::vector<ContactManifold*>& collisions)
{
	collisions.clear();;
	for each (RigidBodyPair pair in rigidBodiesPairs)
	{
		Vector3f position1 = pair.first->GetMaster()->GetMaster()->GetComponent<TransformComponent>()->GetPosition();
		Vector3f position2 = pair.second->GetMaster()->GetMaster()->GetComponent<TransformComponent>()->GetPosition();
		Vector3f guess = position1 - position2;
		sResults result;
		if (Penetration(pair, guess, result))
		{
		
		}
	}
}

void scarlett::NarrowPhaseGJKEPA::InitializeMinkowskiDiff(RigidBodyPair& pair, sResults & result, MinkowskiDiff & diff)
{
	// result
	result.witnesses[0] = result.witnesses[1] = Vector3f(0, 0, 0);
	result.status = sResults::Separated;

	//minkowski
	diff.box1 = pair.first->GetMaster();
	diff.box2 = pair.second->GetMaster();
}

/*
guess 是 second指向first的向量
*/
bool scarlett::NarrowPhaseGJKEPA::Penetration(RigidBodyPair& pair, Vector3f & guess, sResults & result)
{
	MinkowskiDiff shape;
	InitializeMinkowskiDiff(pair, result, shape);

	// gjk 算法求解
	GJK gjk;
	GJK::eStatus::_ gjk_status = gjk.Evaluate(shape, guess);

	switch (gjk_status)
	{

	case GJK::eStatus::Inside:
	{
		return true;
	}
	case GJK::eStatus::Failed:
	{
		break;
	}
	default:
	{
		break;
	}

	}
	return false;
}


