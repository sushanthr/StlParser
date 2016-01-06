#include "D3dApp.h"

D3DApp* D3DApp::g_d3dApp;

D3DApp::D3DApp(HINSTANCE hInstance) 
{
    m_hAppInstance = hInstance;
    m_hAppWindow = NULL;
    m_width = 800;
    m_height = 600;
    m_AppTitle = L"D3d Application - .STL format parser";
    D3DApp::g_d3dApp = this;
}

D3DApp::~D3DApp()
{

}

int D3DApp::Run()
{
    MSG msg = {0};
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            Update();
            Render();
        }
    }

    return msg.wParam;
}

bool D3DApp::Init()
{
    if (!InitWindow())
        return false;
    if (!InitDirect3D())
        return false;
    return true;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) 
{
    return D3DApp::g_d3dApp->MsgProc(hwnd, msg, wparam, lparam);
}

bool D3DApp::InitWindow()
{
    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.hInstance = m_hAppInstance;
    wcex.lpfnWndProc = WndProc;
    wcex.lpszClassName = L"D3DAppClassName";

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL, L"Failed to register class", NULL, NULL);
        return false;
    }

    RECT rect = { 0,0,m_width,m_height };
    DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    AdjustWindowRect(&rect, dwStyle, false);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    m_hAppWindow = CreateWindow(L"D3DAppClassName", m_AppTitle, dwStyle, 0, 0, width, height, NULL, NULL, NULL, NULL);

    if (!m_hAppWindow)
    {
        MessageBox(NULL, L"Failed to create window", NULL, NULL);
        return false;
    }

    return !ShowWindow(m_hAppWindow, SW_SHOW);
}

bool D3DApp::InitDirect3D()
{
    m_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (!m_pDirect3D)
    {
        MessageBox(NULL, L"Failed to create D3D", NULL, NULL);
        return false;
    }

    D3DCAPS9 d3dDevCaps;
    m_pDirect3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dDevCaps);

    int vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    if (d3dDevCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
    {
        vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    }

    ZeroMemory(&m_d3dpp, sizeof(D3DPRESENT_PARAMETERS));

    m_d3dpp.BackBufferWidth = m_width;
    m_d3dpp.BackBufferHeight = m_height;
    m_d3dpp.Windowed = true;
    m_d3dpp.BackBufferCount = 1;
    m_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    m_d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
    m_d3dpp.MultiSampleQuality = 0;
    m_d3dpp.hDeviceWindow = m_hAppWindow;
    m_d3dpp.EnableAutoDepthStencil = true;
    m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
    m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

    m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hAppWindow, vp, &m_d3dpp, &m_pDevice3D);
    if (!m_pDevice3D)
    {
        MessageBox(NULL, L"Failed to create D3DDevice", NULL, NULL);
        return false;
    }

    D3DVIEWPORT9 viewport;
    ZeroMemory(&viewport, sizeof(viewport));
    viewport.X = 0;
    viewport.Y = 0;
    viewport.Width = m_width;
    viewport.Height = m_height;
    viewport.MinZ = -1;
    viewport.MaxZ = 1;

    m_pDevice3D->SetViewport(&viewport);

    return true;
}

LRESULT D3DApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}