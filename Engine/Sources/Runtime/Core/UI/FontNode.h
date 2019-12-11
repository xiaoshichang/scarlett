#pragma once

#include <string>
#include "Runtime/RHI/Texture.h"
#include "Node.h"

namespace scarlett {

	class FontNode : public UINode {
	public:
		static FontNode*	Create(const std::string& text="", const std::string& style="arial.ttf");

		virtual void		Visit(Renderer* render, const Matrix4x4f& parentTransform, uint32_t flags);
		virtual void		Render(Renderer* render, const Matrix4x4f& parentTransform, uint32_t flags);

		void				SetText(const std::string& text);
		std::string			GetText();

		void				SetColor(const Vector3f& color);
		Vector3f			GetColor();

		void				SetStyle(const std::string& style);
		std::string			GetStyle();

		void				UpdateContent();
		void				ResizeTexture(int width, int height);
		void				CopyFontBitmap();
		void				CalculateTextureHeight();

	private:
		FontNode();

		std::string			mText;
		std::string			mStyle;

		Vector3f			mColor;

		// render
		int					mTextureWidth;
		int					mTextureHeight;
		int					mTextureOriginY;
		std::shared_ptr<ITexture>	mTexture;

		bool				mContentDirty;
		
	};

}
