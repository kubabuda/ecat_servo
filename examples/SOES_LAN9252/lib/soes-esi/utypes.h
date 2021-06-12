#ifndef __UTYPES_H__
#define __UTYPES_H__

#include "cc.h"

/* Object dictionary storage */

typedef struct
{
   /* Identity */

   uint32_t serial;

   /* Inputs */

   int32_t CounterOut;
   uint16_t Status_Word;

   /* Outputs */

   uint16_t Control_Word;
   int32_t LedIn;

} _Objects;

extern _Objects Obj;

#endif /* __UTYPES_H__ */
