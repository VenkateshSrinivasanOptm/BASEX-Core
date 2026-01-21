#ifndef APPMANMSGIF_H
#define APPMANMSGIF_H

typedef enum
{
    EXIT_NO_ERROR=0,
    MW_UI_LEVEL_ERR=1,
    STB_LEVEL_ERR=100
}ErrorLevel;

typedef enum
{
    POWER_RESET_STATE_UNKNOWN, // Unable to determine the state
    POWER_ON,           // Power notification that the box has been switched on.
    POWER_OFF,          // Power notification that the box has been switched off. termination required
    POWER_DEEP_SLEEP,   // Power notification that the box has been set to deep sleep. termination required
    POWER_STANDBY,      // power notification that we are going to standby, no restart
    RESTART,            // Notification that you need to restart due to new software or error. termination required.
    RESTART_OTHER,      // notification that other MW modules are having to restart, no restart
    RESTARTED_MW_ACTIVE // notification that the MW has completed a restart and all components are online.
} StateChange;

typedef enum
{
    RESTART_UNDEFINED,  // Unable to determine the restart state.
    RESTART_UI_ONLY,
    RESTART_MW_AND_UI,
    RESTART_MW_ONLY,
    RESTART_STB
} RestartLevel;

typedef enum
{
    UI_NOTIFY_POWER_ON,
    UI_NOTIFY_POWER_OFF,
    UI_NOTIFY_POWER_DEEP_SLEEP,
    UI_NOTIFY_POWER_STANDBY
} UiPowerNotification;

#endif // APPMANMSGIF_H
