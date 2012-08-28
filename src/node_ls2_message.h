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

#ifndef NODE_LS2_MESSAGE_H
#define NODE_LS2_MESSAGE_H

#include <lunaservice.h>
#include <node.h>
#include <node_object_wrap.h>
#include <string>

struct LSMessage;



class LS2Message : public node::ObjectWrap {
public:
	// Create the "Message" function template and add it to the target.
	static void Initialize (v8::Handle<v8::Object> target);
	
	// Create a "Message" JavaScript object and wrap it around the C++ LSMessage object.
	static v8::Local<v8::Value> NewFromMessage(LSMessage*);
	
	LSMessage* Get() const;

protected:
	// Called by V8 when the "Message" function is used with new. This has to be here, but the
	// resulting "Message" object is useless as it has no matching LSMessage structure from
	// the lunaservice library.
	static v8::Handle<v8::Value> New(const v8::Arguments& args);

private:
	// This constructor is private as these objects are only created by the 
	// static functions "New" and "NewFromMessage"
	explicit LS2Message(LSMessage* m = 0);
	virtual ~LS2Message();
	
	// Replace the message on an existing "Message" object. Used to avoid
	// Having to create two LS2Messages.
    void SetMessage(LSMessage* m);

	// Wrappers and accessors for use in the "Message" function template. 
	static v8::Handle<v8::Value> ApplicationIDWrapper(const v8::Arguments& args);
	const char* ApplicationID() const;

	static v8::Handle<v8::Value> CategoryWrapper(const v8::Arguments& args);
	const char* Category() const;

	static v8::Handle<v8::Value> IsSubscriptionWrapper(const v8::Arguments& args);
	bool IsSubscription() const;

	static v8::Handle<v8::Value> KindWrapper(const v8::Arguments& args);
	const char* Kind() const;

	static v8::Handle<v8::Value> MethodWrapper(const v8::Arguments& args);
	const char* Method() const;

	static v8::Handle<v8::Value> PayloadWrapper(const v8::Arguments& args);
	const char* Payload() const;

	static v8::Handle<v8::Value> PrintWrapper(const v8::Arguments& args);
	void Print() const;

	static v8::Handle<v8::Value> RespondWrapper(const v8::Arguments& args);
	bool Respond(const char* payload) const;

	static v8::Handle<v8::Value> ResponseTokenWrapper(const v8::Arguments& args);
	LSMessageToken ResponseToken() const;

	static v8::Handle<v8::Value> SenderWrapper(const v8::Arguments& args);
	const char* Sender() const;

	static v8::Handle<v8::Value> SenderServiceNameWrapper(const v8::Arguments& args);
	const char* SenderServiceName() const;

	static v8::Handle<v8::Value> TokenWrapper(const v8::Arguments& args);
	LSMessageToken Token() const;

	static v8::Handle<v8::Value> UniqueTokenWrapper(const v8::Arguments& args);
	const char* UniqueToken() const;

	// Wrapper around an LSMessage string getter function that will throw an
	// exception if fMessage is null and also return an empty string if the
	// lower level accessor returns 0.
	typedef const char* (*StringGetterFunction) (LSMessage *msg);
	const char* GetString(StringGetterFunction f) const;

	// Wrapper around an LSMessage token getter function that will throw an
	// exception if fMessage is null.
	typedef LSMessageToken (*TokenGetterFunction) (LSMessage *msg);
	LSMessageToken GetToken(TokenGetterFunction f) const;
	
	// Throws an exception if fMessage is 0.
	void RequireMessage() const;

	// prevent copying
    LS2Message( const LS2Message& );
    const LS2Message& operator=( const LS2Message& );

	LSMessage* fMessage;
	static v8::Persistent<v8::FunctionTemplate> gMessageTemplate;
};

#endif
