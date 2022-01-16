#include "ButtonGesture.h"


ButtonGesture::ButtonGesture(DigitalIn& din, int clickTimeMs, int holdRepeatMs, int debounceTimeMs) :
    din_(din), clickTimeMs_(clickTimeMs), holdRepeatMs_(holdRepeatMs), debounceTimeMs_(debounceTimeMs) {
  debounceTimer_.start();
  pressTimer_.start();
}

ButtonGesture::Gesture ButtonGesture::update() {
  bool buttonState = din_;
  bool newDebounceState = debouncedState_;
  if (buttonState != debouncedState_) {
    if (debounceTimer_.read_ms() > debounceTimeMs_) {
      newDebounceState = buttonState;
    }
  } else {
    debounceTimer_.reset();
  }

  Gesture result;
  if (newDebounceState != debouncedState_) {  // edge
    if (!newDebounceState) {  // down edge
      pressTimer_.reset();
      result = Gesture::kClickPress;
    } else {  // up edge
      if (isLongPress_) {
        result = Gesture::kHoldRelease;
      } else {
        result = Gesture::kClickRelease;
      }
      isLongPress_ = false;
    }
  } else {  // holding
    if (!newDebounceState) {  // held down
      if (!isLongPress_ && pressTimer_.read_ms() > clickTimeMs_) {  // new long press
        isLongPress_ = true;
        pressTimer_.reset();
        result = Gesture::kHoldTransition;
      } else if (isLongPress_) {  // holding previous long press
        if (pressTimer_.read_ms() > holdRepeatMs_) {
          pressTimer_.reset();
          result = Gesture::kHoldRepeat;
        } else {
          result = Gesture::kHold;
        }
      } else {  // not long press
        result = Gesture::kDown;
      }
    } else {  // released up
      result = Gesture::kUp;
    }
  }
  debouncedState_ = newDebounceState;
  return result;
}
