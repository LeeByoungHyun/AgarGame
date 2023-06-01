#include "ccGraphicDevice_Dx11.h"
#include "ccApplication.h"
#include "ccRenderer.h"

extern cc::Application application;

namespace cc::graphics
{
	GraphicDevice_Dx11::GraphicDevice_Dx11()
	{
		// 1. graphic device, context 생성

		// 2. 화면에 렌더링 할수 있게 도와주는
		// swapchain 생성

		// 3. rendertarget,view 생성하고 
		// 4. 깊이버퍼와 깊이버퍼 뷰 생성해주고

		// 5. 렌더타겟 클리어 ( 화면 지우기 )
		// 6. present 함수로 렌더타겟에 있는 텍스쳐를
		//    모니터에 그려준다.

		// mRenderTarget->
		// mRenderTargetView->GetResource();
		
		// Device, Context 생성
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
		// 셰이더 폴더 경로
		std::filesystem::path shaderPath
			= std::filesystem::current_path().parent_path();
		shaderPath += L"\\Shader_SOURCE\\";

		//std::filesystem::path vsPath(shaderPath.c_str());
		//vsPath += L"TriangleVS.hlsl";

		// 정점 셰이더 경로
		std::filesystem::path triangleVSPath(shaderPath.c_str());
		triangleVSPath += L"TriangleVS.hlsl";

		std::filesystem::path rectangleVSPath(shaderPath.c_str());
		rectangleVSPath += L"RectangleVS.hlsl";

		std::filesystem::path hexagonVSPath(shaderPath.c_str());
		hexagonVSPath += L"HexagonVS.hlsl";

		std::filesystem::path starVSPath(shaderPath.c_str());
		starVSPath += L"StarVS.hlsl";

		std::filesystem::path circleVSPath(shaderPath.c_str());
		circleVSPath += L"CircleVS.hlsl";

		// 정점 셰이더 컴파일
		D3DCompileFromFile(triangleVSPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, "main", "vs_5_0", 0, 0, &cc::renderer::triangleVSBlob, &cc::renderer::errorBlob);	// 삼각형

		D3DCompileFromFile(rectangleVSPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, "main", "vs_5_0", 0, 0, &cc::renderer::rectangleVSBlob, &cc::renderer::errorBlob);	// 사각형

		D3DCompileFromFile(hexagonVSPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, "main", "vs_5_0", 0, 0, &cc::renderer::hexagonVSBlob, &cc::renderer::errorBlob);	// 육각형

		D3DCompileFromFile(starVSPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, "main", "vs_5_0", 0, 0, &cc::renderer::starVSBlob, &cc::renderer::errorBlob);	// 별

		D3DCompileFromFile(circleVSPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, "main", "vs_5_0", 0, 0, &cc::renderer::circleVSBlob, &cc::renderer::errorBlob);	// 원

		if (cc::renderer::errorBlob)
		{
			OutputDebugStringA((char*)cc::renderer::errorBlob->GetBufferPointer());
			cc::renderer::errorBlob->Release();
		}

		// 정점 데이터 생성
		mDevice->CreateVertexShader(cc::renderer::triangleVSBlob->GetBufferPointer()
			, cc::renderer::triangleVSBlob->GetBufferSize()
			, nullptr, &cc::renderer::triangleVSShader);	// 삼각형

		mDevice->CreateVertexShader(cc::renderer::rectangleVSBlob->GetBufferPointer()
			, cc::renderer::rectangleVSBlob->GetBufferSize()
			, nullptr, &cc::renderer::rectangleVSShader);	// 사각형

		mDevice->CreateVertexShader(cc::renderer::hexagonVSBlob->GetBufferPointer()
			, cc::renderer::hexagonVSBlob->GetBufferSize()
			, nullptr, &cc::renderer::hexagonVSShader);	// 육각형

		mDevice->CreateVertexShader(cc::renderer::starVSBlob->GetBufferPointer()
			, cc::renderer::starVSBlob->GetBufferSize()
			, nullptr, &cc::renderer::starVSShader);	// 별

		mDevice->CreateVertexShader(cc::renderer::circleVSBlob->GetBufferPointer()
			, cc::renderer::circleVSBlob->GetBufferSize()
			, nullptr, &cc::renderer::circleVSShader);	// 원

		// 픽셀 셰이더 경로
		std::filesystem::path trialglePSPath(shaderPath.c_str());
		trialglePSPath += L"TrianglePS.hlsl";

		std::filesystem::path rectanglePSPath(shaderPath.c_str());
		rectanglePSPath += L"RectanglePS.hlsl";

		std::filesystem::path hexagonPSPath(shaderPath.c_str());
		hexagonPSPath += L"HexagonPS.hlsl";

		std::filesystem::path starPSPath(shaderPath.c_str());
		starPSPath += L"StarPS.hlsl";

		std::filesystem::path circlePSPath(shaderPath.c_str());
		circlePSPath += L"CirclePS.hlsl";

		// 픽셀 셰이더 컴파일
		D3DCompileFromFile(trialglePSPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, "main", "ps_5_0", 0, 0, &cc::renderer::trianglePSBlob, &cc::renderer::errorBlob);	// 삼각형

		D3DCompileFromFile(rectanglePSPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, "main", "ps_5_0", 0, 0, &cc::renderer::rectanglePSBlob, &cc::renderer::errorBlob);	// 사각형
		
		D3DCompileFromFile(hexagonPSPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, "main", "ps_5_0", 0, 0, &cc::renderer::hexagonPSBlob, &cc::renderer::errorBlob);	// 육각형

		D3DCompileFromFile(starPSPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, "main", "ps_5_0", 0, 0, &cc::renderer::starPSBlob, &cc::renderer::errorBlob);	// 별

		D3DCompileFromFile(circlePSPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, "main", "ps_5_0", 0, 0, &cc::renderer::circlePSBlob, &cc::renderer::errorBlob);	// 원

		if (cc::renderer::errorBlob)
		{
			OutputDebugStringA((char*)cc::renderer::errorBlob->GetBufferPointer());
			cc::renderer::errorBlob->Release();
		}

		// 픽셀 셰이더 생성
		mDevice->CreatePixelShader(cc::renderer::trianglePSBlob->GetBufferPointer()
			, cc::renderer::trianglePSBlob->GetBufferSize()
			, nullptr, &cc::renderer::trianglePSShader);

		mDevice->CreatePixelShader(cc::renderer::rectanglePSBlob->GetBufferPointer()
			, cc::renderer::rectanglePSBlob->GetBufferSize()
			, nullptr, &cc::renderer::rectanglePSShader);

		mDevice->CreatePixelShader(cc::renderer::hexagonPSBlob->GetBufferPointer()
			, cc::renderer::hexagonPSBlob->GetBufferSize()
			, nullptr, &cc::renderer::hexagonPSShader);

		mDevice->CreatePixelShader(cc::renderer::starPSBlob->GetBufferPointer()
			, cc::renderer::starPSBlob->GetBufferSize()
			, nullptr, &cc::renderer::starPSShader);

		mDevice->CreatePixelShader(cc::renderer::circlePSBlob->GetBufferPointer()
			, cc::renderer::circlePSBlob->GetBufferSize()
			, nullptr, &cc::renderer::circlePSShader);

		// Input layout 정점 구조 정보를 넘겨줘야한다.
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

		// Input layout 설정
		// 삼각형
		mDevice->CreateInputLayout(arrLayout, 2
		 	, renderer::triangleVSBlob->GetBufferPointer()
		 	, renderer::triangleVSBlob->GetBufferSize()
		 	, &renderer::triangleLayout);

		// 사각형
		mDevice->CreateInputLayout(arrLayout, 2
			, renderer::rectangleVSBlob->GetBufferPointer()
			, renderer::rectangleVSBlob->GetBufferSize()
			, &renderer::rectangleLayout);

		// hexagon
		mDevice->CreateInputLayout(arrLayout, 2
			, renderer::hexagonVSBlob->GetBufferPointer()
			, renderer::hexagonVSBlob->GetBufferSize()
			, &renderer::hexagonLayout);

		// star
		mDevice->CreateInputLayout(arrLayout, 2
			, renderer::starVSBlob->GetBufferPointer()
			, renderer::starVSBlob->GetBufferSize()
			, &renderer::starLayout);

		// star
		mDevice->CreateInputLayout(arrLayout, 2
			, renderer::circleVSBlob->GetBufferPointer()
			, renderer::circleVSBlob->GetBufferSize()
			, &renderer::circleLayout);

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

	void GraphicDevice_Dx11::SetConstantBuffer(ID3D11Buffer* buffer, void* data, UINT size)
	{
		D3D11_MAPPED_SUBRESOURCE subRes = {};
		mContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes);
		memcpy(subRes.pData, data, size);
		mContext->Unmap(buffer, 0);
	}

