#include <vector>
#include "draw.h"
#include "drawboxes.h"

void Draw::Reset(DWORD Width, DWORD Height)
{
    box::box_draw_command_buffer.screen_width = Width;
    box::box_draw_command_buffer.screen_height = Height;
    Screen.Width = (float)Width;
    Screen.Height = (float)Height;
    Screen.x_center = (float)Screen.Width / 2.0;
    Screen.y_center = (float)Screen.Height / 2.0;
}

void Draw::Line(IDirect3DDevice9* device, float x1, float y1, float x2, float y2, float width, bool antialias, DWORD color)
{
    ID3DXLine* m_Line;

    D3DXCreateLine(device, &m_Line);
    D3DXVECTOR2 line[] = { D3DXVECTOR2(x1, y1), D3DXVECTOR2(x2, y2) };
    m_Line->SetWidth(width);
    if (antialias) m_Line->SetAntialias(1);
    m_Line->Begin();
    m_Line->Draw(line, 2, color);
    m_Line->End();
    m_Line->Release();
}

void Draw::Circle(IDirect3DDevice9* device, float x, float y, float radius, int rotate, int type, bool smoothing, int resolution, DWORD color)
{
    std::vector<vertex> circle(resolution + 2);
    float angle = rotate * D3DX_PI / 180;
    float pi;

    if (type == full) pi = D3DX_PI;        // Full circle
    if (type == half) pi = D3DX_PI / 2;      // 1/2 circle
    if (type == quarter) pi = D3DX_PI / 4;   // 1/4 circle

    for (int i = 0; i < resolution + 2; i++)
    {
        circle[i].x = (float)(x - radius * cos(i * (2 * pi / resolution)));
        circle[i].y = (float)(y - radius * sin(i * (2 * pi / resolution)));
        circle[i].z = 0;
        circle[i].rhw = 1;
        circle[i].color = color;
    }

    // Rotate matrix
    int _res = resolution + 2;
    for (int i = 0; i < _res; i++)
    {
        circle[i].x = x + cos(angle) * (circle[i].x - x) - sin(angle) * (circle[i].y - y);
        circle[i].y = y + sin(angle) * (circle[i].x - x) + cos(angle) * (circle[i].y - y);
    }

    device->CreateVertexBuffer((resolution + 2) * sizeof(vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB, NULL);

    VOID* pVertices;
    g_pVB->Lock(0, (resolution + 2) * sizeof(vertex), (void**)&pVertices, 0);
    memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(vertex));
    g_pVB->Unlock();


    device->SetTexture(0, NULL);
    device->SetPixelShader(NULL);
    if (smoothing)
    {
        device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
        device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);
    }
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    device->SetStreamSource(0, g_pVB, 0, sizeof(vertex));
    device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

    device->DrawPrimitive(D3DPT_LINESTRIP, 0, resolution);
    if (g_pVB != NULL) g_pVB->Release();
}

void Draw::CircleFilled(IDirect3DDevice9* device, float x, float y, float rad, float rotate, int type, int resolution, DWORD color)
{
    std::vector<vertex> circle(resolution + 2);
    float angle = rotate * D3DX_PI / 180;
    float pi;

    if (type == full) pi = D3DX_PI;        // Full circle
    if (type == half) pi = D3DX_PI / 2;      // 1/2 circle
    if (type == quarter) pi = D3DX_PI / 4;   // 1/4 circle

    circle[0].x = x;
    circle[0].y = y;
    circle[0].z = 0;
    circle[0].rhw = 1;
    circle[0].color = color;

    for (int i = 1; i < resolution + 2; i++)
    {
        circle[i].x = (float)(x - rad * cos(pi * ((i - 1) / (resolution / 2.0f))));
        circle[i].y = (float)(y - rad * sin(pi * ((i - 1) / (resolution / 2.0f))));
        circle[i].z = 0;
        circle[i].rhw = 1;
        circle[i].color = color;
    }

    // Rotate matrix
    int _res = resolution + 2;
    for (int i = 0; i < _res; i++)
    {
        circle[i].x = x + cos(angle) * (circle[i].x - x) - sin(angle) * (circle[i].y - y);
        circle[i].y = y + sin(angle) * (circle[i].x - x) + cos(angle) * (circle[i].y - y);
    }

    device->CreateVertexBuffer((resolution + 2) * sizeof(vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB, NULL);

    VOID* pVertices;
    g_pVB->Lock(0, (resolution + 2) * sizeof(vertex), (void**)&pVertices, 0);
    memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(vertex));
    g_pVB->Unlock();

    device->SetTexture(0, NULL);
    device->SetPixelShader(NULL);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    device->SetStreamSource(0, g_pVB, 0, sizeof(vertex));
    device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, resolution);
    if (g_pVB != NULL) g_pVB->Release();
}

