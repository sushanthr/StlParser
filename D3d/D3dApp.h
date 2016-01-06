#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <string>

class D3DApp
{
public:
    D3DApp(HINSTANCE hInstance);
    virtual ~D3DApp();

    int Run();

    virtual bool Init();
    virtual void Update() = 0;
    virtual void Render() = 0;

    LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
    HWND    m_hAppWindow;
    HINSTANCE m_hAppInstance;
    unsigned int m_width;
    unsigned int m_height;
    wchar_t* m_AppTitle;

    // DX Members
    IDirect3D9* m_pDirect3D;
    IDirect3DDevice9* m_pDevice3D;
    D3DPRESENT_PARAMETERS m_d3dpp;

    bool InitWindow();
    bool InitDirect3D();

public:
    static D3DApp* g_d3dApp;
};
