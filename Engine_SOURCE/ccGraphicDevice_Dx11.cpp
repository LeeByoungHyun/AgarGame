#include "ccGraphicDevice_Dx11.h"
#include "ccApplication.h"
#include "ccRenderer.h"

extern cc::Application application;

namespace cc::graphics
{
	GraphicDevice_Dx11::GraphicDevice_Dx11()
	{
		// 1. graphic device, context ����

		// 2. ȭ�鿡 ������ �Ҽ� �ְ� �����ִ�
		// swapchain ����

		// 3. rendertarget,view �����ϰ� 
		// 4. ���̹��ۿ� ���̹��� �� �������ְ�

		// 5. ����Ÿ�� Ŭ���� ( ȭ�� ����� )
		// 6. present �Լ��� ����Ÿ�ٿ� �ִ� �ؽ��ĸ�
		//    ����Ϳ� �׷��ش�.

		// mRenderTarget->
		// mRenderTargetView->GetResource();
		
		// Device, Context ����
		HWND hWnd = application.GetHwnd();
		UINT deviceFlag = D3D11_CREATE_DEVICE_DEBUG;
		D3D_FEATURE_LEVEL featureLevel = (D3D_FEATURE_LEVEL)0;

		// ID3D11Device* pDevice = nullptr;
		// ID3D11DeviceContext* pContext = nullptr;
		// ID3D11DeviceContext** ppContext = &pContext;
		D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr
			, deviceFlag, nullptr, 0
			, D3D11_SDK_VERSION
			, mDevice.GetAddressOf(), &featureLevel
			, mContext.GetAddressOf());

		// SwapChain
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferCount = 2;
		swapChainDesc.BufferDesc.Width = application.GetWidth();
		swapChainDesc.BufferDesc.Height = application.GetHeight();

		if (!CreateSwapChain(&swapChainDesc, hWnd))
			return;

