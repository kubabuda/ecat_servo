#ifndef __UTYPES_H__
#define __UTYPES_H__

#include "cc.h"

/* Object dictionary storage */

typedef struct
{
   /* Identity */

   uint32_t serial;

   /* Inputs */

   uint8_t Key1;
   uint8_t Key2;
   uint32_t Counter;

   /* Outputs */

   uint8_t LedIn;

} _Objects;

extern _Objects Obj;

#endif /* __UTYPES_H__ */
