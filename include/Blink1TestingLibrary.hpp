/**
 * @file Blink1TestingLibrary.hpp
 * @brief Provides functions to control a simulated blink(1) device for testing
 */

#pragma once

#include <exception>
#include <string>
#include <vector>
#include <map>

#include "RGB.hpp"
#include "PlayState.hpp"
#include "PatternLineN.hpp"

#include "blink1-lib.h"

/// @cond
typedef struct hid_device_ {} hid_device_;
/// @endcond

/**
 * @brief Testing library namespace.
 *
 * This library simulates blink(1) devices for use in testing. It keeps track of
 * each device opened by the application, but for simplicity each one maps to the
 * same simulated device. For example, setting LED 1 to red on device 1 and then
 * reading the color of LED 1 on device 2 will return red, despite device 2 not
 * having the color set.
 *
 * Additional functions provided in this namespace allow for controlling the
 * simulated device in ways that are not normally possible.
 */
namespace fake_blink1_lib {
    /// @cond
    extern std::vector<blink1_device*> blink1_devices;
    extern std::map<long, blink1_lib::RGB> ledColors;
    extern std::map<long, uint16_t> ledFadeMillis;
    extern std::map<long, blink1_lib::PatternLineN> patternLines;
    extern int cacheIndex;
    extern std::string serial;
    extern bool isMk2;
    extern uint8_t patternLineLEDN;
    extern blink1_lib::PlayState playState;
    extern int blink1Version;
    extern bool successfulOperation;
    extern bool successfulInit;
    extern bool degammaEnabled;
    extern int vid;
    extern int pid;
    /// @endcond

    /**
     * Resets all features of the simulated blink(1) device to their default values.
     * LEDs are cleared and all devices are removed. This function should be in the
     * teardown for your test cases.
     */
    void CLEAR_ALL();

    /**
     * Indicates whether there are any blink(1) devices still connected to the application.
     * Whenever a blink1_lib::Blink1Device goes out of scope, the corresponding device
     * should be disconnected, so this can be used to determine whether any devices are still
     * in scope.
     *
     * @return `true` if all blink(1) devices have been properly closed and freed, `false` otherwise
     */
    bool ALL_DEVICES_FREED();

    /**
     * Sets the value that will be read by blink1_lib::Blink1Device::getVersion(). The default value is `0`.
     */
    void SET_BLINK1_VERSION(int version);

    /**
     * Sets whether operations that can fail should succeed or not. By default, all operations will fail -
     * functions returning optionals will have empty results, functions that return bools will return
     * `false`, etc.
     */
    void SET_BLINK1_SUCCESSFUL_OPERATION(bool op);

    /**
     * Sets whether attempting to open a new blink(1) device should succeed or not.
     * The default value is `false`.
     */
    void SET_BLINK1_SUCCESSFUL_INIT(bool init);

    /**
     * Sets the value that will be read by blink1_lib::Blink1Device::vid().
     * The default value is `0`.
     */
    void SET_BLINK1_VID(int _vid);

    /**
     * Sets the value that will be read by blink1_lib::Blink1Device::pid().
     * The default value is `0`.
     */
    void SET_BLINK1_PID(int _pid);

    /**
     * Sets the value that will be read by blink1_lib::Blink1Device::getCacheIndex().
     * The default value is `0`.
     */
    void SET_CACHE_INDEX(int index);

    /**
     * Sets the value that will be read by blink1_lib::Blink1Device::getSerial().
     * The default value is `""`.
     */
    void SET_SERIAL(std::string serial);

    /**
     * Sets the value that will be read by blink1_lib::Blink1Device::isMk2().
     * The default value is `false`.
     */
    void SET_IS_MK2(bool mk2);

    /**
     * For internal use.
     */
    bool SUCCESS(blink1_device* dev);

    /**
     * Returns the color of simulated LED `n`.
     */
    blink1_lib::RGB GET_RGB(long n);

    /**
     * Sets the color of simulated LED `n` to `rgb`.
     */
    void SET_RGB(blink1_lib::RGB rgb, long n);

    /**
     * Gets the `fadeMillis` value that was used in the last call to either
     * blink1_lib::Blink1Device::fadeToRGB or blink1_lib::Blink1Device::fadeToRGBN
     * for the LED at index `n`.
     */
    uint16_t GET_FADE_MILLIS(long n);

    /**
     * Sets the most recent `fadeMillis` value for LED `n`.
     */
    void SET_FADE_MILLIS(uint16_t fadeMillis, long n);

    /**
     * Reads the pattern line set by blink1_lib::Blink1Device::writePatternLineN at position `pos`.
     */
    blink1_lib::PatternLineN GET_PATTERN_LINE(long pos);

    /**
     * Sets the pattern line at position `pos` to `line`.
     */
    void SET_PATTERN_LINE(blink1_lib::PatternLineN line, long pos);

    /**
     * Gets the value of the play state, bypassing the need to have a device and read from it.
     */
    blink1_lib::PlayState GET_PLAY_STATE();

    /**
     * Sets the value that will be read by blink1_lib::Blink1Device::readPlayState().
     */
    void SET_PLAY_STATE(blink1_lib::PlayState state);
}
