#include <iostream>
#include "countDownTimer.h"
using namespace Napi;
using namespace std;

Napi::Object CreateTimer(const Napi::CallbackInfo &info)
{   
    cout << "test info after creation: " << info.Length() << "," << info[0].As<Napi::Number>().DoubleValue() << "," << info[1].As<Napi::Number>().DoubleValue() << endl;;
    return CountDownTimer::Create(info.Env(), info[0], info[1]);
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports)
{
    Napi::Object new_exports =
        Napi::Function::New(env, CreateTimer, "CreateTimer");
    return CountDownTimer::Init(env, new_exports);
}

NODE_API_MODULE(addon, InitAll)