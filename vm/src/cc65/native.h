//
// native.h
//

#ifndef NATIVE_H
#define NATIVE_H

#define NATIVE_CLASS_BASE           16

// java/lang/Object
#define NATIVE_CLASS_OBJECT         (NATIVE_CLASS_BASE+0)
#define NATIVE_METHOD_INIT          0

// java/lang/System
#define NATIVE_CLASS_SYSTEM         (NATIVE_CLASS_BASE+1)
#define NATIVE_FIELD_OUT            0
#define NATIVE_FIELD_IN             1

// java/io/PrintStream
#define NATIVE_CLASS_PRINTSTREAM    (NATIVE_CLASS_BASE+2)
#define NATIVE_METHOD_PRINTLN_STR   1
#define NATIVE_METHOD_PRINTLN_INT   2
#define NATIVE_METHOD_PRINTLN_CHAR  3
#define NATIVE_METHOD_PRINT_STR     4
#define NATIVE_METHOD_PRINT_INT     5
#define NATIVE_METHOD_PRINT_CHAR    6
#define NATIVE_METHOD_FORMAT        7

// java/io/InputStream
#define NATIVE_CLASS_INPUTSTREAM    (NATIVE_CLASS_BASE+3)
#define NATIVE_METHOD_INPUTSTREAM_AVAILABLE 1
#define NATIVE_METHOD_INPUTSTREAM_READ      2

// java/lang/StringBuffer
#define NATIVE_CLASS_STRINGBUFFER   (NATIVE_CLASS_BASE+4)
#define NATIVE_METHOD_INIT_STR      1
#define NATIVE_METHOD_APPEND_STR    2
#define NATIVE_METHOD_APPEND_INT    3
#define NATIVE_METHOD_APPEND_CHR    4
#define NATIVE_METHOD_TOSTRING      5
#define NATIVE_METHOD_APPEND_FLOAT  6

#define NATIVE_ID(c,m)  ((c<<8)|m)

#define NATIVE_ID2CLASS(m)   (m>>8)
#define NATIVE_ID2METHOD(m)  (m&0xff)
#define NATIVE_ID2FIELD(m)   (m&0xff)

#endif // NATIVE_H
