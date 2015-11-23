

template <typename T>
T*
get_memory(size_t size, T *ptr){
    if( !ptr ){
        ptr = (T*) malloc(size * sizeof(T));
    } else {
        ptr = (T*) realloc( ptr, size * sizeof(T));
    }
    return (T*) ptr;
}

template <typename T>
T*
noResult(size_t *count, T *ptr) {
    count = 0;
    if( ptr )
        free(ptr);
    return NULL;
}

