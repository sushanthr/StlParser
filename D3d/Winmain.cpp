#include <Windows.h>
#include <DirectXMath.h>
#include <sys\stat.h>
#include "D3dApp.h"
#include "..\StlParser\Tokenizer.h"

using namespace DirectX;

struct Vertex
{
    float x, y, z;
    D3DCOLOR color;
    static const DWORD fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE;
};

class TestD3DApp : public D3DApp
{
public:
    TestD3DApp(HINSTANCE hInstance);
    ~TestD3DApp();

    bool Init();
    void Update() override;
    void Render() override;

private:
    void LoadTextureBuffer();

    IDirect3DVertexBuffer9* _pVB;
    int _nPrimitives;
    float _maxVal;
};

TestD3DApp::TestD3DApp(HINSTANCE hInstance) :D3DApp(hInstance)
{

}

TestD3DApp::~TestD3DApp()
{

}

size_t getFilesize(const char* filename) {
    struct stat st;
    if (stat(filename, &st) != 0) {
        return 0;
    }
    return st.st_size;
}

void TestD3DApp::LoadTextureBuffer()
{
    char* path = "C:\\Users\\sushraja\\Source\\Workspaces\\StlParser\\StlParser\\D3d\\Assets\\teapot.stl";
    Tokenizer tokenizer = Tokenizer(path);
    size_t fileSize = getFilesize(path);
    Vertex* pVertex = (Vertex*)malloc(fileSize);
    int vertexCount = 0;
    _maxVal = 0;
    while (tokenizer.NextToken().Type() != Tokenizer::NONE && vertexCount < fileSize)
    {
        Tokenizer::TOKEN currentToken = tokenizer.CurrentToken();
        if (currentToken.Type() == Tokenizer::KEYWORD_VERTEX)
        {
            Tokenizer::TOKEN float1 = tokenizer.NextToken();
            assert(float1.Type() == Tokenizer::FLOAT);
            Tokenizer::TOKEN float2 = tokenizer.NextToken();
            assert(float2.Type() == Tokenizer::FLOAT);
            Tokenizer::TOKEN float3 = tokenizer.NextToken();
            assert(float3.Type() == Tokenizer::FLOAT);
            pVertex[vertexCount].x = float1.AsFloat();
            pVertex[vertexCount].y = float2.AsFloat();
            pVertex[vertexCount].z = float3.AsFloat();
            _maxVal = max(_maxVal, pVertex[vertexCount].x);
            _maxVal = max(_maxVal, pVertex[vertexCount].y);
            _maxVal = max(_maxVal, pVertex[vertexCount].z);
            pVertex[vertexCount].color = D3DCOLOR_XRGB((vertexCount % 3) == 0 ? 255 : 0, (vertexCount % 3) == 1 ? 255 : 0, (vertexCount % 3) == 2 ? 255 : 0);
            vertexCount++;
        }
    }
        
    m_pDevice3D->CreateVertexBuffer(vertexCount * sizeof(Vertex), 0, Vertex::fvf, D3DPOOL_MANAGED, &_pVB, NULL);
    void* pVerts;
    _pVB->Lock(0, vertexCount * sizeof(Vertex), (void**)&pVerts, 0);
    memcpy(pVerts, pVertex, vertexCount * sizeof(Vertex));
    _pVB->Unlock();
    free(pVertex);
    _nPrimitives = vertexCount / 3;
}

bool TestD3DApp::Init()
{
    if (!D3DApp::Init())
    {
        return false;
    }    

    LoadTextureBuffer();

    XMMATRIX view;
    XMMATRIX proj;
    const XMFLOAT3 position = { 0.0f, 0.0f, 210 };
    const XMFLOAT3 target = { 0.0f, 0.0f, 1.0f };
    const XMFLOAT3 up = { 0.0f, 1.0f, 0.0f };
    view = XMMatrixLookAtLH(XMLoadFloat3(&position), XMLoadFloat3(&target), XMLoadFloat3(&up));
    proj = XMMatrixPerspectiveFovLH(XMConvertToRadians(90.0f), m_width / m_height, 1.0f, 5000.0f);

    m_pDevice3D->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&proj);
    m_pDevice3D->SetTransform(D3DTS_VIEW, (D3DMATRIX*)&view);

    m_pDevice3D->SetRenderState(D3DRS_LIGHTING, false);
    m_pDevice3D->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
    m_pDevice3D->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    return true;
}

void TestD3DApp::Update()
{

}

void TestD3DApp::Render()
{
    static float pitch = 0;
    m_pDevice3D->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    m_pDevice3D->BeginScene();
    pitch += 0.00005;
    XMMATRIX world = XMMatrixRotationRollPitchYaw(pitch, 0, 0);
    m_pDevice3D->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&world);    
    m_pDevice3D->SetStreamSource(0, _pVB, 0, sizeof(Vertex));
    m_pDevice3D->SetFVF(Vertex::fvf);
    m_pDevice3D->DrawPrimitive(D3DPT_TRIANGLELIST, 0, _nPrimitives);
    m_pDevice3D->EndScene();
    m_pDevice3D->Present(nullptr, nullptr, NULL, nullptr);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreviousInstance, LPSTR lpCmdLine, int ncmdShow)
{
    TestD3DApp testApp = TestD3DApp(hInstance);
    if (!testApp.Init())
    {
        return -1;
    }

    return testApp.Run();
}