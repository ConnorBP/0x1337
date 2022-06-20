#include "gui.h"

//#define WINDOW_WIDTH 800
//#define WINDOW_HEIGHT 600
//
//#define NK_INCLUDE_FIXED_TYPES
//#define NK_INCLUDE_STANDARD_IO
//#define NK_INCLUDE_STANDARD_VARARGS
//#define NK_INCLUDE_DEFAULT_ALLOCATOR
//#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
//#define NK_INCLUDE_FONT_BAKING
//#define NK_INCLUDE_DEFAULT_FONT
//#define NK_IMPLEMENTATION
//#define NK_D3D9_IMPLEMENTATION
//#define __cplusplus
//
//#include "../../ext/nk/nuklear.h"
//#include "../../ext/nk/nuklear_d3d9.h"

#include "nk_header.h"

#include "hooks.h"
#include "config.h"
#include "../util/Logging.h"

//imgui window process
//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
//	HWND window,
//	UINT message,
//	WPARAM wideParam,
//	LPARAM longParam
//);

// our window process handler (for events such as clicks)
LRESULT CALLBACK WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParam,
	LPARAM longParam
);

bool gui::SetupWindowClass(const char* windowClassName) noexcept
{
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	windowClass.lpfnWndProc = DefWindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.hIcon = NULL;
	windowClass.hCursor = NULL;
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = windowClassName;
	windowClass.hIconSm = NULL;

	// register the class
	if (!RegisterClassEx(&windowClass)) {
		return false;
	}
	else {
		return true;
	}
}
void gui::DestroyWindowClass() noexcept{
	// free up the window class when we don't need it anymore
	UnregisterClass(
		windowClass.lpszClassName,
		windowClass.hInstance
	);
}

bool gui::SetupWindow(const char* windowName) noexcept{
	window = CreateWindowA(
		windowClass.lpszClassName,
		windowName,
		WS_EX_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,//window dimensions
		0,
		NULL, //no parent
		NULL, //no menu
		windowClass.hInstance,
		NULL
	);
	if (!window) {
		return false;
	}
	else {
		return true;
	}
}
void gui::DestroyWindow() noexcept{
	if (window) {
		DestroyWindow(window);
	}
}

bool gui::SetupDirectX() noexcept{
	// get handle to direct x dll
	const auto handle = GetModuleHandle("d3d9.dll");

	if (!handle)
		return false;

	using CreateFn = LPDIRECT3D9(__stdcall*)(UINT);

	// use get proc addess to retreice d3d9.dll exported fn Direct3DCreate9 address that the game has
	const auto create = reinterpret_cast<CreateFn>(GetProcAddress(
		handle,
		"Direct3DCreate9"
	));
	if (!create)
		return false;

	d3d9 = create(D3D_SDK_VERSION);

	if (!d3d9)
		return false;

	presentParams.BackBufferWidth = 0;
	presentParams.BackBufferHeight = 0;
	presentParams.BackBufferFormat = D3DFMT_UNKNOWN;
	presentParams.BackBufferCount = 0;
	presentParams.MultiSampleType = D3DMULTISAMPLE_NONE;
	presentParams.MultiSampleQuality = NULL;
	presentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParams.hDeviceWindow = window;
	presentParams.Windowed = 1;
	presentParams.EnableAutoDepthStencil = 0;
	presentParams.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
	presentParams.Flags = NULL;
	presentParams.FullScreen_RefreshRateInHz = 0;
	presentParams.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	// create device
	if (d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_NULLREF,
		window,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT,
		&presentParams,
		&device
	) < 0) return false;

	return true;

}
void gui::DestroyDirectX() noexcept{
	if (device) {
		device->Release();
		device = NULL;
	}
	if (d3d9) {
		d3d9->Release();
		d3d9 = NULL;
	}
}

// setup device
void gui::Setup(){
	if (!SetupWindowClass("0x1337Class")) {
		throw std::runtime_error("Failed to create window class.");
	}

	if (!SetupWindow("0x1337Window")) {
		throw std::runtime_error("Failed to create window.");
	}
	if (!SetupDirectX()) {
		throw std::runtime_error("Failed to create device.");
	}

	// destroy our temporary window
	// as we will not need it when we steal the window from the game in setup
	DestroyWindow();
	DestroyWindowClass();
}

