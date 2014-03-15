#include "include.h"

void _utils_sdl_must(int retVal, const char *file, int line, const char *func) {
	if (retVal >= 0) { return; }
	printf("%s:%d: %s(): %s\n", file, line, func, SDL_GetError());
	sys_abort();
}

int util_textInput(SDL_Event *sdlEvent, char **cBuffer) {
	char *cTemp;
	char *cClipboardText;
	if (sdlEvent->type == SDL_KEYDOWN) {
		/* Backspace */
		if (sdlEvent->key.keysym.sym == SDLK_BACKSPACE) {
			cTemp = strdup(*cBuffer);
			for (unsigned int i = 0; i < strlen(cTemp) + 1; i++) {
				if (cTemp[i] == '\0' && i > 0) {
					cTemp[i - 1] = '\0';
				}
			}
			strcpy(*cBuffer, cTemp);
			free(cTemp);
		}

		/* Paste from clipboard (cross-platform). */
		if (sdlEvent->key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
			cClipboardText = SDL_GetClipboardText();
			if (cClipboardText == nullptr) {
				return 0;
			}
			cTemp = strdup(*cBuffer);
			*cBuffer = (char *)realloc(*cBuffer, sizeof(char) * (strlen(cTemp) + strlen(cClipboardText) + 2));
			strcpy(*cBuffer, cTemp);
			strcat(*cBuffer, cClipboardText);
			free(cTemp);
			return 0;
		}

		/* Hit RETURN, stop text input. */
		if (sdlEvent->key.keysym.sym == SDLK_RETURN) {
			SDL_StopTextInput();
			return 1;
		}
	}

	/* Currently getting text input. */
	if (sdlEvent->type == SDL_TEXTINPUT) {
		// Ignore ctrl+v so we don't type 'v'
		if (!((sdlEvent->text.text[0] == 'v' || sdlEvent->text.text[0] == 'V' ) && SDL_GetModState() & KMOD_CTRL)) {
			cTemp = strdup(*cBuffer);
			*cBuffer = (char *)realloc(*cBuffer, sizeof(char) * (strlen(cTemp) + strlen(sdlEvent->text.text) + 2));
			strcpy(*cBuffer, cTemp);
			strcat(*cBuffer, sdlEvent->text.text);
			free(cTemp);
			return 0;
		}
	}
	return 0;
}

void state_stack_kill(state_stack *stack) {
	state_desc* states = (state_desc*) stack->m_data;
	for (int i = 0; i < stack->m_len; i++) {
		states[i].m_isDead = true;
	}
}

