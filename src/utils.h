// sdl_must() prints an error and calls sys_abort() if the argument is negative.
// It is meant to be used when there's absolutely no hope of recovering or otherwise
// reasonably responding to an error condition, e.g. when drawing to an in-memory image.
#define sdl_must(X) _utils_sdl_must((X), __FILE__, __LINE__, __FUNCTION__)
void _utils_sdl_must(int retVal, const char *file, int line, const char *func);

// util_textInput appends typed characters to cBuffer and returns 1 when text input ends.
// You must call SDL_StartTextInput() before calling this.
int util_textInput(SDL_Event *sdlEvent, char **cBuffer);

// state_stack_kill sets m_isDead of all state descriptors on the stack to true.
// Use this if you want to quit the game immediately, but in an orderly manner.
// (closing files, freeing memory etc.)
void state_stack_kill(state_stack *stack);
