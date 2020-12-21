#include "NarrowPhase.h"
#include "Runtime/Core/Object/Entity.h"
#include "Runtime/Core/Object/Components/RigidBodyComponent.h"
#include "Runtime/Core/Object/Components/TransformComponent.h"
#include "Runtime/Utils/Logging.h"


#define GJK_MAX_ITERATIONS 128
#define GJK_ACCURACY ((float)0.0001)
#define GJK_MIN_DISTANCE ((float)0.0001)
#define GJK_DUPLICATED_EPS ((float)0.0001)
#define GJK_SIMPLEX2_EPS ((float)0.0)
#define GJK_SIMPLEX3_EPS ((float)0.0)
#define GJK_SIMPLEX4_EPS ((float)0.0)

#define EPA_ACCURACY ((float)0.0001)
#define EPA_PLANE_EPS ((float)0.00001)
#define EPA_INSIDE_EPS ((float)0.01)
#define EPA_MAX_VERTICES 128
#define EPA_MAX_ITERATIONS 255
#define EPA_FALLBACK (10 * EPA_ACCURACY)
#define EPA_MAX_FACES (EPA_MAX_VERTICES * 2)

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

Vector3f scarlett::MinkowskiDiff::Support(Vector3f& dir, int idx)
{
	if (idx == 1)
	{
		return (Support1(dir));
	}
	else
	{
		return (Support2(dir));
	}
}


////////////////////////////////////////////// GJK //////////////////////


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


template < typename T>
void btSwap(T &a, T &b)
{
   T tmp = a;
   a = b;
   b = tmp;
}
////////////////////////////////////////////// GJK //////////////////////




////////////////////////////////////////////// EPA //////////////////////
struct EPA
{
	/* Types		*/
	typedef GJK::sSV sSV;
	struct sFace
	{
		Vector3f n;
		float d;
		sSV* c[3];
		sFace* f[3];
		sFace* l[2];
		U1 e[3];
		U1 pass;
	};
	struct sList
	{
		sFace* root;
		U count;
		sList() : root(0), count(0) {}
	};
	struct sHorizon
	{
		sFace* cf;
		sFace* ff;
		U nf;
		sHorizon() : cf(0), ff(0), nf(0) {}
	};
	struct eStatus
	{
		enum _
		{
			Valid,
			Touching,
			Degenerated,
			NonConvex,
			InvalidHull,
			OutOfFaces,
			OutOfVertices,
			AccuraryReached,
			FallBack,
			Failed
		};
	};
	/* Fields		*/
	eStatus::_ m_status;
	GJK::sSimplex m_result;
	Vector3f m_normal;
	float m_depth;
	sSV m_sv_store[EPA_MAX_VERTICES];
	sFace m_fc_store[EPA_MAX_FACES];
	U m_nextsv;
	sList m_hull;
	sList m_stock;
	/* Methods		*/
	EPA()
	{
		Initialize();
	}

	static inline void bind(sFace* fa, U ea, sFace* fb, U eb)
	{
		fa->e[ea] = (U1)eb;
		fa->f[ea] = fb;
		fb->e[eb] = (U1)ea;
		fb->f[eb] = fa;
	}
	static inline void append(sList& list, sFace* face)
	{
		face->l[0] = 0;
		face->l[1] = list.root;
		if (list.root) list.root->l[0] = face;
		list.root = face;
		++list.count;
	}
	static inline void remove(sList& list, sFace* face)
	{
		if (face->l[1]) face->l[1]->l[0] = face->l[0];
		if (face->l[0]) face->l[0]->l[1] = face->l[1];
		if (face == list.root) list.root = face->l[1];
		--list.count;
	}

