#include "Node.h"
#include "Director.h"
#include "Sprite.h"
#include "Runtime/Utils/Logging.h"

using namespace scarlett;


UINode* UINode::Create() {
	auto node = new UINode();
	node->AutoRelease();
	return node;
}

UINode::UINode() :
	m_Name(),
	m_Parent(nullptr),
	m_Position(0, 0),
	m_Localzorder(0),
	m_Transform(),
	m_TransformDirty(true),
	m_ToParentMatrix(),
	m_Inverse(),
	m_bVisible(true),
	m_bTransformUpdated(true)
{
}

UINode::~UINode() {
	// release all children when this node destruct
	for (auto& child : m_Children){
		child->m_Parent = nullptr;
		child->Release();
	}
	m_Children.clear();
	SCARLETT_LOG(info) << "node destruct:" << m_Name;
}

void UINode::AddChild(UINode* child) {
	AddChild(child, 0);
}

void UINode::AddChild(UINode* child, int zorder) {
	auto assertNotSelfChild
	([this, child]() -> bool
	{
		for (UINode* parent(GetParent()); parent != nullptr;
			parent = parent->GetParent())
			if (parent == child)
				return false;
		return true;
	});
	(void)assertNotSelfChild;
	
	if (!assertNotSelfChild()) {
		SCARLETT_LOG(info) << "become graph after add child";
		return;
	}

	if (m_Children.empty()) {
		this->m_Children.reserve(4);
	}

	InsertChild(child, zorder);
}

void UINode::InsertChild(UINode* child, int zorder) {
	m_bTransformUpdated = true;
	m_Children.push_back(child);
	child->SetLocalZorder(zorder);
	child->SetParent(this);
	child->Retain();
}

size_t UINode::GetChildrenCount() {
	return m_Children.size();
}

size_t UINode::GetChildrenCountRecusive()
{
	size_t count = 0;
	for (auto child : m_Children) {
		count += child->GetChildrenCountRecusive();
	}
	return 1 + count;
}

void UINode::SetParent(UINode* parent) {
	m_Parent = parent;
	m_bTransformUpdated = m_TransformDirty = m_InverseDirty = true;
}

UINode* UINode::GetParent() {
	return m_Parent;
}

void UINode::RemoveFromParent() {
	if (m_Parent != nullptr) {
		m_Parent->RemoveChild(this);
		this->Release();
	}
}


void UINode::RemoveChild(UINode* child) {
	if (m_Children.empty()) {
		return;
	}

	size_t idx;
	auto iter = std::find(m_Children.begin(), m_Children.end(), child);
	if (iter == m_Children.end()) {
		idx = -1;
	}
	else {
		idx = iter - m_Children.begin();
	}

	if (idx >= 0) {
		child->SetParent(nullptr);
		child->Release();
		iter = std::next(m_Children.begin(), idx);
		m_Children.erase(iter);
	}
}

Vector2f& UINode::GetPosition() { return m_Position; }

void UINode::SetPosition(float x, float y) {
	m_Position = Vector2f(x, y);
	m_bTransformUpdated = m_TransformDirty = true;
}

int UINode::GetLocalZorder() { return m_Localzorder; }

void UINode::SetLocalZorder(int zorder) {
	m_Localzorder = zorder;
}

bool UINode::GetVisible(){
	return m_bVisible;
}

void UINode::SetVisible(bool v){
	m_bVisible = v;
}


void UINode::Update() {
// update some thing. by now, nothing.
}

void UINode::Render() {
	auto renderer = Director::GetInstance()->GetRender();
	auto mvp = Director::GetInstance()->GetMVPMatrix();
	Render(renderer, mvp, UINodeRenderFlag::TransformDirty);
}

void UINode::Render(Renderer* renderer, const Matrix4f& transform, uint32_t flags) {
}

void UINode::Visit() {
	auto render = Director::GetInstance()->GetRender();
	auto mvp = Director::GetInstance()->GetMVPMatrix();
	Visit(render, mvp, UINodeRenderFlag::TransformDirty);
}

void UINode::Visit(Renderer* renderer, const Matrix4f& parentTransform, uint32_t flags) {
	if (!m_bVisible) {
		return;
	}
	
	auto _flags = ProcessParentFlags(parentTransform, flags);

	if (!m_Children.empty()) {
		for (auto node : m_Children) {
			if (node->GetLocalZorder() < 0) {
				node->Visit(renderer, m_Transform, _flags);
			}
		}

		this->Render(renderer, m_Transform, _flags);

		for (auto node : m_Children) {
			if (node->GetLocalZorder() >= 0) {
				node->Visit(renderer, m_Transform, _flags);
			}
		}
	}
	else {
		this->Render(renderer, m_Transform, _flags);
	}
}

Matrix4f UINode::GetNodeToParentTransform() {
	if (m_TransformDirty) {
		m_ToParentMatrix = Matrix4f::Identity();
		auto offset = Vector3f(m_Position.x() / 512, m_Position.y() / 384, 0);
		auto translation = BuildTranslationMatrix(offset);
		m_ToParentMatrix =  m_ToParentMatrix * translation;
		m_TransformDirty = false;
	}
	return m_ToParentMatrix;
}

uint32_t UINode::ProcessParentFlags(const Matrix4f& parentTransform, uint32_t flags) {
	uint32_t _flags = flags;
	_flags |= (m_bTransformUpdated ? UINodeRenderFlag::TransformDirty : 0);

	if (_flags & UINodeRenderFlag::TransformDirty) {
		auto toParentMatrix = GetNodeToParentTransform();
		m_Transform =  parentTransform * toParentMatrix;
	}

	m_bTransformUpdated = false;
	return _flags;
}