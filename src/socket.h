/* Copyright (c) 2017-2018 Rolf Timmermans */
#pragma once

#include "binding.h"
#include "inline/poller.h"

namespace zmq {
class Socket : public Napi::ObjectWrap<Socket> {
public:
    static Napi::FunctionReference Constructor;
    static void Initialize(Napi::Env& env, Napi::Object& exports);

    Socket(const Napi::CallbackInfo& info);
    ~Socket();

protected:
    enum class State : uint8_t {
        Open, /* Socket is open. */
        Closed, /* Socket is closed. */
        Blocked, /* Async operation in progress that disallows socket access. */
    };

    inline void Close(const Napi::CallbackInfo& info);

    inline Napi::Value Bind(const Napi::CallbackInfo& info);
    inline Napi::Value Unbind(const Napi::CallbackInfo& info);

    inline void Connect(const Napi::CallbackInfo& info);
    inline void Disconnect(const Napi::CallbackInfo& info);

    inline Napi::Value Send(const Napi::CallbackInfo& info);
    inline Napi::Value Receive(const Napi::CallbackInfo& info);

    template <typename T>
    inline Napi::Value GetSockOpt(const Napi::CallbackInfo& info);

    template <typename T>
    inline void SetSockOpt(const Napi::CallbackInfo& info);

    inline Napi::Value GetEvents(const Napi::CallbackInfo& info);
    inline Napi::Value GetContext(const Napi::CallbackInfo& info);

    inline Napi::Value GetClosed(const Napi::CallbackInfo& info);
    inline Napi::Value GetReadable(const Napi::CallbackInfo& info);
    inline Napi::Value GetWritable(const Napi::CallbackInfo& info);

private:
    inline void WarnUnlessImmediateOption(int32_t option) const;
    inline bool ValidateOpen() const;
    inline bool HasEvents(int32_t events);

    void Close();

    /* Send/receive are usually in a hot path and will benefit slightly
       from being inlined. They are used in more than one location and are
       not necessarily automatically inlined by all compilers. */
    force_inline void Send(const Napi::Promise::Deferred& res, const Napi::Array& msg);
    force_inline void Receive(const Napi::Promise::Deferred& res);

    Napi::ObjectReference context_ref;
    Napi::ObjectReference observer_ref;
    Poller poller;
    void* socket = nullptr;

    int64_t send_timeout = -1;
    int64_t receive_timeout = -1;
    uint32_t endpoints = 0;

    State state = State::Open;
    bool request_close = false;

    friend class Observer;
    friend class Proxy;
};
}
