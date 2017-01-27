/*
 * This program kicks the watchdog less and less frequently. The idea is that eventually the
 * watchdog will fire and the app will be stopped.
 */

#include "legato.h"
#include "interfaces.h"

static le_timer_Ref_t KickTimer;
static uint32_t KickTimerPeriod = 0;

static void RestartKickTimer(void)
{
    KickTimerPeriod += 1000;
    LE_ASSERT_OK(le_timer_SetMsInterval(KickTimer, KickTimerPeriod));
    LE_ASSERT_OK(le_timer_Start(KickTimer));
}

static void KickTimerExpiryHandler(le_timer_Ref_t t)
{
    LE_INFO("Kick timer expired after %d ms", KickTimerPeriod);
    le_wdog_Kick();
    RestartKickTimer();
    LE_INFO("Kicked watchdog and restarted a %d ms timer.", KickTimerPeriod);
}


COMPONENT_INIT
{
    KickTimer = le_timer_Create("watchdog example");
    LE_ASSERT_OK(le_timer_SetHandler(KickTimer, KickTimerExpiryHandler));
    RestartKickTimer();
}
