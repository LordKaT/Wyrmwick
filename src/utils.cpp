#include "include.h"

int util_textInput(SDL_Event *sdlEvent, char **cBuffer) {
	char *cTemp;
	char *cClipboardText = SDL_GetClipboardText();
	if (sdlEvent->type == SDL_KEYDOWN) {
		/* Backspace */
		cTemp = (char *)malloc(sizeof(char) * strlen(*cBuffer) + 1);
		strcpy(cTemp, *cBuffer);
		if (sdlEvent->key.keysym.sym == SDLK_BACKSPACE) {
			for (unsigned int i = 0; i < strlen(cTemp) + 1; i++) {
				if (cTemp[i] == '\0' && i > 0) {
					cTemp[i - 1] = '\0';
				}
			}
			strcpy(*cBuffer, cTemp);
		}

		/* Paste from clipboard (cross-platform). */
		if (sdlEvent->key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
			if (cClipboardText == nullptr) {
				return 0;
			}
			cTemp = (char *)malloc(sizeof(char) * (strlen(*cBuffer) + 1));
			memset(cTemp, 0, sizeof(char) * (strlen(cClipboardText) + strlen(*cBuffer) + 2));
			strcpy(cTemp, *cBuffer);
			strcat(cTemp, cClipboardText);
			strcpy(cTemp, *cBuffer);
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
		/* Ignore CTRL+V (had odd behaviour with SDL_GetClipboardText). */
		if (!((sdlEvent->text.text[0] == 'v' || sdlEvent->text.text[0] == 'V' ) && SDL_GetModState() & KMOD_CTRL)) {
			char *cTemp;
			cTemp = (char *)malloc(sizeof(char) * (strlen(*cBuffer) + 1));
			memset(cTemp, 0, sizeof(char) * (strlen(*cBuffer) + 1));
			strcpy(cTemp, *cBuffer);
			*cBuffer = (char *)realloc(*cBuffer, sizeof(char) * (strlen(cTemp) + strlen(sdlEvent->text.text) + 2));
			strcpy(*cBuffer, cTemp);
			strcat(*cBuffer, sdlEvent->text.text);
			free(cTemp);
			return 0;
		}
	}
	return 0;
}