void gui::SetupMenu(LPDIRECT3DDEVICE9 device) noexcept{

	// get the direct x paramaters from the actual game so we can steal them
	auto params = D3DDEVICE_CREATION_PARAMETERS{};
	device->GetCreationParameters(&params);

	// set our local window variable to the games window
	window = params.hFocusWindow;
	originalWindowProcess = 
		reinterpret_cast<WNDPROC>(
			SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcess))
		);

	//RECT rect = { 0, 0, 0, 0 };
	//GetWindowRect(window, &rect);

	D3DVIEWPORT9 viewport;
	device->GetViewport(&viewport);

	ctx = nk_d3d9_init(device, viewport.Width, viewport.Height);
	/* Load Fonts: if none of these are loaded a default font will be used  */
	/* Load Cursor: if you uncomment cursor loading please hide the cursor */
	{struct nk_font_atlas* atlas;
	nk_d3d9_font_stash_begin(&atlas);
	/*struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../../extra_font/DroidSans.ttf", 14, 0);*/
	/*struct nk_font *robot = nk_font_atlas_add_from_file(atlas, "../../extra_font/Roboto-Regular.ttf", 14, 0);*/
	/*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
	/*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../extra_font/ProggyClean.ttf", 12, 0);*/
	/*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../extra_font/ProggyTiny.ttf", 10, 0);*/
	/*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../extra_font/Cousine-Regular.ttf", 13, 0);*/
	nk_d3d9_font_stash_end();
	/*nk_style_load_all_cursors(ctx, atlas->cursors);*/
	/*nk_style_set_font(ctx, &droid->handle)*/; }

	/* style.c */
	#ifdef INCLUDE_STYLE
	/*set_style(ctx, THEME_WHITE);*/
	/*set_style(ctx, THEME_RED);*/
	/*set_style(ctx, THEME_BLUE);*/
	/*set_style(ctx, THEME_DARK);*/
	#endif
	//bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 0.6f;
	doneSetup = true;
}
void gui::Destroy() noexcept{
	nk_d3d9_shutdown();

	// revert the window process to send events to the game instead of us
	SetWindowLongPtr(
		window,
		GWLP_WNDPROC,
		reinterpret_cast<LONG_PTR>(originalWindowProcess)
	);

	//finally, destroy direct x
	DestroyDirectX();
}

void gui::Render() noexcept{

	static struct nk_colorf bg { 0.10f, 0.18f, 0.24f, 0.6f };
	static struct nk_colorf highlightf {1.0f,1.0f,1.0f,1.0f};
	struct nk_color highlight = nk_rgb_cf(highlightf); //{152,43,220,255};

	ctx->style.checkbox.cursor_hover = nk_style_item_color(nk_rgb(highlight.r, highlight.g, 169));
	ctx->style.checkbox.cursor_normal = nk_style_item_color(highlight);

	//////

	if (nk_window_is_closed(ctx, "0x1337"))
		gui::open = false;

	if (nk_begin(ctx, "0x1337", nk_rect(50, 50, 300, 340),
		NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
		NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE | NK_WINDOW_CLOSABLE))
	{
		nk_layout_row_dynamic(ctx, 30, 3);
		nk_label(ctx, "trigger:", NK_TEXT_LEFT);
		if (nk_option_label(ctx, "hitchance", config::legitBot.triggerType == TRIG_HITC)) config::legitBot.triggerType = TRIG_HITC;
		if (nk_option_label(ctx, "fast", config::legitBot.triggerType == TRIG_FAST)) config::legitBot.triggerType = TRIG_FAST;
		if (config::legitBot.triggerType == TRIG_HITC) {
			nk_layout_row_dynamic(ctx, 22, 1);
			nk_property_int(ctx, "Hitchance:", 0, &config::legitBot.triggerHitChance, 100, 1, 1);
		}

		nk_layout_row_dynamic(ctx, 20, 1);
		nk_label(ctx, "misc:", NK_TEXT_LEFT);

		nk_layout_row_dynamic(ctx, 30, 2);
		nk_checkbox_label(ctx, "bhop", &config::misc.enableBhop);
		nk_checkbox_label(ctx, "auto accept", &config::misc.enableAutoAccept);

		nk_layout_row_dynamic(ctx, 20, 1);
		nk_label(ctx, "chams:", NK_TEXT_LEFT);

		nk_layout_row_dynamic(ctx, 30, 2);
		nk_checkbox_label(ctx, "enemy", &config::chams.enableEnemy);
		nk_checkbox_label(ctx, "ignore Z", &config::chams.enableEnemyZ);

		nk_layout_row_dynamic(ctx, 25, 1);
		if (nk_combo_begin_color(ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(ctx), 400))) {
			nk_layout_row_dynamic(ctx, 120, 1);
			bg = nk_color_picker(ctx, bg, NK_RGBA);
			nk_layout_row_dynamic(ctx, 25, 1);
			bg.r = nk_propertyf(ctx, "#R:", 0, bg.r, 1.0f, 0.01f, 0.005f);
			bg.g = nk_propertyf(ctx, "#G:", 0, bg.g, 1.0f, 0.01f, 0.005f);
			bg.b = nk_propertyf(ctx, "#B:", 0, bg.b, 1.0f, 0.01f, 0.005f);
			bg.a = nk_propertyf(ctx, "#A:", 0, bg.a, 1.0f, 0.01f, 0.005f);

			config::chams.enemyColor = fcolor4 {bg.r,bg.g,bg.b,bg.a};

			nk_combo_end(ctx);
		}


		// menu highlight color picker
		nk_layout_row_dynamic(ctx, 20, 1);
		nk_label(ctx, "menu highlight:", NK_TEXT_LEFT);
		nk_layout_row_dynamic(ctx, 25, 1);
		if (nk_combo_begin_color(ctx, nk_rgb_cf(highlightf), nk_vec2(nk_widget_width(ctx), 400))) {
			nk_layout_row_dynamic(ctx, 120, 1);
			highlightf = nk_color_picker(ctx, highlightf, NK_RGBA);
			nk_layout_row_dynamic(ctx, 25, 1);
			highlightf.r = nk_propertyf(ctx, "#R:", 0, highlightf.r, 1.0f, 0.01f, 0.005f);
			highlightf.g = nk_propertyf(ctx, "#G:", 0, highlightf.g, 1.0f, 0.01f, 0.005f);
			highlightf.b = nk_propertyf(ctx, "#B:", 0, highlightf.b, 1.0f, 0.01f, 0.005f);
			highlightf.a = nk_propertyf(ctx, "#A:", 0, highlightf.a, 1.0f, 0.01f, 0.005f);
			nk_combo_end(ctx);
		}

		//unhook button
		nk_layout_row_static(ctx, 30, 80, 1);
		if (nk_button_label(ctx, "unhook"))
			globals::shouldUnload = true;
	}
	nk_end(ctx);

	/* Draw */
	{
		HRESULT hr;
		//hr = IDirect3DDevice9_Clear(device, 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		//	D3DCOLOR_COLORVALUE(bg.r, bg.g, bg.b, bg.a), 0.0f, 0);
		//NK_ASSERT(SUCCEEDED(hr));

		//hr = IDirect3DDevice9_BeginScene(device);
		//NK_ASSERT(SUCCEEDED(hr));
		nk_d3d9_render(NK_ANTI_ALIASING_ON);
		//hr = IDirect3DDevice9_EndScene(device);
		//NK_ASSERT(SUCCEEDED(hr));

		
		//hr = IDirect3DDevice9_Present(device, NULL, NULL, NULL, NULL);
		//if (hr == D3DERR_DEVICELOST || hr == D3DERR_DEVICEHUNG || hr == D3DERR_DEVICEREMOVED) {
		//	/* to recover from this, you'll need to recreate device and all the resources */
		//	MessageBoxW(NULL, L"D3D9 device is lost or removed!", L"Error", 0);
		//	//break;
		//}
		//else if (hr == S_PRESENT_OCCLUDED) {
		//	/* window is not visible, so vsync won't work. Let's sleep a bit to reduce CPU usage */
		//	Sleep(10);
		//}
		//NK_ASSERT(SUCCEEDED(hr));
	}
	/* Input */
	MSG msg;
	nk_input_begin(ctx);
	while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT)
			log_console("Nuklear | Quit Button Pressed", Color::Red);
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	nk_input_end(ctx);
}