	void Initialize()
	{
		m_status = eStatus::Failed;
		m_normal = Vector3f(0, 0, 0);
		m_depth = 0;
		m_nextsv = 0;
		for (U i = 0; i < EPA_MAX_FACES; ++i)
		{
			append(m_stock, &m_fc_store[EPA_MAX_FACES - i - 1]);
		}
	}
	eStatus::_ Evaluate(GJK& gjk, Vector3f& guess)
	{
		GJK::sSimplex& simplex = *gjk.m_simplex;
		if ((simplex.rank > 1) && gjk.EncloseOrigin())
		{
			/* Clean up				*/
			while (m_hull.root)
			{
				sFace* f = m_hull.root;
				remove(m_hull, f);
				append(m_stock, f);
			}
			m_status = eStatus::Valid;
			m_nextsv = 0;
			/* Orient simplex		*/
			if (gjk.det(simplex.c[0]->w - simplex.c[3]->w,
				simplex.c[1]->w - simplex.c[3]->w,
				simplex.c[2]->w - simplex.c[3]->w) < 0)
			{
				btSwap(simplex.c[0], simplex.c[1]);
				btSwap(simplex.p[0], simplex.p[1]);
			}
			/* Build initial hull	*/
			sFace* tetra[] = { newface(simplex.c[0], simplex.c[1], simplex.c[2], true),
							  newface(simplex.c[1], simplex.c[0], simplex.c[3], true),
							  newface(simplex.c[2], simplex.c[1], simplex.c[3], true),
							  newface(simplex.c[0], simplex.c[2], simplex.c[3], true) };
			if (m_hull.count == 4)
			{
				sFace* best = findbest();
				sFace outer = *best;
				U pass = 0;
				U iterations = 0;
				bind(tetra[0], 0, tetra[1], 0);
				bind(tetra[0], 1, tetra[2], 0);
				bind(tetra[0], 2, tetra[3], 0);
				bind(tetra[1], 1, tetra[3], 2);
				bind(tetra[1], 2, tetra[2], 1);
				bind(tetra[2], 2, tetra[3], 1);
				m_status = eStatus::Valid;
				for (; iterations < EPA_MAX_ITERATIONS; ++iterations)
				{
					if (m_nextsv < EPA_MAX_VERTICES)
					{
						sHorizon horizon;
						sSV* w = &m_sv_store[m_nextsv++];
						bool valid = true;
						best->pass = (U1)(++pass);
						gjk.getsupport(best->n, *w);
						const float wdist = DotProduct(best->n, w->w) - best->d;
						if (wdist > EPA_ACCURACY)
						{
							for (U j = 0; (j < 3) && valid; ++j)
							{
								valid &= expand(pass, w,
									best->f[j], best->e[j],
									horizon);
							}
							if (valid && (horizon.nf >= 3))
							{
								bind(horizon.cf, 1, horizon.ff, 2);
								remove(m_hull, best);
								append(m_stock, best);
								best = findbest();
								outer = *best;
							}
							else
							{
								m_status = eStatus::InvalidHull;
								break;
							}
						}
						else
						{
							m_status = eStatus::AccuraryReached;
							break;
						}
					}
					else
					{
						m_status = eStatus::OutOfVertices;
						break;
					}
				}
				const Vector3f projection = outer.n * outer.d;
				m_normal = outer.n;
				m_depth = outer.d;
				m_result.rank = 3;
				m_result.c[0] = outer.c[0];
				m_result.c[1] = outer.c[1];
				m_result.c[2] = outer.c[2];
				m_result.p[0] = CrossProduct(outer.c[1]->w - projection,
					outer.c[2]->w - projection)
					.length();
				m_result.p[1] = CrossProduct(outer.c[2]->w - projection,
					outer.c[0]->w - projection)
					.length();
				m_result.p[2] = CrossProduct(outer.c[0]->w - projection,
					outer.c[1]->w - projection)
					.length();
				const float sum = m_result.p[0] + m_result.p[1] + m_result.p[2];
				m_result.p[0] /= sum;
				m_result.p[1] /= sum;
				m_result.p[2] /= sum;
				return (m_status);
			}
		}
		/* Fallback		*/
		m_status = eStatus::FallBack;
		m_normal = guess * -1;
		const float nl = m_normal.length();
		if (nl > 0)
			m_normal = m_normal * (1 / nl);
		else
			m_normal = Vector3f(1, 0, 0);
		m_depth = 0;
		m_result.rank = 1;
		m_result.c[0] = simplex.c[0];
		m_result.p[0] = 1;
		return (m_status);
	}
	bool getedgedist(sFace* face, sSV* a, sSV* b, float& dist)
	{
		Vector3f ba = b->w - a->w;
		Vector3f n_ab = CrossProduct(ba, face->n);   // Outward facing edge normal direction, on triangle plane
		float a_dot_nab = DotProduct(a->w, n_ab);  // Only care about the sign to determine inside/outside, so not normalization required

		if (a_dot_nab < 0)
		{
			// Outside of edge a->b

			const float ba_l2 = ba.length2();
			const float a_dot_ba = DotProduct(a->w, ba);
			const float b_dot_ba = DotProduct(b->w, ba);

			if (a_dot_ba > 0)
			{
				// Pick distance vertex a
				dist = a->w.length();
			}
			else if (b_dot_ba < 0)
			{
				// Pick distance vertex b
				dist = b->w.length();
			}
			else
			{
				// Pick distance to edge a->b
				const float a_dot_b = DotProduct(a->w, b->w);

				float t = (a->w.length2() * b->w.length2() - a_dot_b * a_dot_b) / ba_l2;
				float bigger = t >= 0 ? t : 0;
				dist = sqrtf(bigger);
			}

			return true;
		}

		return false;
	}
	sFace* newface(sSV* a, sSV* b, sSV* c, bool forced)
	{
		if (m_stock.root)
		{
			sFace* face = m_stock.root;
			remove(m_stock, face);
			append(m_hull, face);
			face->pass = 0;
			face->c[0] = a;
			face->c[1] = b;
			face->c[2] = c;
			face->n = CrossProduct(b->w - a->w, c->w - a->w);
			const float l = face->n.length();
			const bool v = l > EPA_ACCURACY;

			if (v)
			{
				if (!(getedgedist(face, a, b, face->d) ||
					getedgedist(face, b, c, face->d) ||
					getedgedist(face, c, a, face->d)))
				{
					// Origin projects to the interior of the triangle
					// Use distance to triangle plane
					face->d = DotProduct(a->w, face->n) / l;
				}

				face->n = face->n * (1 / l);
				if (forced || (face->d >= -EPA_PLANE_EPS))
				{
					return face;
				}
				else
					m_status = eStatus::NonConvex;
			}
			else
				m_status = eStatus::Degenerated;

			remove(m_hull, face);
			append(m_stock, face);
			return 0;
		}
		m_status = m_stock.root ? eStatus::OutOfVertices : eStatus::OutOfFaces;
		return 0;
	}
	sFace* findbest()
	{
		sFace* minf = m_hull.root;
		float mind = minf->d * minf->d;
		for (sFace* f = minf->l[1]; f; f = f->l[1])
		{
			const float sqd = f->d * f->d;
			if (sqd < mind)
			{
				minf = f;
				mind = sqd;
			}
		}
		return (minf);
	}
	bool expand(U pass, sSV* w, sFace* f, U e, sHorizon& horizon)
	{
		static const U i1m3[] = { 1, 2, 0 };
		static const U i2m3[] = { 2, 0, 1 };
		if (f->pass != pass)
		{
			const U e1 = i1m3[e];
			if ((DotProduct(f->n, w->w) - f->d) < -EPA_PLANE_EPS)
			{
				sFace* nf = newface(f->c[e1], f->c[e], w, false);
				if (nf)
				{
					bind(nf, 0, f, e);
					if (horizon.cf)
						bind(horizon.cf, 1, nf, 2);
					else
						horizon.ff = nf;
					horizon.cf = nf;
					++horizon.nf;
					return (true);
				}
			}
			else
			{
				const U e2 = i2m3[e];
				f->pass = (U1)pass;
				if (expand(pass, w, f->f[e1], f->e[e1], horizon) &&
					expand(pass, w, f->f[e2], f->e[e2], horizon))
				{
					remove(m_hull, f);
					append(m_stock, f);
					return (true);
				}
			}
		}
		return (false);
	}
};

