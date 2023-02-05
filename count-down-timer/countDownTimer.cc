#include "countDownTimer.h"
#include <napi.h>
#include <iostream>
#include <thread>
using namespace Napi;
using namespace std;

// setInterval
std::thread thisThread;
ThreadSafeFunction tsfn;

Napi::Object CountDownTimer::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function classFunc = DefineClass(env, "CountDownTimer",
                                           {
                                               InstanceMethod("start", &CountDownTimer::start),
                                               InstanceMethod("stop", &CountDownTimer::stop),
                                               InstanceMethod("reset", &CountDownTimer::reset),
                                               InstanceMethod("addSecs", &CountDownTimer::addSecs),
                                               InstanceMethod("subSecs", &CountDownTimer::subSecs),
                                               InstanceMethod("isStarted", &CountDownTimer::isStarted),
                                               InstanceMethod("getMins", &CountDownTimer::getMins),
                                               InstanceMethod("getSecs", &CountDownTimer::getSecs),
                                               // InstanceMethod("getTimerVal", &CountDownTimer::getTimerVal),
                                           });

    Napi::FunctionReference *constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(classFunc);

    env.SetInstanceData(constructor);
    exports.Set("CountDownTimer", classFunc);

    return exports;
}

// initialize the class
CountDownTimer::CountDownTimer(const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<CountDownTimer>(info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 2)
    {
        Napi::TypeError::New(env, "Need to enter both mintues and seconds")
            .ThrowAsJavaScriptException();
    }

    if (!info[0].IsNumber() || !info[1].IsNumber())
    {
        Napi::TypeError::New(env, "Either of inputs is not a valid number")
            .ThrowAsJavaScriptException();
    }

    int32_t mins = info[0].As<Napi::Number>().Int32Value();
    int32_t secs = info[1].As<Napi::Number>().Int32Value();
    if (mins < 0 || mins > 60 || secs < 0 || secs > 60)
    {
        Napi::TypeError::New(env, "Input of minutes and seconds should be range from 0 to 60")
            .ThrowAsJavaScriptException();
    }

    cout << "create new object info.length: " << info.Length() << endl;
    cout << "test minutes and seconds data: " << mins << " " << secs << endl;
    this->minutes = this->remainingMins = mins;
    this->seconds = this->remainingSecs = secs;

    this->started = false;
}

Napi::Object CountDownTimer::Create(Napi::Env env, Napi::Value minutes, Napi::Value seconds)
{
    Napi::EscapableHandleScope scope(env);
    Napi::Object instance = env.GetInstanceData<Napi::FunctionReference>()->New({minutes, seconds});
    return scope.Escape(napi_value(instance)).ToObject();
}

Napi::Value CountDownTimer::start(const CallbackInfo &info)
{
    Napi::Env env = info.Env();
    this->started = true;
    auto cb = []()
    { cout << "1 sec\n"; };
    tsfn = ThreadSafeFunction::New(
        env,
        info[0].As<Function>(),
        "start timer",
        0,
        1,
        [](Napi::Env)
        {
            thisThread.join();
        });


    // auto globalInfo = info.This().As<Napi::Object>();
    // globalInfo.Get("subSecs").As<Napi::Function>().Call(info.This(), {});

    thisThread =
        std::thread([&]
                    {
                            //update remaining timers
                            auto callback = []( Napi::Env env, Function jsCallback, int value ) {
                                    jsCallback.Call( {Number::New( env, value )} );
                            };
                        while(this->started)
                        {
                            this->decrement(info);
                            cout << "1 sec elapsed;" << endl;
                            this_thread::sleep_for(chrono::seconds(1));
                        }

                        tsfn.Release(); });

    return Boolean::New(env, true);
}

Napi::Value CountDownTimer::stop(const CallbackInfo &info)
{
    Napi::Env env = info.Env();
    this->started = false;
    return Boolean::New(env, true);
}

Napi::Value CountDownTimer::reset(const CallbackInfo &info)
{
    Napi::Env env = info.Env();
    this->remainingMins = this->minutes;
    this->remainingSecs = this->seconds;
    this->started = false;
    return Napi::Boolean(env, false);
}

void CountDownTimer::calculate(const CallbackInfo &info)
{
    if (this->remainingSecs >= 60)
    {
        this->remainingSecs %= 60;
        this->remainingMins += 1;
    }
    else if (this->remainingSecs < 0)
    {
        if (this->remainingMins > 0)
        {
            this->remainingMins -= 1;
            this->remainingSecs = this->remainingSecs + 60;
        }
        else
        {
            this->remainingSecs = 0;
            this->started = false;
        }
    }
    cout << "after calc: " << this->remainingMins << ", " << this->remainingSecs << endl;
}

void CountDownTimer::addSecs(const CallbackInfo &info)
{
    int32_t num = info[0].As<Napi::Number>().Int32Value();
    this->remainingSecs += num;
    this->calculate(info);
}

void CountDownTimer::subSecs(const CallbackInfo &info)
{
    int32_t num = info[0].As<Napi::Number>().Int32Value();
    this->remainingSecs -= num;
    this->calculate(info);
}

void CountDownTimer::decrement(const CallbackInfo &info)
{
    this->remainingSecs -= 1;
    this->calculate(info);
}

Napi::Value CountDownTimer::isStarted(const CallbackInfo &info)
{
    Napi::Env env = info.Env();
    return Boolean::New(env, this->started);
}

// Napi::Object CountDownTimer::getTimerVal(const CallbackInfo &info)
// {
//     Napi::Env env = info.Env();
//     Napi::Object obj = Napi::Object::New(env);
//     obj.Set(Napi::String::New(env, "remainingMins"), Napi::Number::New( env, this->remainingMins));
//     obj.Set(Napi::String::New(env, "remainingSecs"), Napi::Number::New( env, this->remainingSecs));
//     return obj;
// }

Napi::Value CountDownTimer::getMins(const CallbackInfo &info)
{
    Napi::Env env = info.Env();
    return Napi::Number::New(env, this->remainingMins);
}

Napi::Value CountDownTimer::getSecs(const CallbackInfo &info)
{
    Napi::Env env = info.Env();
    return Napi::Number::New(env, this->remainingSecs);
}
