#pragma once
#include "CCEngine.h"
#include "ccGraphicDevice_Dx11.h"

namespace cc
{
	class Application
	{
	public:
		Application();
		~Application();
		
		void Run();

		void Initialize();
		void Update();
		void LateUpdate();
		void Render();

		void SetWindow(HWND hwnd, UINT width, UINT height);

		UINT GetWidth() { return mWidth; }
		UINT GetHeight() { return mHeight; }
		HWND GetHwnd() { return mHwnd; }

	private:
		bool mbInitialize = false;
		// ������ �Ѱ��� ��ü�� ����� �ִ� ����Ʈ ������
		std::unique_ptr<cc::graphics::GraphicDevice_Dx11> graphicDevice;

		// HDC mHdc; -> GPU API
		HWND mHwnd;
		UINT mWidth;
		UINT mHeight;
	};
}