////////////////////////////////////////////// EPA //////////////////////

/*
rigidBodiesPairs 所有可能发生碰撞的刚体对
collisions 实际的碰撞结果
*/
void scarlett::NarrowPhaseGJKEPA::CollideDetection(std::vector<RigidBodyPair>& rigidBodiesPairs, std::vector<std::shared_ptr<ContactManifold>>& collisions)
{
	for each (RigidBodyPair pair in rigidBodiesPairs)
	{
		Vector3f position1 = pair.first->GetMaster()->GetMaster()->GetComponent<TransformComponent>()->GetPosition();
		Vector3f position2 = pair.second->GetMaster()->GetMaster()->GetComponent<TransformComponent>()->GetPosition();
		Vector3f guess = position1 - position2;
		sResults result;

		// 如果发生了穿透，则生成对应的ContactPoint信息
		if (Penetration(pair, guess, result))
		{
			ContactPoint point;
			point.localPositionA = result.witnessesInFirstLocal[0];
			point.localPositionB = result.witnessesInFirstLocal[1];
			point.globalPositionA = result.witnessInGlobal[0];
			point.globalPositionB = result.witnessInGlobal[1];
			point.normal = result.normal;
			point.penetrationDistance = result.distance;
			GenerateTangents(point);

			point.rA = point.globalPositionA - position1;
			point.rB = point.globalPositionB - position2;

			std::shared_ptr<ContactManifold> manifold = std::make_shared<ContactManifold>();
			manifold->colliderA = pair.first->GetMaster();
			manifold->colliderB = pair.second->GetMaster();
			manifold->contactPoints[0] = point;
			manifold->contactPointCount = 1;

			collisions.push_back(manifold);
		}
	}



}

