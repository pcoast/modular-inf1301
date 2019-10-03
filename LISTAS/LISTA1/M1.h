#if defined (EXEMP_OWN)
    #define EXEMP_EXT

#else
    #define EXEMP_EXT extern

#endif

EXEMP_EXT int vetor[7]

#if defined (EXEMP_OWN)
    ={1,2,3,4,5,6,7}

#else
    ;

#endif