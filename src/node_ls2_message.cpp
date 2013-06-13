/* @@@LICENSE
*
*      Copyright (c) 2010-2013 LG Electronics, Inc.
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

#include "node_ls2_message.h"
#include "node_ls2_error_wrapper.h"
#include "node_ls2_utils.h"

#include <syslog.h>
#include <stdlib.h>

using namespace std;
using namespace v8;
using namespace node;

// Converter for LSMessage tokens. See node_ls2_utils.h for a description 
// of how the conversion system works.
template <> v8::Handle<v8::Value> ConvertToJS<LSMessageToken>(LSMessageToken v)
{
    return v8::Integer::NewFromUnsigned(v);
}

// Need to hold on to a reference to the function template for use by
// NewFromMessage.
Persistent<FunctionTemplate> LS2Message::gMessageTemplate;

// Called during add-on initialization to add the "Message" template function
// to the target object.
void LS2Message::Initialize (Handle<Object> target)
{
    HandleScope scope;

    Local<FunctionTemplate> t = FunctionTemplate::New(New);

    t->SetClassName(String::New("palmbus/Message"));

    gMessageTemplate = Persistent<FunctionTemplate>::New(t);

    t->InstanceTemplate()->SetInternalFieldCount(1);

    NODE_SET_PROTOTYPE_METHOD(t, "payload", PayloadWrapper);
    NODE_SET_PROTOTYPE_METHOD(t, "responseToken", ResponseTokenWrapper);
    NODE_SET_PROTOTYPE_METHOD(t, "print", PrintWrapper);
    NODE_SET_PROTOTYPE_METHOD(t, "method", MethodWrapper);
    NODE_SET_PROTOTYPE_METHOD(t, "applicationID", ApplicationIDWrapper);
    NODE_SET_PROTOTYPE_METHOD(t, "sender", SenderWrapper);
    NODE_SET_PROTOTYPE_METHOD(t, "senderServiceName", SenderServiceNameWrapper);
    NODE_SET_PROTOTYPE_METHOD(t, "uniqueToken", UniqueTokenWrapper);
    NODE_SET_PROTOTYPE_METHOD(t, "kind", KindWrapper);
    NODE_SET_PROTOTYPE_METHOD(t, "category", CategoryWrapper);
    NODE_SET_PROTOTYPE_METHOD(t, "token", TokenWrapper);
    NODE_SET_PROTOTYPE_METHOD(t, "isSubscription", IsSubscriptionWrapper);
    NODE_SET_PROTOTYPE_METHOD(t, "respond", RespondWrapper);

    target->Set(String::NewSymbol("Message"), t->GetFunction());
}

// Used by LSHandle to create a "Message" object that wraps a particular
// LSMessage structure.
Local<Value> LS2Message::NewFromMessage(LSMessage* message)
{
    TryCatch try_catch;

    Local<Function> function = gMessageTemplate->GetFunction();
    Local<Object> messageObject = function->NewInstance();

    // If we get an exception in LS2Message::New, then it will return
    // v8::ThrowException which adds a pending exception.
    // Function::NewInstance checks to see if the callee set an exception.
    // If it did, it returns an empty handle 
    if (!messageObject.IsEmpty()) {
        LS2Message *m = node::ObjectWrap::Unwrap<LS2Message>(messageObject);
        if (!m) {
            return Local<Value>::New(v8::ThrowException(v8::String::New("Unable to unwrap native object.")));
        }
        m->SetMessage(message);
    } else {
        // We got an exception; If we try to continue we're going to lose
        // a message, so just crash
        v8::String::Utf8Value exception(try_catch.Exception());
        syslog(LOG_USER | LOG_CRIT, "%s: exception: %s; aborting", __PRETTY_FUNCTION__, 
                                    *exception ? *exception : "no exception string");
        abort();
    }
    return messageObject;
}

LSMessage* LS2Message::Get() const
{
    RequireMessage();
    return fMessage;
}

// This function exists because it is a requirement of template functions. 
// The object created with it will not be very useful, since the LS2Message
// object doesn't correspond to any lower level LSMessage.
Handle<Value> LS2Message::New(const Arguments& args)
{
    try {
        LS2Message *m = new LS2Message();
        m->Wrap(args.This());
        return args.This();
    } catch (std::bad_alloc const & ex) {
        syslog(LOG_USER | LOG_CRIT, "%s: throwing memory allocation exception: %s", __func__, ex.what());
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( std::exception const & ex ) {
        syslog(LOG_USER | LOG_CRIT, "%s: throwing standard exception: %s", __func__, ex.what());
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( ... ) {
        syslog(LOG_USER | LOG_CRIT, "%s: throwing other exception", __func__);
        return v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
    }
}


LS2Message::LS2Message(LSMessage* m)
    : fMessage(m)
{
    if(fMessage) {
        LSMessageRef(fMessage);
    }
}

LS2Message::~LS2Message()
{
#if TRACE_DESTRUCTORS
    cerr << "LS2Message::~LS2Message()" << endl;
#endif
    if(fMessage) {
        LSMessageUnref(fMessage);
    }
}

void LS2Message::SetMessage(LSMessage* m)
{
    if(fMessage) {
        LSMessageUnref(fMessage);
    }
    fMessage = m;
    if(fMessage) {
        LSMessageRef(fMessage);
    }
}

v8::Handle<v8::Value> LS2Message::ApplicationIDWrapper(const v8::Arguments& args)
{
    return MemberFunctionWrapper<LS2Message, const char*>(&LS2Message::ApplicationID, args);
}

const char* LS2Message::ApplicationID() const
{
    return GetString(LSMessageGetApplicationID);
}

v8::Handle<v8::Value> LS2Message::CategoryWrapper(const v8::Arguments& args)
{
    return MemberFunctionWrapper<LS2Message, const char*>(&LS2Message::Category, args);
}

const char* LS2Message::Category() const
{
    return GetString(LSMessageGetCategory);
}

v8::Handle<v8::Value> LS2Message::IsSubscriptionWrapper(const v8::Arguments& args)
{
    return MemberFunctionWrapper<LS2Message, bool>(&LS2Message::IsSubscription, args);
}

bool LS2Message::IsSubscription() const
{
    RequireMessage();
    return LSMessageIsSubscription(fMessage);
}

v8::Handle<v8::Value> LS2Message::KindWrapper(const v8::Arguments& args)
{
    return MemberFunctionWrapper<LS2Message, const char*>(&LS2Message::Kind, args);
}

const char* LS2Message::Kind() const
{
    return GetString(LSMessageGetKind);
}

v8::Handle<v8::Value> LS2Message::MethodWrapper(const v8::Arguments& args)
{
    return MemberFunctionWrapper<LS2Message, const char*>(&LS2Message::Method, args);
}

const char* LS2Message::Method() const
{
    return GetString(LSMessageGetMethod);
}

v8::Handle<v8::Value> LS2Message::PayloadWrapper(const v8::Arguments& args)
{
    return MemberFunctionWrapper<LS2Message, const char*>(&LS2Message::Payload, args);
}

const char* LS2Message::Payload() const
{
    return GetString(LSMessageGetPayload);
}

v8::Handle<v8::Value> LS2Message::PrintWrapper(const v8::Arguments& args)
{
    return VoidMemberFunctionWrapper<LS2Message>(&LS2Message::Print, args);
}

void LS2Message::Print() const
{
    RequireMessage();
    LSMessagePrint(fMessage, stderr);
}

v8::Handle<v8::Value> LS2Message::RespondWrapper(const v8::Arguments& args)
{
    return MemberFunctionWrapper<LS2Message, bool, const char*>(&LS2Message::Respond, args);
}

bool LS2Message::Respond(const char* payload) const
{
    RequireMessage();
    LSErrorWrapper err;
    if (!LSMessageRespond(fMessage, payload, err)) {
        err.ThrowError();
        return false;
    }
    return true;
}

v8::Handle<v8::Value> LS2Message::ResponseTokenWrapper(const v8::Arguments& args)
{
    return MemberFunctionWrapper<LS2Message, LSMessageToken>(&LS2Message::ResponseToken, args);
}

LSMessageToken LS2Message::ResponseToken() const
{
    return GetToken(LSMessageGetResponseToken);
}

v8::Handle<v8::Value> LS2Message::SenderWrapper(const v8::Arguments& args)
{
    return MemberFunctionWrapper<LS2Message, const char*>(&LS2Message::Sender, args);
}

const char* LS2Message::Sender() const
{
    return GetString(LSMessageGetSender);
}

v8::Handle<v8::Value> LS2Message::SenderServiceNameWrapper(const v8::Arguments& args)
{
    return MemberFunctionWrapper<LS2Message, const char*>(&LS2Message::SenderServiceName, args);
}

const char* LS2Message::SenderServiceName() const
{
    return GetString(LSMessageGetSenderServiceName);
}

v8::Handle<v8::Value> LS2Message::TokenWrapper(const v8::Arguments& args)
{
    return MemberFunctionWrapper<LS2Message, LSMessageToken>(&LS2Message::Token, args);
}

LSMessageToken LS2Message::Token() const
{
    return GetToken(LSMessageGetToken);
}

v8::Handle<v8::Value> LS2Message::UniqueTokenWrapper(const v8::Arguments& args)
{
    return MemberFunctionWrapper<LS2Message, const char*>(&LS2Message::UniqueToken, args);
}

const char* LS2Message::UniqueToken() const
{
    return GetString(LSMessageGetUniqueToken);
}

const char* LS2Message::GetString(StringGetterFunction f) const
{
    RequireMessage();

    const char* s = (*f)(fMessage);
    if (s == 0) {
        return "";
    }
    return s;
}

LSMessageToken LS2Message::GetToken(TokenGetterFunction f) const
{
    RequireMessage();
    return (*f)(fMessage);
}

void LS2Message::RequireMessage() const
{
    if (fMessage == 0) {
        throw runtime_error("Message object is missing native message.");
    }
}
