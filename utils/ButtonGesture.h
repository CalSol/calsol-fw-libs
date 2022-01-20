#include <mbed.h>

#ifndef __BUTTON_GESTURE_H__
#define __BUTTON_GESTURE_H__

/**
 * Button gesture detector (click / click-and-hold / long press) with debouncing.
 */
class ButtonGesture {
public:
  enum Gesture {
    kUp,  // repeatedly emitted when not pressed (up)
    kClickPress,  // emitted on press edge (up to down)
    kDown,  // repeatedly emitted when pressed, but not held long enough to be a long press
    kClickRelease,  // emitted on release edge (down to up), when not held long enough to be a long press
    kHoldTransition,  // emitted once when held long enough to be a long press
    kHold,  // repeatedly emitted when held long enough to be a long press
    kHoldRepeat,  // emitted periodically (every holdRepeatMs) when held long enough to e a long press
    kHoldRelease,  // emitted on release edge (down to up), when held long enough to be a long press
  };

  ButtonGesture(DigitalIn& din, int clickTimeMs=700, int holdRepeatMs=100, int debounceTimeMs=50);
  Gesture update();

protected:
  DigitalIn& din_;
  
  int clickTimeMs_;  // duration boundary between click and click-and-hold
  int holdRepeatMs_;  // duration between generating hold repeat events
  int debounceTimeMs_;  // duration to debounce edge - must be stable for this long to register change

  Timer debounceTimer_;
  Timer pressTimer_;

  bool debouncedState_ = true;  // inverted logic, true is up
  bool isLongPress_ = false;
};

#endif  // __BUTTON_GESTURE_H__
