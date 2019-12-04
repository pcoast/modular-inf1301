#if defined (EXEMP_OWN)
    #define EXEMP_EXT

#else
    #define EXEMP_EXT extern

#endif

EXEMP_EXT int var

#if defined (EXEMP_OWN)
    = 2;

#else
    ;

#endif