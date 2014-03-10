string_table* string_table_new();

// FIXME: Should string_table_get be thread-safe, as a matter of contract, as long as it's not concurrently written?

// string_table_get returns the value assigned to the given key or NULL if there's no such value.
// If ok is not NULL, it's used to indicate if the given key has an associated value.
void* string_table_get(string_table *st, const char *key, bool *ok);
void string_table_set(string_table *st, const char *key, void *val);
// Deleting a nonexistent key is a no-op.
void string_table_del(string_table *st, const char *key);

void string_table_free(string_table *st);