	void GraphicDevice_Dx11::BindConstantBuffer(eShaderStage stage, eCBType type, ID3D11Buffer* buffer)
	{
		switch (stage)
		{
		case eShaderStage::VS:
			mContext->VSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case eShaderStage::HS:
			mContext->HSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case eShaderStage::DS:
			mContext->DSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case eShaderStage::GS:
			mContext->GSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case eShaderStage::PS:
			mContext->PSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case eShaderStage::CS:
			mContext->CSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case eShaderStage::End:
			break;
		default:
			break;
		}
	}

	void GraphicDevice_Dx11::BindsConstantBuffer(eShaderStage stage, eCBType type, ID3D11Buffer* buffer)
	{
		mContext->VSSetConstantBuffers((UINT)type, 1, &buffer);
		mContext->HSSetConstantBuffers((UINT)type, 1, &buffer);
		mContext->DSSetConstantBuffers((UINT)type, 1, &buffer);
		mContext->GSSetConstantBuffers((UINT)type, 1, &buffer);
		mContext->PSSetConstantBuffers((UINT)type, 1, &buffer);
		mContext->CSSetConstantBuffers((UINT)type, 1, &buffer);
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

		// 렌더 타겟 설정
		mContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());

		// input assembler 정점데이터 정보 지정
		UINT vertexsize = sizeof(renderer::Vertex);
		UINT offset = 0;

