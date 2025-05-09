/*********************
 * extui_example.cpp *
 *********************/

/****************************************************************************
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "../../inc/MarlinConfigPre.h"

#if BOTH(MD_FSMC_LCD, EXTENSIBLE_UI)

#include "ui_api.h"
#include "lib/tsc/TSC_Menu.h"

// To implement a new UI, complete the functions below and
// read or update Marlin's state using the methods in the
// ExtUI methods in "../ui_api.h"
//
// Although it may be possible to access other state
// variables from Marlin, using the API here possibly
// helps ensure future compatibility.


namespace ExtUI {
  void onStartup() {
    /* Initialize the display module here. The following
     * routines are available for access to the GPIO pins:
     *
     *   SET_OUTPUT(pin)
     *   SET_INPUT_PULLUP(pin)
     *   SET_INPUT(pin)
     *   WRITE(pin,value)
     *   READ(pin)
     */
    // LCD_Setup(); // move to setup() for fast startup speed
  }
  void onIdle() {
    LCD_init_Reg();
    menuUpdate();
  }

  void onPrinterKilled(PGM_P const error, PGM_P const component) {
    char context[126];
    //GUI_Clear(BK_COLOR);
    sprintf_P(context, "%s %s %s: %s", GET_TEXT(MSG_HALTED), GET_TEXT(MSG_PLEASE_RESET), error, component);
    popupDrawPage(NULL, textSelect(LABEL_ERROR), (uint8_t *)context, NULL, NULL);
  }
  
  void onMediaInserted() {
    #ifdef USEOLDSTATUSSCREEN
    volumeReminderMessage(LABEL_TFTSD_INSERTED, STATUS_NORMAL);
    #endif
  }
  void onMediaError() {
    #ifdef USEOLDSTATUSSCREEN
    volumeReminderMessage(LABEL_READ_TFTSD_ERROR, STATUS_NORMAL);
    #endif
  }
  void onMediaRemoved() {
    #ifdef USEOLDSTATUSSCREEN
    volumeReminderMessage(LABEL_TFTSD_REMOVED, STATUS_NORMAL);
    #endif
  }
  void onUsbInserted() {
    #ifdef USEOLDSTATUSSCREEN
    volumeReminderMessage(LABEL_U_DISK_INSERTED, STATUS_NORMAL);
    #endif
  }
  void onUsbError() {
    #ifdef USEOLDSTATUSSCREEN
    volumeReminderMessage(LABEL_READ_U_DISK_ERROR, STATUS_NORMAL);
    #endif
  }
  void onUsbRemoved() {
    #ifdef USEOLDSTATUSSCREEN
    volumeReminderMessage(LABEL_U_DISK_REMOVED, STATUS_NORMAL);
    #endif
  }

  void onPlayTone(const uint16_t frequency, const uint16_t duration) {
    CRITICAL_SECTION_START();
    ::tone(BEEPER_PIN, frequency, duration);
    CRITICAL_SECTION_END();
  }
  void onPrintTimerStarted() {}
  void onPrintTimerPaused() {}
  void onPrintTimerStopped() {}
  void onFilamentRunout(const extruder_t extruder) {
    popupReminder_p(textSelect(LABEL_WARNING), textSelect(LABEL_FILAMENT_RUNOUT));
    Buzzer_play(sound_error);
  }
  void onUserConfirmRequired(const char * const msg) {
    popupDrawPage(&bottomSingleBtn, textSelect(LABEL_CONFIRM), (uint8_t *)msg, textSelect(LABEL_CONFIRM), 0);
    infoMenu.menu[++infoMenu.cur] = menuUsrConfirm;
    Buzzer_play(sound_notify);
  }

  void onStatusChanged(const char * const msg) {
    statusScreen_setMsg(0, (uint8_t *)msg);
  }
  void onFactoryReset() {}

  void onStoreSettings(char *buff) {
    // This is called when saving to EEPROM (i.e. M500). If the ExtUI needs
    // permanent data to be stored, it can write up to eeprom_data_size bytes
    // into buff.

    // Example:
    //  static_assert(sizeof(myDataStruct) <= ExtUI::eeprom_data_size);
    //  memcpy(buff, &myDataStruct, sizeof(myDataStruct));
  }

  void onLoadSettings(const char *buff) {
    // This is called while loading settings from EEPROM. If the ExtUI
    // needs to retrieve data, it should copy up to eeprom_data_size bytes
    // from buff

    // Example:
    //  static_assert(sizeof(myDataStruct) <= ExtUI::eeprom_data_size);
    //  memcpy(&myDataStruct, buff, sizeof(myDataStruct));
  }

  void onConfigurationStoreWritten(bool success) {
    // This is called after the entire EEPROM has been written,
    // whether successful or not.
    //popupReminder(textSelect(LABEL_TIPS), textSelect(success ? LABEL_EEPROM_SAVE_SUCCESS : LABEL_EEPROM_SAVE_FAILED));
  }

  void onConfigurationStoreRead(bool success) {
    // This is called after the entire EEPROM has been read,
    // whether successful or not.
  }

  void onMeshUpdate(const int8_t xpos, const int8_t ypos, const float zval) {
    // This is called when any mesh points are updated
  }

  void onRecoveryChecked() {
    infoMenu.menu[++infoMenu.cur] = menuPowerFailed;
  }

  #if ENABLED(POWER_LOSS_RECOVERY)
    void onPowerLossResume() {
      // Called on resume from power-loss
      infoMenu.menu[++infoMenu.cur] = menuPowerFailed;
    }
  #endif

  #if HAS_PID_HEATING
    void onPidTuning(const result_t rst) {
      // Called for temperature PID tuning result
    }
  #endif
}

#endif // EXTUI_EXAMPLE && EXTENSIBLE_UI
