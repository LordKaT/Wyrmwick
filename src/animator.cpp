#include "include.h"

/* Start Visual C++ nonsense. */

struct animatorWrapper {
	void (*m_vFunc)(void *vData);
};

animatorWrapper g_aniWrap;

/* End Visual C++ mnonsense. */

void animator_init() {
	g_aniWrap.m_vFunc = &animator_animate;
	return;
}

void animator_destroy() {
	return;
}

void animator_animate(void *vData) {
	animation *temp = (animation *)vData;
	if (temp->m_iCurrFrame < temp->m_iNumFrames) {
		temp->m_iCurrFrame++;
	} else {
		temp->m_iCurrFrame = 0;

	}
	temp->m_rectSource.x = temp->m_iCurrFrame * 32;
	temp->m_rectSource.y = temp->m_iRow * 32;
	return;
}

Uint32 animator_callback(Uint32 iInterval, void *vParam) {
	SDL_Event sdlEvent;
	SDL_UserEvent userEvent;
	userEvent.type = SDL_USEREVENT;
	userEvent.code = 0;
	userEvent.data1 = &g_aniWrap;
	userEvent.data2 = vParam;
	sdlEvent.type = SDL_USEREVENT;
	sdlEvent.user = userEvent;
	SDL_PushEvent(&sdlEvent);
	return iInterval;
}

void animator_input(SDL_Event *sdlEvent) {
	if (sdlEvent->type == SDL_USEREVENT) {
		// this nonsense courtesy Visual C++
		animatorWrapper *temp = (animatorWrapper *)sdlEvent->user.data1;
		if (temp->m_vFunc != nullptr) {
			temp->m_vFunc(sdlEvent->user.data2);
		}
		/*
		And now, for a fireside chat with LordKaT.

		Under normal circumstances, I would just do this:
		
		void (*p)(void *) = sdlEvent->user.data1;
		p(sdlEvent->user.data2);
		
		And all would be fine.

		But, thanks to some shit interpretation of the C++ standard, I can't.
		Instead, I need to wrap a function address in a global that's wrapped in
		a struct, because C++ doesn't understand that I just want to call a fucking
		function address.

		This method works in GCC by the way. :|
		*/
	}
	return;
}

void animator_add(animation *aAnim, int iInterval, void *vParam) {
	aAnim->m_timerID = SDL_AddTimer(iInterval, animator_callback, vParam);
	return;
}
