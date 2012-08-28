/* @@@LICENSE
*
*      Copyright (c) 2010-2012 Hewlett-Packard Development Company, L.P.
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

#include "node_ls2.h"
#include "node_ls2_error_wrapper.h"
#include "node_ls2_handle.h"
#include "node_ls2_message.h"
#include "node_ls2_call.h"
#include "node_ls2_utils.h"

#include <syslog.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstring>

using namespace std;
using namespace v8;
using namespace node;

static Persistent<String> cancel_symbol;
static Persistent<String> request_symbol;

// Converter for LS2Message objects that converts a wrapped object to its native
// object. See node_ls2_utils.h for a description of how ConvertFromJS works.

template <> struct ConvertFromJS<LS2Message*> {

    explicit ConvertFromJS(const v8::Handle<v8::Value>& value) : fMessage(0) {
        Handle<Object> o = Handle<Object>::Cast(value);
        fMessage = node::ObjectWrap::Unwrap<LS2Message>(o);
        if (!fMessage) {
            throw runtime_error("Unable to unwrap native object.");
        }
    }

    operator LS2Message*() {
        return fMessage;
    }

    LS2Message* fMessage;
};


// Called during add-on initialization to add the "Handle" template function
// to the target object.
void LS2Handle::Initialize(Handle<Object> target)
{
    HandleScope scope;

    Local<FunctionTemplate> t = FunctionTemplate::New(New);
    
    t->SetClassName(String::New("palmbus/Handle"));

    t->Inherit(EventEmitter::constructor_template);
    t->InstanceTemplate()->SetInternalFieldCount(1);

    NODE_SET_PROTOTYPE_METHOD(t, "call", CallWrapper);
    NODE_SET_PROTOTYPE_METHOD(t, "watch", WatchWrapper);
    NODE_SET_PROTOTYPE_METHOD(t, "subscribe", SubscribeWrapper);
    NODE_SET_PROTOTYPE_METHOD(t, "registerMethod", RegisterMethodWrapper);
    NODE_SET_PROTOTYPE_METHOD(t, "subscriptionAdd", SubscriptionAddWrapper);
    NODE_SET_PROTOTYPE_METHOD(t, "cancel", CancelWrapper);
    NODE_SET_PROTOTYPE_METHOD(t, "pushRole", PushRoleWrapper);
    NODE_SET_PROTOTYPE_METHOD(t, "unregister", UnregisterWrapper);

    cancel_symbol = NODE_PSYMBOL("cancel");
    request_symbol = NODE_PSYMBOL("request");

    target->Set(String::NewSymbol("Handle"), t->GetFunction());
}

void LS2Handle::CallCreated(LS2Call*)
{
    Ref();
}

void LS2Handle::CallCompleted(LS2Call*)
{
    Unref();
}

LSHandle* LS2Handle::Get()
{
    RequireHandle();
    return fHandle;
}

// Called by V8 when the "Handle" function is used with new.
Handle<Value> LS2Handle::New(const Arguments& args)
{
    try {
        HandleScope scope;
        string busName;
        const char* busNamePtr = NULL;
        bool publicBus = false;

        if (args.Length() > 1) {
            publicBus = args[1]->BooleanValue();
        }

        if (args.Length() > 0) {
            if (!args[0]->IsNull()) {
                String::Utf8Value busNameValue(args[0]);
                busName = string(*busNameValue);
                busNamePtr = busName.c_str();
            }
        }

        LS2Handle *handle = new LS2Handle(busNamePtr, publicBus);
        handle->Wrap(args.This());

        return args.This();
    } catch( std::exception const & ex ) {
        v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
		return Local<Value>::New(Undefined());
    } catch( ... ) {
        v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
		return Local<Value>::New(Undefined());
    }
}

LS2Handle::LS2Handle(const char* namePtr, bool publicBus)
    : fHandle(0)
{
    LSErrorWrapper err;
    if (!LSRegisterPubPriv(namePtr, &fHandle, publicBus, err)) {
        err.ThrowError();
    }
   
	Attach(GetMainLoop());

    if (!LSSubscriptionSetCancelFunction(fHandle, LS2Handle::CancelCallback, static_cast<void*>(this), err)) {
        err.ThrowError();
    }
}

LS2Handle::~LS2Handle()
{
#if TRACE_DESTRUCTORS
    cerr << "LS2Handle::~LS2Handle()" << endl;
#endif
	// This should never happen, since the destructor won't get called when we still have registered methods
	if (fRegisteredMethods.size() > 0) {
		cerr << "LS2Handle::~LS2Handle() called with registered methods active" << endl;
		this->Unregister();
	}
}

Handle<Value> LS2Handle::CallWrapper(const Arguments& args)
{
    return MemberFunctionWrapper<LS2Handle, Handle<Value>, const char*, const char*>(&LS2Handle::Call, args);
}

Handle<Value> LS2Handle::Call(const char* busName, const char* payload)
{
    return CallInternal(busName, payload, 1);
}

Handle<Value> LS2Handle::WatchWrapper(const Arguments& args)
{
    return MemberFunctionWrapper<LS2Handle, Handle<Value>, const char*, const char*>(&LS2Handle::Watch, args);
}

Handle<Value> LS2Handle::Watch(const char* busName, const char* payload)
{
    return CallInternal(busName, payload, 2);
}

Handle<Value> LS2Handle::SubscribeWrapper(const Arguments& args)
{
    return MemberFunctionWrapper<LS2Handle, Handle<Value>, const char*, const char*>(&LS2Handle::Subscribe, args);
}

Handle<Value> LS2Handle::Subscribe(const char* busName, const char* payload)
{
    return CallInternal(busName, payload, LS2Call::kUnlimitedResponses);
}

Handle<Value> LS2Handle::CancelWrapper(const Arguments& args)
{
    return MemberFunctionWrapper<LS2Handle, bool, LSMessageToken>(&LS2Handle::Cancel, args);
}

bool LS2Handle::Cancel(LSMessageToken t)
{
    RequireHandle();
    LSErrorWrapper err;
    if(!LSCallCancel(fHandle, t, err)) {
        err.ThrowError();
    }
    return true;
}

Handle<Value> LS2Handle::PushRoleWrapper(const Arguments& args)
{
    return VoidMemberFunctionWrapper<LS2Handle, const char*>(&LS2Handle::PushRole, args);
}

void LS2Handle::PushRole(const char* pathToRoleFile)
{
    RequireHandle();
    LSErrorWrapper err;
    if(!LSPushRole(fHandle, pathToRoleFile, err)) {
        err.ThrowError();
    }
}

Handle<Value> LS2Handle::RegisterMethodWrapper(const Arguments& args)
{
    return VoidMemberFunctionWrapper<LS2Handle, const char*, const char*>(&LS2Handle::RegisterMethod, args);
}

void LS2Handle::RegisterMethod(const char* category, const char* methodName)
{
    RequireHandle();
    if (fRegisteredMethods.size() == 0) {
        // Establish a self-reference on the first method registration so that this object
        // won't get collected.
        Ref();
    }
    RegisteredMethod* m = new RegisteredMethod(methodName);
    fRegisteredMethods.push_back(m);
    RegisterCategory(category, m->GetMethods());
}

Handle<Value> LS2Handle::UnregisterWrapper(const Arguments& args)
{
    return VoidMemberFunctionWrapper<LS2Handle>(&LS2Handle::Unregister, args);
}

void LS2Handle::Unregister()
{
    //cerr << "LS2Handle::Unregister()" << endl;
    if (fHandle) {
        LSErrorWrapper err;
        if(!LSUnregister(fHandle, err)) {
            // Cannot call err.ThrowError() here, since throwing from a destructor is a no-no
            std::cerr << "LSUnregister failed during Unregister()." << std::endl;
            err.Print();
        }
	fHandle = 0;
    }
    if (fRegisteredMethods.size() > 0) {
        // Undo the Ref() operation from the first registerMethod() - this object is now eligible for collection
        Unref();
    }
    // I wish I had boost and could use shared_ptr here.
    for(MethodVector::const_iterator i = fRegisteredMethods.begin(); i != fRegisteredMethods.end(); ++i) {
        RegisteredMethod* m = *i;
        delete m;
    }

    // Since we've deleted all of the items above, make sure we
    // clean up the vector as well (make size == 0) so that if this gets
    // called again for some reason we won't attempt to double-free
    fRegisteredMethods.clear();
}

Handle<Value> LS2Handle::SubscriptionAddWrapper(const Arguments& args)
{
    return VoidMemberFunctionWrapper<LS2Handle, const char*, LS2Message*>(&LS2Handle::SubscriptionAdd, args);
}

void LS2Handle::SubscriptionAdd(const char* key, LS2Message* msg)
{
    RequireHandle();
    LSErrorWrapper err;
    if(!LSSubscriptionAdd(fHandle, key, msg->Get(), err)) {
        err.ThrowError();
    }
}

Handle<Value> LS2Handle::CallInternal(const char* busName, const char* payload, int responseLimit)
{
    RequireHandle();
    Local<Object> callObject = LS2Call::NewForCall();
    LS2Call *call = node::ObjectWrap::Unwrap<LS2Call>(callObject);
    if (!call) {
        return v8::ThrowException(v8::String::New("Unable to unwrap native object."));
    }
    call->SetHandle(this);
    call->Call(busName, payload, responseLimit);
    return callObject;
}

void LS2Handle::Attach(GMainLoop *mainLoop)
{
    LSErrorWrapper err;
    if(!LSGmainAttach(fHandle, mainLoop, err)) {
        err.ThrowError();
    }
}

bool LS2Handle::RegisterCategory(const char* categoryName, LSMethod *methods)
{
    LSErrorWrapper err;
    const char* catName = "/";
    if (strlen(categoryName) > 0) {
        catName = categoryName;
    }

    if(!LSRegisterCategoryAppend(fHandle, catName, methods, 0, err)) {
        err.ThrowError();
    }

    if(!LSCategorySetData(fHandle, catName, static_cast<void*>(this), err)) {
        err.ThrowError();
    }

    return true;
}

bool LS2Handle::CancelCallback(LSHandle *sh, LSMessage *message, void *ctx)
{
    LS2Handle* h = static_cast<LS2Handle*>(ctx);
    return h->CancelArrived(message);
}

bool LS2Handle::CancelArrived(LSMessage *message)
{
    HandleScope scope;
    //UnrefIfPending(LSMessageGetResponseToken(message));
    EmitMessage(cancel_symbol, message);
    return true;
}

bool LS2Handle::RequestCallback(LSHandle *sh, LSMessage *message, void *ctx)
{
    LS2Handle* h = static_cast<LS2Handle*>(ctx);
    return h->RequestArrived(message);
}

bool LS2Handle::RequestArrived(LSMessage *message)
{
    HandleScope scope;
    EmitMessage(request_symbol, message);
    return true;
}

void LS2Handle::EmitMessage(const Handle<String>& symbol, LSMessage *message)
{
    Local<Value> messageObject = LS2Message::NewFromMessage(message);
        
    // messageObject will be empty if a v8 exception is thrown in
    // LS2Message::NewFromMessage
    if (!messageObject.IsEmpty()) {
        Emit(symbol, 1, &messageObject);
    } else {
        // We don't want to silently lose messages
        syslog(LOG_USER | LOG_CRIT, "%s: messageObject is empty", __PRETTY_FUNCTION__);
        abort();
    }
}

void LS2Handle::RequireHandle()
{
    if (fHandle == 0) {
        throw runtime_error("Handle object is missing native handle.");
    }
}

LS2Handle::RegisteredMethod::RegisteredMethod(const char* name)
{
    std::memset(&fMethods, 0, sizeof(fMethods));
    copy(name, name + strlen(name), back_inserter(fName));
    fName.push_back(0);
    fMethods[0].name = &fName[0];
    fMethods[0].function = &LS2Handle::RequestCallback;
}

LSMethod* LS2Handle::RegisteredMethod::GetMethods()
{
    return fMethods;
}
