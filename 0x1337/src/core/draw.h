#ifndef _DRAW_H_
#define _DRAW_H_

#include <d3d9.h>
#include <d3dx9.h>

#include "dxcolors.h"

enum circle_type { full, half, quarter };
enum text_alignment { lefted, centered, righted };

#define MAX_FONTS 6

struct vertex
{
    FLOAT x, y, z, rhw;
    DWORD color;
};

namespace Draw
{

    //CDraw()
    //{
    //    g_pVB = NULL;
    //    g_pIB = NULL;
    //    int FontNr = 0;
    //}

    inline struct sScreen
    {
        float Width;
        float Height;
        float x_center;
        float y_center;
    } Screen;
    inline ID3DXFont* pFont[MAX_FONTS];
    inline bool fontsInitialized = false;

    void Sprite(IDirect3DDevice9* device,LPDIRECT3DTEXTURE9 tex, float x, float y, float resolution, float scale, float rotation);

    //=============================================================================================
    void Line(IDirect3DDevice9* device, float x1, float y1, float x2, float y2, float width, bool antialias, DWORD color);

    void Box(IDirect3DDevice9* device, float x, float y, float w, float h, float linewidth, DWORD color);
    void BoxFilled(IDirect3DDevice9* device, float x, float y, float w, float h, DWORD color);
    void BoxBordered(IDirect3DDevice9* device, float x, float y, float w, float h, float border_width, DWORD color, DWORD color_border);
    void BoxRounded(IDirect3DDevice9* device, float x, float y, float w, float h, float radius, bool smoothing, DWORD color, DWORD bcolor);

    void Circle(IDirect3DDevice9* device, float x, float y, float radius, int rotate, int type, bool smoothing, int resolution, DWORD color);
    void CircleFilled(IDirect3DDevice9* device, float x, float y, float rad, float rotate, int type, int resolution, DWORD color);

    void Text(const char* text, float x, float y, int orientation, int font, bool bordered, DWORD color, DWORD bcolor);
    void Message(IDirect3DDevice9* device, char* text, float x, float y, int font, int orientation);
    //=============================================================================================

    //=============================================================================================
    bool Font();
    bool AddFont(IDirect3DDevice9* device, const char* Caption, float size, bool bold, bool italic);
    void FontReset();
    void OnLostDevice();
    //=============================================================================================

    //void GetDevice(LPDIRECT3DDEVICE9 pDev) { pDevice = pDev; }
    void Reset(DWORD Width, DWORD Height);

    //LPDIRECT3DDEVICE9 pDevice;
    inline LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL;    // Buffer to hold vertices
    inline LPDIRECT3DINDEXBUFFER9  g_pIB = NULL;    // Buffer to hold indices

    inline int FontNr = 0;
    inline LPD3DXSPRITE sSprite = NULL;
};

//extern CDraw Draw;

#endif /* _DRAW_H_ */