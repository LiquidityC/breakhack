#include <stdbool.h>
#include "menu.h"
#include "util.h"
#include "sprite.h"
#include "defines.h"
#include "gui_button.h"

typedef struct MenuItems_t {
	Sprite *sprite;
	Sprite *hsprite;
	GuiButton *button;
	bool selected;
} MenuItem;

Menu *
menu_create(void)
{
	Menu *menu = ec_malloc(sizeof(Menu));
	menu->items = linkedlist_create();
	menu->selected = 0;
	return menu;
}

void
menu_handle_event(Menu *m, SDL_Event *event)
{
	UNUSED(m);
	LinkedList *items;

	if (event->type != SDL_KEYDOWN && event->type != SDL_MOUSEMOTION) {
		return;
	}

	items = m->items;
	if (event->type == SDL_MOUSEMOTION) {
		while (items) {
			MenuItem *item = items->data;
			items = items->next;
			gui_button_handle_event(item->button, event);
			item->selected = item->button->hover;
		}
	}
}

void
menu_item_add(Menu *m, Sprite *s1, Sprite *s2)
{
	MenuItem *item = ec_malloc(sizeof(MenuItem));
	item->sprite = s1;
	item->hsprite = s2;
	item->selected = false;

	SDL_Rect area = {
		item->sprite->pos.x,
		item->sprite->pos.y,
		item->sprite->textures[0]->dim.width,
		item->sprite->textures[0]->dim.height
	};
	item->button = gui_button_create(area, NULL, NULL);
	linkedlist_append(&m->items, item);
}

void
menu_render(Menu *m, Camera *cam)
{
	LinkedList *items = m->items;

	while (items) {
		MenuItem *item = items->data;
		if (item->selected)
			sprite_render(item->hsprite, cam);
		else
			sprite_render(item->sprite, cam);
		items = items->next;
	}
}

static void
menu_item_destroy(MenuItem *item)
{
	if (item->sprite)
		sprite_destroy(item->sprite);
	if (item->hsprite)
		sprite_destroy(item->hsprite);
	gui_button_destroy(item->button);
	free(item);
}

void
menu_destroy(Menu *m)
{
	LinkedList *items = m->items;
	while (items) {
		menu_item_destroy(items->data);
		items = items->next;
	}
	free(m);
}
