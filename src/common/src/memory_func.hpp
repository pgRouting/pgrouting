

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

