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

#ifndef NODE_LS2_HANDLE_H
#define NODE_LS2_HANDLE_H

#include "node_ls2_base.h"

#include <set>
#include <glib.h>
#include <lunaservice.h>
#include <vector>

class LS2Message;
class LS2Call;

class LS2Handle : public LS2Base {
public:
	// Create the "Handle" function template and add it to the target.
	static void Initialize (v8::Handle<v8::Object> target);
	
    void CallCreated(LS2Call* call);
    void CallCompleted(LS2Call* call);
    
    LSHandle* Get();
    bool IsValid() { return fHandle != 0;}

protected:
	// Called by V8 when the "Handle" function is used with new.
	static v8::Handle<v8::Value> New(const v8::Arguments& args);

private:
	// This constructor is private as these objects are only created by the 
	// static function "New".
	LS2Handle(const char* name = "", bool publicBus = false);
	virtual ~LS2Handle();

	static v8::Handle<v8::Value> CallWrapper(const v8::Arguments& args);
	v8::Handle<v8::Value> Call(const char* busName, const char* payload);

	static v8::Handle<v8::Value> WatchWrapper(const v8::Arguments& args);
	v8::Handle<v8::Value> Watch(const char* busName, const char* payload);

	static v8::Handle<v8::Value> SubscribeWrapper(const v8::Arguments& args);
	v8::Handle<v8::Value> Subscribe(const char* busName, const char* payload);

	static v8::Handle<v8::Value> CancelWrapper(const v8::Arguments& args);
	bool Cancel(LSMessageToken token);

	static v8::Handle<v8::Value> RegisterMethodWrapper(const v8::Arguments& args);
	void RegisterMethod(const char* category, const char* methodName);

	static v8::Handle<v8::Value> UnregisterWrapper(const v8::Arguments& args);
	void Unregister();

	static v8::Handle<v8::Value> PushRoleWrapper(const v8::Arguments& args);
	void PushRole(const char* pathToRoleFile);

	static v8::Handle<v8::Value> SubscriptionAddWrapper(const v8::Arguments& args);
	void SubscriptionAdd(const char* key, LS2Message* msg);

	// Common implmentation for Call, Watch and Subscribe
   	v8::Handle<v8::Value> CallInternal(const char* busName, const char* payload, int responseLimit);
   	
   	// Glib integration
	void Attach(GMainLoop *mainLoop);
	
	// Method registration implementation method.
	bool RegisterCategory(const char* categoryName, LSMethod *methods);
	
	static bool CancelCallback(LSHandle *sh, LSMessage *message, void *ctx);
	bool CancelArrived(LSMessage *message);

	static bool RequestCallback(LSHandle *sh, LSMessage *message, void *ctx);
	bool RequestArrived(LSMessage *message);
	
	// Common routine called whenever a message arrives from the bus. Different symbols
	// are used to differentiate requests, responses and cancelled subscriptions
	void EmitMessage(const v8::Handle<v8::String>& symbol, LSMessage *message);
	
	// Throws an exception if fHandle is 0.
	void RequireHandle();
	
	// prevent copying
    LS2Handle( const LS2Handle& );
    const LS2Handle& operator=( const LS2Handle& );

    // Helper class to manage the data structures LS2 requires for method registration.
	class RegisteredMethod {
	public:
		RegisteredMethod(const char* name);
		LSMethod* GetMethods();
	private:
		std::vector<char> fName;
		LSMethod fMethods[2];
	};

	LSHandle* fHandle;
	
	typedef std::vector<RegisteredMethod*> MethodVector;
	MethodVector fRegisteredMethods;
};


#endif
