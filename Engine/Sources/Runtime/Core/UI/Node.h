#pragma once
#include <vector>
#include <string>
#include "Runtime/Core/Math/ScltMath.h"
#include "Ref.h"

namespace scarlett {
	class Renderer;
	enum UINodeRenderFlag{
		TransformDirty = (1 << 0)
	};

	class UINode : public Ref {
	public:
		static UINode* Create();

		void SetName(std::string name) { m_Name = name; }
		std::string GetName() { return m_Name; }

		virtual void AddChild(UINode* child);
		virtual void AddChild(UINode* child, int zorder);
		virtual void InsertChild(UINode* child, int zorder);

		virtual size_t GetChildrenCount();
		virtual std::vector<UINode*>& GetChildren() { return m_Children; }
		virtual size_t GetChildrenCountRecusive();

		virtual void SetParent(UINode* parent);
		virtual UINode* GetParent();

		virtual void RemoveFromParent();
		virtual void RemoveChild(UINode* child);

		virtual Vector2f GetPosition();
		virtual void SetPosition(float x, float y);

		virtual Vector2f GetContentSize();
		virtual void SetContentSize(float x, float y);

		virtual int GetLocalZorder();
		virtual void SetLocalZorder(int zorder);

		virtual bool GetVisible();
		virtual void SetVisible(bool v);

		virtual uint32_t ProcessParentFlags(const Matrix4x4f& parentTransform, uint32_t flags);
		virtual Matrix4x4f GetNodeToParentTransform();

		virtual void Update();
		virtual void Render();
		virtual void Render(Renderer* render, const Matrix4x4f& parentTransform, uint32_t flags);
		virtual void Visit();
		virtual void Visit(Renderer* render, const Matrix4x4f& parentTransform, uint32_t flags);

		virtual ~UINode();

	protected:
		UINode();

	protected:
		std::string m_Name;

		std::vector<UINode*> m_Children;
		UINode* m_Parent;

		Vector2f m_Position;
		Vector2f mContentSize;
		int m_Localzorder;

		Matrix4x4f m_Transform;
		Matrix4x4f m_ToParentMatrix;
		bool m_TransformDirty;
		Matrix4x4f m_Inverse;
		bool m_InverseDirty;
		bool m_bVisible;
		bool m_bTransformUpdated;

	};

}