LRESULT CALLBACK WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParam,
	LPARAM longParam
)
{
	//toggle menu 0x55 is U key
	// (&1 makes it only detect the click once, and not multiple times when held)
	if (GetAsyncKeyState(VK_INSERT) & 1) {
		gui::open = !gui::open;
	}

	// pass windproc messages to imgui if menu is open
	if (gui::open && nk_d3d9_handle_event(window, message, wideParam, longParam)) {
		return 1L;
	}

	// Call the games original window proc so that it still receives events
	return CallWindowProc(
		gui::originalWindowProcess,
		window,
		message,
		wideParam,
		longParam
	);
}

long __stdcall hooks::EndScene(IDirect3DDevice9* device) noexcept {

	//csgo is weird and calls endscene twice, so this makes sure it only runs on one of them
	static const auto returnAddress = _ReturnAddress();

	// forward to the original func no matter what
	HRESULT hr = EndSceneOriginal(device, device);

	// only continue when the return address is the same as the first
	if (_ReturnAddress() != returnAddress)
		return hr;

	if (!gui::doneSetup)
		gui::SetupMenu(device);

	if (gui::open)
		gui::Render();

	return hr;
}

HRESULT __stdcall hooks::Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept {

	HRESULT hr;

	if (gui::ctx)
	{
		nk_free(gui::ctx);
		gui::ctx = NULL;
	}

	nk_d3d9_release();

	hr = ResetOriginal(device, device, params);
	if (SUCCEEDED(hr))
	{
		D3DVIEWPORT9 viewport;
		device->GetViewport(&viewport);

		gui::ctx = nk_d3d9_init(device, viewport.Width, viewport.Height);
		struct nk_font_atlas* atlas;
		nk_d3d9_font_stash_begin(&atlas);
		nk_d3d9_font_stash_end();
	}


	return hr;
}