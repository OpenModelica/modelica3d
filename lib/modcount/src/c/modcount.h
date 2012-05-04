void* modcount_acquire_context();

void modcount_release_context(void* vctxt);

int modcount_get(void* vctxt);

int modcount_set(void* vctxt, int val);

void* modcount_acquire_string(const char* content);

void modcount_release_string(void* str);

const char* modcount_get_string(void* str);
