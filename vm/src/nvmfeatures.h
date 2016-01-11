#ifndef _NVMFEAUTURES_H_
#define _NVMFEAUTURES_H_

// checking flags...
#ifdef NVM_USE_SWITCH
# ifndef NVM_USE_LOOKUPSWITCH
#  define NVM_USE_LOOKUPSWITCH
# endif
# ifndef NVM_USE_TABLESWITCH
#  define NVM_USE_TABLESWITCH
# endif
#endif

// checking int size flags
#ifdef NVM_USE_16BIT_WORD
# ifdef NVM_USE_32BIT_WORD
#  error "you must define either NVM_USE_16BIT_WORD or NVM_USE_32BIT_WORD, not both!"
# endif
#else 
# ifndef NVM_USE_32BIT_WORD
#  define NVM_USE_16BIT_WORD
# endif
#endif

// checking float flags
#ifdef NVM_USE_FLOAT
# ifndef NVM_USE_32BIT_WORD
#  error "NVM_USE_FLOAT is only allowed with NVM_USE_32BIT_WORD!"
# endif
#endif


#define NVMFILE_VERSION    2
#define NVMFILE_MAGIC      0xBE000000L


#define NVM_FEAUTURE_LOOKUPSWITCH (1L<<0)
#define NVM_FEAUTURE_TABLESWITCH  (1L<<1)
#define NVM_FEAUTURE_32BIT        (1L<<2)
#define NVM_FEAUTURE_FLOAT        (1L<<3)
#define NVM_FEAUTURE_ARRAY        (1L<<4)
#define NVM_FEAUTURE_INHERITANCE  (1L<<5)
#define NVM_FEAUTURE_EXTSTACK     (1L<<6)

#ifndef NVM_USE_LOOKUPSWITCH
# undef NVM_FEAUTURE_LOOKUPSWITCH
# define NVM_FEAUTURE_LOOKUPSWITCH 0
#endif

#ifndef NVM_USE_TABLESWITCH
# undef NVM_FEAUTURE_TABLESWITCH
# define NVM_FEAUTURE_TABLESWITCH 0
#endif

#ifndef NVM_USE_32BIT_WORD
# undef NVM_FEAUTURE_32BIT
# define NVM_FEAUTURE_32BIT 0
#endif

#ifndef NVM_USE_FLOAT
# undef NVM_FEAUTURE_FLOAT
# define NVM_FEAUTURE_FLOAT 0
#endif

#ifndef NVM_USE_ARRAY
# undef NVM_FEAUTURE_ARRAY
# define NVM_FEAUTURE_ARRAY 0
#endif

#ifndef NVM_USE_INHERITANCE
# undef NVM_FEAUTURE_INHERITANCE
# define NVM_FEAUTURE_INHERITANCE 0
#endif

#ifndef NVM_USE_EXTSTACKOPS
# undef NVM_FEAUTURE_EXTSTACK
# define NVM_FEAUTURE_EXTSTACK 0
#endif


#define NVM_MAGIC_FEAUTURE (NVMFILE_MAGIC\
                           |NVM_FEAUTURE_LOOKUPSWITCH\
                           |NVM_FEAUTURE_TABLESWITCH\
                           |NVM_FEAUTURE_32BIT\
                           |NVM_FEAUTURE_FLOAT\
                           |NVM_FEAUTURE_ARRAY\
                           |NVM_FEAUTURE_INHERITANCE)


#endif // _NVMFEAUTURES_H_
