#include "piobus.h"
#include "target.h"

#define FOO_PIOBUS PIOBUS_DEFINE(PORT_A, 16, 31)

int foo(void)
{
    return piobus_input_get (FOO_PIOBUS);
}


