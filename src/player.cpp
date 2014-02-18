#include "include.h"

player g_player;

void player_init() {
	g_player.m_image = image_load("data/images/DebugMan3.bmp", true, 255, 0, 255);
	g_player.m_iFacing = 1;
	g_player.m_animator.m_iNumFrames = 2;
	g_player.m_animator.m_iCurrFrame = 0;
	
	g_player.m_animator.m_rectSource.x = 0;
	g_player.m_animator.m_rectSource.y = 0;
	g_player.m_animator.m_rectSource.w = 32;
	g_player.m_animator.m_rectSource.h = 32;

	g_player.m_animator.m_rectDest.x = 32;
	g_player.m_animator.m_rectDest.y = 32;
	g_player.m_animator.m_rectDest.w = 32;
	g_player.m_animator.m_rectDest.h = 32;

	animator_add(&g_player.m_animator, 150, &g_player.m_animator);

	return;
}

void player_input(SDL_Event *sdlEvent) {
	animator_input(sdlEvent);
	return;
}

bool player_collide() {
	return false;
}

void player_render() {
	image_draw(g_player.m_image, &g_player.m_animator.m_rectSource, &g_player.m_animator.m_rectDest);
	return;
}

void player_destroy() {
	return;
}
