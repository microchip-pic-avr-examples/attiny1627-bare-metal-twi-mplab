#include "SLPCTRL.h"

#include <xc.h>

void SLPCTRL_init(void)
{
    SLPCTRL.CTRLA = SLPCTRL_SMODE_STDBY_gc | SLPCTRL_SEN_bm;
}