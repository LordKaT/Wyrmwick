#include "include.h"

static int _where(string_table *st, const char *key, bool *found);

string_table* string_table_new() {
	return table_new(sizeof(string_table_entry), 0, 16);
}

void* string_table_get(string_table *st, const char *key, bool *ok) {
	int i;
	bool found;
	string_table_entry *tab = (string_table_entry*) st->m_data;
	
	i = _where(st, key, &found);
	if (ok) { *ok = found; }
	if (found) {
		return tab[i].m_pVal;
	}
	return nullptr;
}

void string_table_set(string_table *st, const char *key, void *val) {
	int i;
	bool found;
	string_table_entry *tab = (string_table_entry*) st->m_data;
	
	i = _where(st, key, &found);
	if (found) {
		tab[i].m_pVal = val;
		return;
	}
	
	string_table_entry newentry = {strdup(key), val};
	table_insert(st, i, &newentry);
}

void string_table_del(string_table *st, const char *key) {
	int i;
	bool found;
	string_table_entry *tab = (string_table_entry*) st->m_data;
	
	i = _where(st, key, &found);
	if (! found) { return; }
	free(tab[i].m_sKey);
	table_remove(st, i);
}

void string_table_free(string_table *st) {
	table_free(st);
}

int _where(string_table *st, const char *key, bool *found) {
	int a, b, mid, cmp;
	string_table_entry *tab = (string_table_entry*) st->m_data;
	
	a = 0;
	b = st->m_len;
	
	while (a != b) {
		mid = a + (b - a)/2;
		cmp = strcmp(tab[mid].m_sKey, key);
		if (cmp == 0) {
			*found = true;
			return mid;
		}
		
		if (cmp > 0) {
			b = mid;
			continue;
		}
		a = mid + 1;
	}
	*found = false;
	return a;
}
