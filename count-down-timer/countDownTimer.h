#ifndef COUNTDOWNTIMER_H
#define COUNTDOWNTIMER_H

#include <napi.h>
#include <thread>

class CountDownTimer : public Napi::ObjectWrap<CountDownTimer>
{
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::Object Create(Napi::Env env, Napi::Value minutes, Napi::Value seconds);
  CountDownTimer(const Napi::CallbackInfo &info);
  Napi::Value start(const Napi::CallbackInfo &info);
  Napi::Value stop(const Napi::CallbackInfo &info);
  Napi::Value reset(const Napi::CallbackInfo &info);
  void addSecs(const Napi::CallbackInfo &info);
  void subSecs(const Napi::CallbackInfo &info);
  void decrement(const Napi::CallbackInfo &info);
  Napi::Object getTimerVal(const Napi::CallbackInfo &info);
  Napi::Value getMins(const Napi::CallbackInfo &info);
  Napi::Value getSecs(const Napi::CallbackInfo &info);

private:
  void calculate(const Napi::CallbackInfo &info);
  Napi::Value isStarted(const Napi::CallbackInfo &info);
  bool started;

protected:
  int32_t minutes, seconds;
  int32_t remainingMins, remainingSecs;
  std::thread currThread;
};

#endif