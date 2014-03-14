/* @@@LICENSE
*
*      Copyright (c) 2010-2014 LG Electronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* LICENSE@@@ */

#include "node_ls2_call.h"
#include "node_ls2_error_wrapper.h"
#include "node_ls2_handle.h"
#include "node_ls2_utils.h"

#include <cstring>
#include <iostream>
#include <lunaservice.h>
#include <stdexcept>
#include <cstring>

using namespace std;
using namespace v8;
using namespace node;

Persistent<FunctionTemplate> LS2Call::gCallTemplate;

static Persistent<String> response_symbol;

// Called during add-on initialization to add the "Call" template function
// to the target object.
void LS2Call::Initialize (Handle<Object> target)
{
    HandleScope scope;

    Local<FunctionTemplate> t = FunctionTemplate::New(New);

    t->SetClassName(String::New("palmbus/Call"));

    gCallTemplate = Persistent<FunctionTemplate>::New(t);

    t->InstanceTemplate()->SetInternalFieldCount(1);

    NODE_SET_PROTOTYPE_METHOD(t, "cancel", CancelWrapper);
    NODE_SET_PROTOTYPE_METHOD(t, "setResponseTimeout", SetResponseTimeoutWrapper);

    response_symbol = NODE_PSYMBOL("response");

    target->Set(String::NewSymbol("Call"), t->GetFunction());
}

// Used by LSHandle to create a "Call" object that wraps a particular
// LSHandle/LSMessageToken pair.
Local<Object> LS2Call::NewForCall()
{
    Local<Function> function = gCallTemplate->GetFunction();
    Local<Object> callObject = function->NewInstance();
    return callObject;
}

LS2Call::LS2Call()
    : fHandle(0)
    , fToken(LSMESSAGE_TOKEN_INVALID)
    , fResponseLimit(1)
    , fResponseCount(0)
{
    if (fHandle) {
        fHandle->CallCreated(this);
    }
}

void LS2Call::SetHandle(LS2Handle* handle)
{
    if (fHandle) {
        fHandle->CallCompleted(this);
    }
    fHandle = handle;
    if (fHandle) {
        fHandle->CallCreated(this);
    }
}


void LS2Call::Call(const char* busName, const char* payload, int responseLimit)
{
    RequireHandle();
    fResponseLimit = responseLimit;
    fToken = LSMESSAGE_TOKEN_INVALID;
    LSErrorWrapper err;
    bool result;
    void* userData((void*)this);
    if (responseLimit == 1) {
        result = LSCallOneReply(fHandle->Get(), busName, payload, &LS2Call::ResponseCallback, userData, &fToken, err);                
    } else {
        result = LSCall(fHandle->Get(), busName, payload, &LS2Call::ResponseCallback, userData, &fToken, err);        
    }
    if (!result) {
        err.ThrowError();
    }
    Ref();
}

// Called by V8 when the "Call" function is used with new.
Handle<Value> LS2Call::New(const Arguments& args)
{
    try {
	    LS2Call *m = new LS2Call();
	    m->Wrap(args.This());
	    return args.This();
    } catch( std::exception const & ex ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( ... ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
    }
}

Handle<Value> LS2Call::CancelWrapper(const Arguments& args)
{
    return VoidMemberFunctionWrapper<LS2Call>(&LS2Call::Cancel, args);
}

void LS2Call::Cancel()
{
    CancelInternal(fToken, true, false);
    fToken = LSMESSAGE_TOKEN_INVALID;
}

Handle<Value> LS2Call::SetResponseTimeoutWrapper(const Arguments& args)
{
	return VoidMemberFunctionWrapper<LS2Call, int>(&LS2Call::SetResponseTimeout, args);
}

void LS2Call::SetResponseTimeout(int timeout_ms)
{
	LSErrorWrapper err;
	if (!LSCallSetTimeout(fHandle->Get(), fToken, timeout_ms, err)) {
		err.ThrowError();
	}
}

LS2Call::~LS2Call()
{
#if TRACE_DESTRUCTORS
    cerr << "LS2Call::~LS2Call()" << endl;
#endif
    if (fHandle) {
        fHandle->CallCompleted(this);
        CancelInternal(fToken, false, false);
    }
}

bool LS2Call::ResponseCallback(LSHandle*, LSMessage *message, void *ctx)
{
    LS2Call* c = static_cast<LS2Call*>(ctx);
    return c->ResponseArrived(message);
}

bool LS2Call::ResponseArrived(LSMessage *message)
{
    HandleScope scope;

    fResponseCount+=1;
    EmitMessage(response_symbol, message);
    const char* category = LSMessageGetCategory(message);
    bool messageInErrorCategory = (category && strcmp(LUNABUS_ERROR_CATEGORY, category) == 0);    
    if (messageInErrorCategory || (fResponseLimit != kUnlimitedResponses && fResponseCount >= fResponseLimit)) {
        CancelInternal(fToken, false, messageInErrorCategory);
        fToken = LSMESSAGE_TOKEN_INVALID;
    }
    return true;
}

void LS2Call::CancelInternal(LSMessageToken token, bool shouldThrow, bool cancelDueToError)
{
    if (token == LSMESSAGE_TOKEN_INVALID) {
        return;
    }
    if (shouldThrow) {
        RequireHandle();        
    } else if (fHandle == 0 || !fHandle->IsValid()) {
        cerr << "Warning: Handle null on a no-throw call to CancelInternal.";
        return;
    }
    Unref();
    
    // If the message was from the bus, no reason to cancel
    if (cancelDueToError) {
        return;
    }
    
    // If the response limit is one we used LSCallOneReply to call. We are not required to call cancel
    // if we've already received the one response. Otherwise we need to call cancel to let
    // the ls2 library clean up.
    if (fResponseLimit != 1 || fResponseCount != 1) {
        LSErrorWrapper err;
        bool result = LSCallCancel(fHandle->Get(), token, err);
        if (!result) {
            if (shouldThrow) {
                err.ThrowError();            
            } else {
                cerr << "Warning: Cancel error during no-throw call to CancelInternal.";
                err.Print();
            }
        }
    }
}

void LS2Call::RequireHandle()
{
    if (fHandle == 0 || !fHandle->IsValid()) {
        throw runtime_error("Handle object is missing native handle.");
    }
}