void Draw::Box(IDirect3DDevice9* device, float x, float y, float w, float h, float linewidth, DWORD color)
{
    if (linewidth == 0 || linewidth == 1)
    {
        BoxFilled(device, x, y, w, 1, color);             // Top
        BoxFilled(device, x, y + h - 1, w, 1, color);         // Bottom
        BoxFilled(device, x, y + 1, 1, h - 2 * 1, color);       // Left
        BoxFilled(device, x + w - 1, y + 1, 1, h - 2 * 1, color);   // Right
    }
    else
    {
        BoxFilled(device, x, y, w, linewidth, color);                                     // Top
        BoxFilled(device, x, y + h - linewidth, w, linewidth, color);                         // Bottom
        BoxFilled(device, x, y + linewidth, linewidth, h - 2 * linewidth, color);               // Left
        BoxFilled(device, x + w - linewidth, y + linewidth, linewidth, h - 2 * linewidth, color);   // Right
    }
}

void Draw::BoxBordered(IDirect3DDevice9* device, float x, float y, float w, float h, float border_width, DWORD color, DWORD color_border)
{
    BoxFilled(device, x, y, w, h, color);
    Box(device, x - border_width, y - border_width, w + 2 * border_width, h + border_width, border_width, color_border);
}

void Draw::BoxFilled(IDirect3DDevice9* device, float x, float y, float w, float h, DWORD color)
{
    vertex V[4];

    V[0].color = V[1].color = V[2].color = V[3].color = color;

    V[0].z = V[1].z = V[2].z = V[3].z = 0;
    V[0].rhw = V[1].rhw = V[2].rhw = V[3].rhw = 0;

    V[0].x = x;
    V[0].y = y;
    V[1].x = x + w;
    V[1].y = y;
    V[2].x = x + w;
    V[2].y = y + h;
    V[3].x = x;
    V[3].y = y + h;

    unsigned short indexes[] = { 0, 1, 3, 1, 2, 3 };

    device->CreateVertexBuffer(4 * sizeof(vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB, NULL);
    device->CreateIndexBuffer(2 * sizeof(short), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIB, NULL);

    VOID* pVertices;
    g_pVB->Lock(0, sizeof(V), (void**)&pVertices, 0);
    memcpy(pVertices, V, sizeof(V));
    g_pVB->Unlock();

    VOID* pIndex;
    g_pIB->Lock(0, sizeof(indexes), (void**)&pIndex, 0);
    memcpy(pIndex, indexes, sizeof(indexes));
    g_pIB->Unlock();

    device->SetTexture(0, NULL);
    device->SetPixelShader(NULL);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    device->SetStreamSource(0, g_pVB, 0, sizeof(vertex));
    device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    device->SetIndices(g_pIB);

    device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

    g_pVB->Release();
    g_pIB->Release();
}

void Draw::BoxRounded(IDirect3DDevice9* device, float x, float y, float w, float h, float radius, bool smoothing, DWORD color, DWORD bcolor)
{
    BoxFilled(device, x + radius, y + radius, w - 2 * radius - 1, h - 2 * radius - 1, color);   // Center rect.
    BoxFilled(device, x + radius, y + 1, w - 2 * radius - 1, radius - 1, color);            // Top rect.
    BoxFilled(device, x + radius, y + h - radius - 1, w - 2 * radius - 1, radius, color);     // Bottom rect.
    BoxFilled(device, x + 1, y + radius, radius - 1, h - 2 * radius - 1, color);            // Left rect.
    BoxFilled(device, x + w - radius - 1, y + radius, radius, h - 2 * radius - 1, color);     // Right rect.

    // Smoothing method
    if (smoothing)
    {
        CircleFilled(device, x + radius, y + radius, radius - 1, 0, quarter, 16, color);             // Top-left corner
        CircleFilled(device, x + w - radius - 1, y + radius, radius - 1, 90, quarter, 16, color);        // Top-right corner
        CircleFilled(device, x + w - radius - 1, y + h - radius - 1, radius - 1, 180, quarter, 16, color);   // Bottom-right corner
        CircleFilled(device, x + radius, y + h - radius - 1, radius - 1, 270, quarter, 16, color);       // Bottom-left corner

        Circle(device, x + radius + 1, y + radius + 1, radius, 0, quarter, true, 16, bcolor);          // Top-left corner
        Circle(device, x + w - radius - 2, y + radius + 1, radius, 90, quarter, true, 16, bcolor);       // Top-right corner
        Circle(device, x + w - radius - 2, y + h - radius - 2, radius, 180, quarter, true, 16, bcolor);    // Bottom-right corner
        Circle(device, x + radius + 1, y + h - radius - 2, radius, 270, quarter, true, 16, bcolor);      // Bottom-left corner

        Line(device, x + radius, y + 1, x + w - radius - 1, y + 1, 1, false, bcolor);       // Top line
        Line(device, x + radius, y + h - 2, x + w - radius - 1, y + h - 2, 1, false, bcolor);   // Bottom line
        Line(device, x + 1, y + radius, x + 1, y + h - radius - 1, 1, false, bcolor);       // Left line
        Line(device, x + w - 2, y + radius, x + w - 2, y + h - radius - 1, 1, false, bcolor);   // Right line
    }
    else
    {
        CircleFilled(device, x + radius, y + radius, radius, 0, quarter, 16, color);             // Top-left corner
        CircleFilled(device, x + w - radius - 1, y + radius, radius, 90, quarter, 16, color);        // Top-right corner
        CircleFilled(device, x + w - radius - 1, y + h - radius - 1, radius, 180, quarter, 16, color);   // Bottom-right corner
        CircleFilled(device, x + radius, y + h - radius - 1, radius, 270, quarter, 16, color);       // Bottom-left corner
    }
}

void Draw::Text(const char* text, float x, float y, int orientation, int font, bool bordered, DWORD color, DWORD bcolor)
{
    RECT rect;

    switch (orientation)
    {
    case lefted:
        if (bordered)
        {
            SetRect(&rect, x - 1, y, x - 1, y);
            pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, bcolor);
            SetRect(&rect, x + 1, y, x + 1, y);
            pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, bcolor);
            SetRect(&rect, x, y - 1, x, y - 1);
            pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, bcolor);
            SetRect(&rect, x, y + 1, x, y + 1);
            pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, bcolor);
        }
        SetRect(&rect, x, y, x, y);
        pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, color);
        break;
    case centered:
        if (bordered)
        {
            SetRect(&rect, x - 1, y, x - 1, y);
            pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, bcolor);
            SetRect(&rect, x + 1, y, x + 1, y);
            pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, bcolor);
            SetRect(&rect, x, y - 1, x, y - 1);
            pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, bcolor);
            SetRect(&rect, x, y + 1, x, y + 1);
            pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, bcolor);
        }
        SetRect(&rect, x, y, x, y);
        pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, color);
        break;
    case righted:
        if (bordered)
        {
            SetRect(&rect, x - 1, y, x - 1, y);
            pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_RIGHT | DT_NOCLIP, bcolor);
            SetRect(&rect, x + 1, y, x + 1, y);
            pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_RIGHT | DT_NOCLIP, bcolor);
            SetRect(&rect, x, y - 1, x, y - 1);
            pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_RIGHT | DT_NOCLIP, bcolor);
            SetRect(&rect, x, y + 1, x, y + 1);
            pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_RIGHT | DT_NOCLIP, bcolor);
        }
        SetRect(&rect, x, y, x, y);
        pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_RIGHT | DT_NOCLIP, color);
        break;
    }
}

