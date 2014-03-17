text_entry *text_entry_init(int x, int y);

// text_entry_input returns true if it has handled the event.
bool text_entry_input(text_entry *te, SDL_Event *sdlEvent);
// text_entry_text returns the text. This string is valid until the contents change.
const char* text_entry_text(text_entry *te);

int text_entry_render(text_entry *te);

void text_entry_destroy(text_entry *te);