void scarlett::NarrowPhaseGJKEPA::InitializeMinkowskiDiff(RigidBodyPair& pair, sResults & result, MinkowskiDiff & diff)
{
	// result
	result.witnessesInFirstLocal[0] = result.witnessesInFirstLocal[1] = result.witnessInGlobal[0] = result.witnessInGlobal[1] = Vector3f(0, 0, 0);
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
	GJK::eStatus::_ gjk_status = gjk.Evaluate(shape, guess*-1);

	switch (gjk_status)
	{

	case GJK::eStatus::Inside:
	{
		EPA epa;
		EPA::eStatus::_ epa_status = epa.Evaluate(gjk, guess*-1);
		if (epa_status != EPA::eStatus::Failed)
		{
			Vector3f w0 = Vector3f(0, 0, 0);
			for (U i = 0; i < epa.m_result.rank; ++i)
			{
				// w0 是 物体1在物体2的最深穿透点在世界坐标下的坐标
				// Support 返回的是全局坐标系的点，所以可以判断w0是全局坐标系下的点。
				w0 = w0 + shape.Support(epa.m_result.c[i]->d, 1) * epa.m_result.p[i];
				
			}
			Matrix4x4f wtrs1 = pair.first->GetMaster()->GetMaster()->GetComponent<TransformComponent>()->GetWorldMatrixInverse();
			result.status = sResults::Penetrating;
			result.witnessesInFirstLocal[0] = TransformPoint(wtrs1, w0);								// 物体1在物体2中的最深穿透点在物体1下的坐标
			Vector3f secondObjectPointInFirstObject = w0 - epa.m_normal * epa.m_depth;
			result.witnessesInFirstLocal[1] = TransformPoint(wtrs1, secondObjectPointInFirstObject);	// 物体2在物体1中的最深穿透点在物体1下的坐标
			result.witnessInGlobal[0] = w0;
			result.witnessInGlobal[1] = secondObjectPointInFirstObject;
			result.normal = epa.m_normal;	// 全局坐标下，由物体1指向物体2
			result.distance = epa.m_depth;	// 距离为正数
			return true;
		}
		else
		{
			result.status = sResults::EPA_Failed;
		}

	}
	case GJK::eStatus::Failed:
	{
		result.status = sResults::GJK_Failed;
	}
	default:
	{
		break;
	}

	}
	return false;
}

// 用法线生成对应切线
// http://allenchou.net/2013/12/game-physics-contact-generation-epa/
void scarlett::NarrowPhaseGJKEPA::GenerateTangents(ContactPoint & contactPoint)
{
	if (contactPoint.normal.x >= 0.57735f)
	{
		contactPoint.tangent1.x = contactPoint.normal.y;
		contactPoint.tangent1.y = -contactPoint.normal.x;
		contactPoint.tangent1.z = 0;
	} 
	else
	{
		contactPoint.tangent1.x = 0; 
		contactPoint.tangent1.y = contactPoint.normal.z;
		contactPoint.tangent1.z = -contactPoint.normal.y;
	}
	Normalize(contactPoint.tangent1);
	contactPoint.tangent2 = CrossProduct(contactPoint.normal, contactPoint.tangent1);

}


