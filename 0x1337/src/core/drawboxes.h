#pragma once
#include <d3d9.h>
#include <chrono>

enum { DRAW_NOT_READY, DRAW_READY };

// https://www.unknowncheats.me/forum/d3d-tutorials-and-source/126704-drawing-function-directx-9-a.html
namespace box {
	void DrawBoxes(IDirect3DDevice9* device) noexcept;
	void DrawFilledRect(IDirect3DDevice9* device, int x, int y, int w, int h, D3DCOLOR col);


	struct BoxDrawCommand {
		int x, y, w, h;
		unsigned int namePtr;
	};

	inline BoxDrawCommand buffer_copy[256];
	inline int draw_count_copy = 0;

	typedef std::chrono::system_clock timer;
	inline timer::time_point lastUpdate;

	inline volatile struct BoxDrawCommandBuffer {
		// this is so that we can pattern scan to find the buffer
		unsigned int signature = 0xC0CCF00D;//0x0
		// for telling the other program what screen size we are working with for w2s
		unsigned int screen_width = 0; // 0x04
		unsigned int screen_height = 0;// 0x08
		// set this before reading from the buffer to tell the dma not to touch it until done
		int reading = 0;//0x0C

				// set this when we wanna unload our program to tell the other computer to cease writes
		unsigned int wants_unload = 0; // 0x10
		// wait until this is set before closing program
		unsigned int safe_unload = 0; //  0x14 

		// this is how many items into the buffer we should draw. Gets set on each write by the DMA Card.
		unsigned int draw_count = 0; //   0x18
		// this gets turned off during dma writes so we don't draw partial results
		unsigned int draw_ready = DRAW_NOT_READY; // 0x1C

		BoxDrawCommand buffer[256];
	} box_draw_command_buffer;
}