void Draw::Message(IDirect3DDevice9* device, char* text, float x, float y, int font, int orientation)
{
    RECT rect = { x, y, x, y };

    switch (orientation)
    {
    case lefted:
        pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_CALCRECT | DT_LEFT, BLACK(255));

        BoxRounded( device, x - 5, rect.top - 5, rect.right - x + 10, rect.bottom - rect.top + 10, 5, true, DARKGRAY(150), SKYBLUE(255));

        SetRect(&rect, x, y, x, y);
        pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, ORANGE(255));
        break;
    case centered:
        pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_CALCRECT | DT_CENTER, BLACK(255));

        BoxRounded(device, rect.left - 5, rect.top - 5, rect.right - rect.left + 10, rect.bottom - rect.top + 10, 5, true, DARKGRAY(150), SKYBLUE(255));

        SetRect(&rect, x, y, x, y);
        pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, ORANGE(255));
        break;
    case righted:
        pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_CALCRECT | DT_RIGHT, BLACK(255));

        BoxRounded(device, rect.left - 5, rect.top - 5, rect.right - rect.left + 10, rect.bottom - rect.top + 10, 5, true, DARKGRAY(150), SKYBLUE(255));

        SetRect(&rect, x, y, x, y);
        pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_RIGHT | DT_NOCLIP, ORANGE(255));
        break;
    }
}