		// triangle
		mContext->IASetVertexBuffers(0, 1, &renderer::triangleBuffer, &vertexsize, &offset);	// 정점 버퍼 설정
		mContext->IASetIndexBuffer(renderer::triangleIdxBuffer, DXGI_FORMAT_R32_UINT, 0);
		mContext->IASetInputLayout(renderer::triangleLayout);	// input layout 설정
		mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 프리미티브 유형 설정
		mContext->VSSetShader(renderer::triangleVSShader, 0, 0);	// Bind VS, PS 
		mContext->PSSetShader(renderer::trianglePSShader, 0, 0);
		//mContext->Draw(3, 0);		// Draw Render Target
		mContext->DrawIndexed(3, 0, 0);

		// rectangle
		mContext->IASetVertexBuffers(0, 1, &renderer::rectangleBuffer, &vertexsize, &offset);	
		mContext->IASetInputLayout(renderer::rectangleLayout);	
		mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		mContext->VSSetShader(renderer::rectangleVSShader, 0, 0);
		mContext->PSSetShader(renderer::rectanglePSShader, 0, 0);
		mContext->Draw(6, 0);

		// hexagon
		mContext->IASetVertexBuffers(0, 1, &renderer::hexagonBuffer, &vertexsize, &offset);	
		mContext->IASetInputLayout(renderer::hexagonLayout);	
		mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	
		mContext->VSSetShader(renderer::hexagonVSShader, 0, 0);
		mContext->PSSetShader(renderer::hexagonPSShader, 0, 0);
		mContext->Draw(12, 0);

		// star
		mContext->IASetVertexBuffers(0, 1, &renderer::starBuffer, &vertexsize, &offset);	
		mContext->IASetInputLayout(renderer::starLayout);
		mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	
		mContext->VSSetShader(renderer::starVSShader, 0, 0);
		mContext->PSSetShader(renderer::starPSShader, 0, 0);
		mContext->Draw(15, 0);

		// star
		mContext->IASetVertexBuffers(0, 1, &renderer::circleBuffer, &vertexsize, &offset);
		mContext->IASetInputLayout(renderer::circleLayout);
		mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
		mContext->VSSetShader(renderer::circleVSShader, 0, 0);
		mContext->PSSetShader(renderer::circlePSShader, 0, 0);
		mContext->Draw(360, 0);
		
		// 렌더타겟에 있는 이미지를 화면에 그려준다
		mSwapChain->Present(0, 0);
	}
}