		// get rendertarget by swapchain
		if (FAILED(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D)
			, (void**)mRenderTarget.GetAddressOf())))
			return;

		// create rendertarget view
		mDevice->CreateRenderTargetView((ID3D11Resource*)mRenderTarget.Get()
			, nullptr, mRenderTargetView.GetAddressOf());

		D3D11_TEXTURE2D_DESC depthStencilDesc = {};
		depthStencilDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.CPUAccessFlags = 0;

		depthStencilDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.Width = application.GetWidth();
		depthStencilDesc.Height = application.GetHeight();
		depthStencilDesc.ArraySize = 1;

		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;

		depthStencilDesc.MipLevels = 0;
		depthStencilDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA data;
		if (!CreateTexture(&depthStencilDesc, &data))
			return;

		RECT winRect = {};
		GetClientRect(hWnd, &winRect);

		// typedef struct D3D11_VIEWPORT
		// {
		// 	FLOAT TopLeftX;
		// 	FLOAT TopLeftY;
		// 	FLOAT Width;
		// 	FLOAT Height;
		// 	FLOAT MinDepth;
		// 	FLOAT MaxDepth;
		// }		D3D11_VIEWPORT;

		mViewPort =
		{
			0.0f, 0.0f
			, (float)(winRect.right - winRect.left)
			, (float)(winRect.bottom - winRect.top)
			, 0.0f, 1.0f
		};

		BindViewPort(&mViewPort);
		mContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());
	}

	GraphicDevice_Dx11::~GraphicDevice_Dx11()
	{

	}

	bool GraphicDevice_Dx11::CreateSwapChain(const DXGI_SWAP_CHAIN_DESC* desc, HWND hWnd)
	{
		DXGI_SWAP_CHAIN_DESC dxgiDesc = {};

		dxgiDesc.OutputWindow = hWnd;
		dxgiDesc.Windowed = true;
		dxgiDesc.BufferCount = desc->BufferCount;
		dxgiDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;

		dxgiDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		dxgiDesc.BufferDesc.Width = desc->BufferDesc.Width;
		dxgiDesc.BufferDesc.Height = desc->BufferDesc.Height;
		dxgiDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		dxgiDesc.BufferDesc.RefreshRate.Numerator = 240;
		dxgiDesc.BufferDesc.RefreshRate.Denominator = 1;
		dxgiDesc.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;
		dxgiDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		dxgiDesc.SampleDesc.Count = 1;
		dxgiDesc.SampleDesc.Quality = 0;

		Microsoft::WRL::ComPtr<IDXGIDevice> pDXGIDevice = nullptr;
		Microsoft::WRL::ComPtr<IDXGIAdapter> pAdapter = nullptr;
		Microsoft::WRL::ComPtr<IDXGIFactory> pFactory = nullptr;

		if (FAILED(mDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf())))
			return false;

		if (FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf())))
			return false;

		if (FAILED(pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf())))
			return false;

		if (FAILED(pFactory->CreateSwapChain(mDevice.Get(), &dxgiDesc, mSwapChain.GetAddressOf())))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateBuffer(ID3D11Buffer** buffer, D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* data)
	{
		if (FAILED(mDevice->CreateBuffer(desc, data, buffer)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateShader()
	{
		// ���̴� ���� ���
		std::filesystem::path shaderPath
			= std::filesystem::current_path().parent_path();
		shaderPath += L"\\Shader_SOURCE\\";

		//std::filesystem::path vsPath(shaderPath.c_str());
		//vsPath += L"TriangleVS.hlsl";

		// ���� ���̴� ���
		std::filesystem::path triangleVSPath(shaderPath.c_str());
		triangleVSPath += L"TriangleVS.hlsl";

		std::filesystem::path rectangleVSPath(shaderPath.c_str());
		rectangleVSPath += L"RectangleVS.hlsl";

		std::filesystem::path hexagonVSPath(shaderPath.c_str());
		hexagonVSPath += L"HexagonVS.hlsl";

		std::filesystem::path starVSPath(shaderPath.c_str());
		starVSPath += L"StarVS.hlsl";

		// ���� ���̴� ������
		/*
		D3DCompileFromFile(triangleVSPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, "main", "vs_5_0", 0, 0, &cc::renderer::triangleVSBlob, &cc::renderer::errorBlob);	// �ﰢ��

		D3DCompileFromFile(rectangleVSPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, "main", "vs_5_0", 0, 0, &cc::renderer::rectangleVSBlob, &cc::renderer::errorBlob);	// �簢��
		*/
		D3DCompileFromFile(hexagonVSPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, "main", "vs_5_0", 0, 0, &cc::renderer::hexagonVSBlob, &cc::renderer::errorBlob);	// ������

		D3DCompileFromFile(starVSPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, "main", "vs_5_0", 0, 0, &cc::renderer::starVSBlob, &cc::renderer::errorBlob);	// ��

		if (cc::renderer::errorBlob)
		{
			OutputDebugStringA((char*)cc::renderer::errorBlob->GetBufferPointer());
			cc::renderer::errorBlob->Release();
		}

		// ���� ������ ����
		/*
		mDevice->CreateVertexShader(cc::renderer::triangleVSBlob->GetBufferPointer()
			, cc::renderer::triangleVSBlob->GetBufferSize()
			, nullptr, &cc::renderer::triangleVSShader);	// �ﰢ��

		mDevice->CreateVertexShader(cc::renderer::rectangleVSBlob->GetBufferPointer()
			, cc::renderer::rectangleVSBlob->GetBufferSize()
			, nullptr, &cc::renderer::rectangleVSShader);	// �簢��

		mDevice->CreateVertexShader(cc::renderer::hexagonVSBlob->GetBufferPointer()
			, cc::renderer::hexagonVSBlob->GetBufferSize()
			, nullptr, &cc::renderer::hexagonVSShader);	// ������
		*/
		mDevice->CreateVertexShader(cc::renderer::starVSBlob->GetBufferPointer()
			, cc::renderer::starVSBlob->GetBufferSize()
			, nullptr, &cc::renderer::starVSShader);	// ��

		// �ȼ� ���̴� ���
		/*
		std::filesystem::path trialglePSPath(shaderPath.c_str());
		trialglePSPath += L"TrianglePS.hlsl";

		std::filesystem::path rectanglePSPath(shaderPath.c_str());
		rectanglePSPath += L"RectanglePS.hlsl";

		std::filesystem::path hexagonPSPath(shaderPath.c_str());
		hexagonPSPath += L"HexagonPS.hlsl";
		*/
		std::filesystem::path starPSPath(shaderPath.c_str());
		starPSPath += L"StarPS.hlsl";

		// �ȼ� ���̴� ������
		/*
		D3DCompileFromFile(trialglePSPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, "main", "ps_5_0", 0, 0, &cc::renderer::trianglePSBlob, &cc::renderer::errorBlob);	// �ﰢ��

		D3DCompileFromFile(rectanglePSPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, "main", "ps_5_0", 0, 0, &cc::renderer::rectanglePSBlob, &cc::renderer::errorBlob);	// �簢��
		
		D3DCompileFromFile(hexagonPSPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, "main", "ps_5_0", 0, 0, &cc::renderer::hexagonPSBlob, &cc::renderer::errorBlob);	// ������
		*/

		D3DCompileFromFile(starPSPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, "main", "ps_5_0", 0, 0, &cc::renderer::starPSBlob, &cc::renderer::errorBlob);	// ��

		if (cc::renderer::errorBlob)
		{
			OutputDebugStringA((char*)cc::renderer::errorBlob->GetBufferPointer());
			cc::renderer::errorBlob->Release();
		}

		// �ȼ� ���̴� ����
		/*
		mDevice->CreatePixelShader(cc::renderer::trianglePSBlob->GetBufferPointer()
			, cc::renderer::trianglePSBlob->GetBufferSize()
			, nullptr, &cc::renderer::trianglePSShader);
		*/

		/*
		mDevice->CreatePixelShader(cc::renderer::rectanglePSBlob->GetBufferPointer()
			, cc::renderer::rectanglePSBlob->GetBufferSize()
			, nullptr, &cc::renderer::rectanglePSShader);
		*/

		/*
		mDevice->CreatePixelShader(cc::renderer::hexagonPSBlob->GetBufferPointer()
			, cc::renderer::hexagonPSBlob->GetBufferSize()
			, nullptr, &cc::renderer::hexagonPSShader);
		*/

		mDevice->CreatePixelShader(cc::renderer::starPSBlob->GetBufferPointer()
			, cc::renderer::starPSBlob->GetBufferSize()
			, nullptr, &cc::renderer::starPSShader);

		// Input layout ���� ���� ������ �Ѱ�����Ѵ�.
		D3D11_INPUT_ELEMENT_DESC arrLayout[2] = {};

		arrLayout[0].AlignedByteOffset = 0;
		arrLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		arrLayout[0].InputSlot = 0;
		arrLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[0].SemanticName = "POSITION";
		arrLayout[0].SemanticIndex = 0;

		arrLayout[1].AlignedByteOffset = 12;
		arrLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		arrLayout[1].InputSlot = 0;
		arrLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[1].SemanticName = "COLOR";
		arrLayout[1].SemanticIndex = 0;

		// Input layout ����
		// �ﰢ��
		/*
		mDevice->CreateInputLayout(arrLayout, 2
		 	, renderer::triangleVSBlob->GetBufferPointer()
		 	, renderer::triangleVSBlob->GetBufferSize()
		 	, &renderer::triangleLayout);
		*/

		// �簢��
		/*
		mDevice->CreateInputLayout(arrLayout, 2
			, renderer::rectangleVSBlob->GetBufferPointer()
			, renderer::rectangleVSBlob->GetBufferSize()
			, &renderer::rectangleLayout);
		*/

		// hexagon
		/*
		mDevice->CreateInputLayout(arrLayout, 2
			, renderer::hexagonVSBlob->GetBufferPointer()
			, renderer::hexagonVSBlob->GetBufferSize()
			, &renderer::hexagonLayout);
		*/

		mDevice->CreateInputLayout(arrLayout, 2
			, renderer::starVSBlob->GetBufferPointer()
			, renderer::starVSBlob->GetBufferSize()
			, &renderer::starLayout);

		return true;
	}

	bool GraphicDevice_Dx11::CreateTexture(const D3D11_TEXTURE2D_DESC* desc, void* data)
	{
		D3D11_TEXTURE2D_DESC dxgiDesc = {};
		dxgiDesc.BindFlags = desc->BindFlags;
		dxgiDesc.Usage = desc->Usage;
		dxgiDesc.CPUAccessFlags = 0;

		dxgiDesc.Format = desc->Format;
		dxgiDesc.Width = desc->Width;
		dxgiDesc.Height = desc->Height;
		dxgiDesc.ArraySize = desc->ArraySize;

		dxgiDesc.SampleDesc.Count = desc->SampleDesc.Count;
		dxgiDesc.SampleDesc.Quality = 0;

		dxgiDesc.MipLevels = desc->MipLevels;
		dxgiDesc.MiscFlags = desc->MiscFlags;

		if (FAILED(mDevice->CreateTexture2D(&dxgiDesc, nullptr, mDepthStencilBuffer.ReleaseAndGetAddressOf())))
			return false;

		if (FAILED(mDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), nullptr, mDepthStencilView.GetAddressOf())))
			return false;

		return true;
	}

	void GraphicDevice_Dx11::BindViewPort(D3D11_VIEWPORT* viewPort)
	{
		mContext->RSSetViewports(1, viewPort);
	}

	void GraphicDevice_Dx11::Draw()
	{
		// render target clear
		FLOAT bgColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
		mContext->ClearRenderTargetView(mRenderTargetView.Get(), bgColor);
		mContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);

		// viewport update
		HWND hWnd = application.GetHwnd();
		RECT winRect = {};
		GetClientRect(hWnd, &winRect);
		mViewPort =
		{
			0.0f, 0.0f
			, (float)(winRect.right - winRect.left)
			, (float)(winRect.bottom - winRect.top)
			, 0.0f, 1.0f
		};

		BindViewPort(&mViewPort);

		// ���� Ÿ�� ����
		mContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());

		// input assembler ���������� ���� ����
		UINT vertexsize = sizeof(renderer::Vertex);
		UINT offset = 0;

		/*
		// triangle
		mContext->IASetVertexBuffers(0, 1, &renderer::triangleBuffer, &vertexsize, &offset);	// ���� ���� ����
		mContext->IASetInputLayout(renderer::triangleLayout);	// input layout ����
		mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// ������Ƽ�� ���� ����

		// rectangle
		mContext->IASetVertexBuffers(0, 1, &renderer::rectangleBuffer, &vertexsize, &offset);	// ���� ���� ����
		mContext->IASetInputLayout(renderer::rectangleLayout);	// input layout ����
		mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// ������Ƽ�� ���� ����

		// hexagon
		mContext->IASetVertexBuffers(0, 1, &renderer::hexagonBuffer, &vertexsize, &offset);	// ���� ���� ����
		mContext->IASetInputLayout(renderer::hexagonLayout);	// input layout ����
		mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// ������Ƽ�� ���� ����
		*/

		mContext->IASetVertexBuffers(0, 1, &renderer::starBuffer, &vertexsize, &offset);	// ���� ���� ����
		mContext->IASetInputLayout(renderer::starLayout);	// input layout ����
		mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// ������Ƽ�� ���� ����

		// Bind VS, PS 
		/*
		mContext->VSSetShader(renderer::triangleVSShader, 0, 0);
		mContext->PSSetShader(renderer::trianglePSShader, 0, 0);
		mContext->VSSetShader(renderer::rectangleVSShader, 0, 0);
		mContext->PSSetShader(renderer::rectanglePSShader, 0, 0);
		mContext->VSSetShader(renderer::hexagonVSShader, 0, 0);
		mContext->PSSetShader(renderer::hexagonPSShader, 0, 0);
		*/
		mContext->VSSetShader(renderer::starVSShader, 0, 0);
		mContext->PSSetShader(renderer::starPSShader, 0, 0);
		// Draw Render Target
		mContext->Draw(15, 0);

		// ����Ÿ�ٿ� �ִ� �̹����� ȭ�鿡 �׷��ش�
		mSwapChain->Present(0, 0);
	}
}