void Draw::Sprite(IDirect3DDevice9* device, LPDIRECT3DTEXTURE9 tex, float x, float y, float resolution, float scale, float rotation)
{
    float screen_pos_x = x;
    float screen_pos_y = y;

    // Texture being used is 64x64:
    D3DXVECTOR2 spriteCentre = D3DXVECTOR2(resolution / 2, resolution / 2);

    // Screen position of the sprite
    D3DXVECTOR2 trans = D3DXVECTOR2(screen_pos_x, screen_pos_y);

    // Build our matrix to rotate, scale and position our sprite
    D3DXMATRIX mat;

    D3DXVECTOR2 scaling(scale, scale);

    // out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
    D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &spriteCentre, rotation, &trans);

    //device->SetRenderState(D3DRS_ZENABLE, false);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
    device->SetPixelShader(NULL);
    sSprite->Begin(NULL);
    sSprite->SetTransform(&mat); // Tell the sprite about the matrix
    sSprite->Draw(tex, NULL, NULL, NULL, 0xFFFFFFFF);
    sSprite->End();
}

bool Draw::Font()
{
    for (int i = 0; i < FontNr; i++)
        if (pFont[i]) return false;
    return true;
}

bool Draw::AddFont(IDirect3DDevice9* device, const char* Caption, float size, bool bold, bool italic)
{
    if (D3DXCreateFont(device, size, 0, (bold) ? FW_BOLD : FW_NORMAL, 1, (italic) ? 1 : 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, Caption, &pFont[FontNr]) == S_OK) {
        FontNr += 1;
        return true;
    }
    return false;
}

void Draw::FontReset()
{
    for (int i = 0; i < FontNr; i++) pFont[i]->OnLostDevice();
    for (int i = 0; i < FontNr; i++) pFont[i]->OnResetDevice();
}

void Draw::OnLostDevice()
{
    for (int i = 0; i < FontNr; i++) pFont[i]->OnLostDevice();
}