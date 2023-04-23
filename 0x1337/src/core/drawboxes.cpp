#include "drawboxes.h"
#include "draw.h"
#include "config.h"

void box::DrawBoxes(IDirect3DDevice9* device) noexcept {
	// Debug Border line to show that everything is in order
	if(config::drawboxes.enableDebug)
		Draw::Box(device, 0, 0, Draw::Screen.Width-1.0, Draw::Screen.Height - 1.0, 1.0, D3DCOLOR_ARGB(100, 200, 50, 50));

	if (!Draw::fontsInitialized)
	{
	//	//Draw::AddFont(device, "Tahoma", 15, false, false);
	//	//Draw::AddFont(device, "Verdana", 15, true, false);
	//	//Draw::AddFont(device, "Verdana", 13, true, false);
		Draw::AddFont(device, "Comic Sans MS", 20, true, false);
		Draw::AddFont(device, "DejaVu Sans Mono", 18, false, false);

		Draw::fontsInitialized = true;
	}

	if (box_draw_command_buffer.draw_ready == DRAW_READY && box_draw_command_buffer.draw_count > 0 && box_draw_command_buffer.draw_count <= 256) {
		box_draw_command_buffer.reading = true;
		memcpy(
			(void*)buffer_copy,
			(void*)box_draw_command_buffer.buffer,
			sizeof(BoxDrawCommand) * (size_t)min(255, box_draw_command_buffer.draw_count)
		);
		draw_count_copy = box_draw_command_buffer.draw_count;
		box_draw_command_buffer.reading = false;
		lastUpdate = timer::now();
	}

	if (std::chrono::duration_cast<std::chrono::milliseconds>(timer::now() - lastUpdate).count() > 20) {
		return;
	}
	
	for (int i = 0; i < draw_count_copy && i < 256; i++) {
		volatile BoxDrawCommand* cmd = &buffer_copy[i];
		//DrawFilledRect(device, cmd->x, cmd->y, cmd->w, cmd->h, cmd->col);
		Draw::Box(
			device,
			cmd->x,
			cmd->y,
			cmd->w,
			cmd->h,
			1.0,
			D3DCOLOR_ARGB(
				(BYTE)(config::drawboxes.enemyColor.a*255.0),
				(BYTE)(config::drawboxes.enemyColor.r * 255.0),
				(BYTE)(config::drawboxes.enemyColor.g * 255.0),
				(BYTE)(config::drawboxes.enemyColor.b * 255.0)
			)
		/*cmd->col*/
		);

		DWORD text_x = cmd->x + cmd->w / 2;
		DWORD text_y = cmd->y - 30;

		if (config::drawboxes.drawNames && cmd->namePtr != 0 && cmd->namePtr != 0x10) {
			Draw::Text(
				(char*)cmd->namePtr,
				text_x,
				text_y,
				text_alignment::centered,
				0,
				false,
				D3DCOLOR_ARGB(
					(BYTE)(config::drawboxes.nameColor.a * 255.0),
					(BYTE)(config::drawboxes.nameColor.r * 255.0),
					(BYTE)(config::drawboxes.nameColor.g * 255.0),
					(BYTE)(config::drawboxes.nameColor.b * 255.0)
				),
				BLACK(255)
			);
		}
	}
}

void box::DrawFilledRect(IDirect3DDevice9* device, int x, int y, int w, int h, D3DCOLOR col) {
	D3DRECT rect = { x,y, x + w, y + h };
	device->Clear(1, &rect, D3DCLEAR_TARGET, col, 0, 0);
}