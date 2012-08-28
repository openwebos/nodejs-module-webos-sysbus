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

// Generated file, do not edit. See generate_templates.rb for details on how to regenerate.

// arity: 0, const:

template <typename T,typename RV> v8::Handle<v8::Value> MemberFunctionWrapper(RV (T::*MemFunc)(), v8::Arguments const & args )
{
    if (args.Length() != 0) {
        return ThrowException(v8::Exception::Error(
                                  v8::String::New("Invalid number of parameters, 0 expected.")));
    }

    T *o = node::ObjectWrap::Unwrap<T>(args.This());
    if (!o) {
        return v8::ThrowException(v8::String::New("Unable to unwrap native object."));
    }

    try {

        return ConvertToJS<RV>((o->*MemFunc)());
    } catch( std::exception const & ex ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( ... ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
    }
}



// arity: 1, const:

template <typename T,typename RV,typename T1> v8::Handle<v8::Value> MemberFunctionWrapper(RV (T::*MemFunc)(T1), v8::Arguments const & args )
{
    if (args.Length() != 1) {
        return ThrowException(v8::Exception::Error(
                                  v8::String::New("Invalid number of parameters, 1 expected.")));
    }

    T *o = node::ObjectWrap::Unwrap<T>(args.This());
    if (!o) {
        return v8::ThrowException(v8::String::New("Unable to unwrap native object."));
    }

    try {

        ConvertFromJS<T1> p1(args[0]);

        return ConvertToJS<RV>((o->*MemFunc)(p1));
    } catch( std::exception const & ex ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( ... ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
    }
}



// arity: 2, const:

template <typename T,typename RV,typename T1,typename T2> v8::Handle<v8::Value> MemberFunctionWrapper(RV (T::*MemFunc)(T1,T2), v8::Arguments const & args )
{
    if (args.Length() != 2) {
        return ThrowException(v8::Exception::Error(
                                  v8::String::New("Invalid number of parameters, 2 expected.")));
    }

    T *o = node::ObjectWrap::Unwrap<T>(args.This());
    if (!o) {
        return v8::ThrowException(v8::String::New("Unable to unwrap native object."));
    }

    try {

        ConvertFromJS<T1> p1(args[0]);

        ConvertFromJS<T2> p2(args[1]);

        return ConvertToJS<RV>((o->*MemFunc)(p1,p2));
    } catch( std::exception const & ex ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( ... ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
    }
}



// arity: 3, const:

template <typename T,typename RV,typename T1,typename T2,typename T3> v8::Handle<v8::Value> MemberFunctionWrapper(RV (T::*MemFunc)(T1,T2,T3), v8::Arguments const & args )
{
    if (args.Length() != 3) {
        return ThrowException(v8::Exception::Error(
                                  v8::String::New("Invalid number of parameters, 3 expected.")));
    }

    T *o = node::ObjectWrap::Unwrap<T>(args.This());
    if (!o) {
        return v8::ThrowException(v8::String::New("Unable to unwrap native object."));
    }

    try {

        ConvertFromJS<T1> p1(args[0]);

        ConvertFromJS<T2> p2(args[1]);

        ConvertFromJS<T3> p3(args[2]);

        return ConvertToJS<RV>((o->*MemFunc)(p1,p2,p3));
    } catch( std::exception const & ex ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( ... ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
    }
}



// arity: 4, const:

template <typename T,typename RV,typename T1,typename T2,typename T3,typename T4> v8::Handle<v8::Value> MemberFunctionWrapper(RV (T::*MemFunc)(T1,T2,T3,T4), v8::Arguments const & args )
{
    if (args.Length() != 4) {
        return ThrowException(v8::Exception::Error(
                                  v8::String::New("Invalid number of parameters, 4 expected.")));
    }

    T *o = node::ObjectWrap::Unwrap<T>(args.This());
    if (!o) {
        return v8::ThrowException(v8::String::New("Unable to unwrap native object."));
    }

    try {

        ConvertFromJS<T1> p1(args[0]);

        ConvertFromJS<T2> p2(args[1]);

        ConvertFromJS<T3> p3(args[2]);

        ConvertFromJS<T4> p4(args[3]);

        return ConvertToJS<RV>((o->*MemFunc)(p1,p2,p3,p4));
    } catch( std::exception const & ex ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( ... ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
    }
}



// arity: 5, const:

template <typename T,typename RV,typename T1,typename T2,typename T3,typename T4,typename T5> v8::Handle<v8::Value> MemberFunctionWrapper(RV (T::*MemFunc)(T1,T2,T3,T4,T5), v8::Arguments const & args )
{
    if (args.Length() != 5) {
        return ThrowException(v8::Exception::Error(
                                  v8::String::New("Invalid number of parameters, 5 expected.")));
    }

    T *o = node::ObjectWrap::Unwrap<T>(args.This());
    if (!o) {
        return v8::ThrowException(v8::String::New("Unable to unwrap native object."));
    }

    try {

        ConvertFromJS<T1> p1(args[0]);

        ConvertFromJS<T2> p2(args[1]);

        ConvertFromJS<T3> p3(args[2]);

        ConvertFromJS<T4> p4(args[3]);

        ConvertFromJS<T5> p5(args[4]);

        return ConvertToJS<RV>((o->*MemFunc)(p1,p2,p3,p4,p5));
    } catch( std::exception const & ex ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( ... ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
    }
}



// arity: 6, const:

template <typename T,typename RV,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6> v8::Handle<v8::Value> MemberFunctionWrapper(RV (T::*MemFunc)(T1,T2,T3,T4,T5,T6), v8::Arguments const & args )
{
    if (args.Length() != 6) {
        return ThrowException(v8::Exception::Error(
                                  v8::String::New("Invalid number of parameters, 6 expected.")));
    }

    T *o = node::ObjectWrap::Unwrap<T>(args.This());
    if (!o) {
        return v8::ThrowException(v8::String::New("Unable to unwrap native object."));
    }

    try {

        ConvertFromJS<T1> p1(args[0]);

        ConvertFromJS<T2> p2(args[1]);

        ConvertFromJS<T3> p3(args[2]);

        ConvertFromJS<T4> p4(args[3]);

        ConvertFromJS<T5> p5(args[4]);

        ConvertFromJS<T6> p6(args[5]);

        return ConvertToJS<RV>((o->*MemFunc)(p1,p2,p3,p4,p5,p6));
    } catch( std::exception const & ex ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( ... ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
    }
}



// arity: 0, const:

template <typename T> v8::Handle<v8::Value> VoidMemberFunctionWrapper(void (T::*MemFunc)(), v8::Arguments const & args )
{
    if (args.Length() != 0) {
        return ThrowException(v8::Exception::Error(
                                  v8::String::New("Invalid number of parameters, 0 expected.")));
    }

    T *o = node::ObjectWrap::Unwrap<T>(args.This());
    if (!o) {
        return v8::ThrowException(v8::String::New("Unable to unwrap native object."));
    }

    try {

        (o->*MemFunc)();
        return v8::Undefined();
    } catch( std::exception const & ex ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( ... ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
    }
}



// arity: 1, const:

template <typename T,typename T1> v8::Handle<v8::Value> VoidMemberFunctionWrapper(void (T::*MemFunc)(T1), v8::Arguments const & args )
{
    if (args.Length() != 1) {
        return ThrowException(v8::Exception::Error(
                                  v8::String::New("Invalid number of parameters, 1 expected.")));
    }

    T *o = node::ObjectWrap::Unwrap<T>(args.This());
    if (!o) {
        return v8::ThrowException(v8::String::New("Unable to unwrap native object."));
    }

    try {

        ConvertFromJS<T1> p1(args[0]);

        (o->*MemFunc)(p1);
        return v8::Undefined();
    } catch( std::exception const & ex ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( ... ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
    }
}



// arity: 2, const:

template <typename T,typename T1,typename T2> v8::Handle<v8::Value> VoidMemberFunctionWrapper(void (T::*MemFunc)(T1,T2), v8::Arguments const & args )
{
    if (args.Length() != 2) {
        return ThrowException(v8::Exception::Error(
                                  v8::String::New("Invalid number of parameters, 2 expected.")));
    }

    T *o = node::ObjectWrap::Unwrap<T>(args.This());
    if (!o) {
        return v8::ThrowException(v8::String::New("Unable to unwrap native object."));
    }

    try {

        ConvertFromJS<T1> p1(args[0]);

        ConvertFromJS<T2> p2(args[1]);

        (o->*MemFunc)(p1,p2);
        return v8::Undefined();
    } catch( std::exception const & ex ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( ... ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
    }
}



// arity: 3, const:

template <typename T,typename T1,typename T2,typename T3> v8::Handle<v8::Value> VoidMemberFunctionWrapper(void (T::*MemFunc)(T1,T2,T3), v8::Arguments const & args )
{
    if (args.Length() != 3) {
        return ThrowException(v8::Exception::Error(
                                  v8::String::New("Invalid number of parameters, 3 expected.")));
    }

    T *o = node::ObjectWrap::Unwrap<T>(args.This());
    if (!o) {
        return v8::ThrowException(v8::String::New("Unable to unwrap native object."));
    }

    try {

        ConvertFromJS<T1> p1(args[0]);

        ConvertFromJS<T2> p2(args[1]);

        ConvertFromJS<T3> p3(args[2]);

        (o->*MemFunc)(p1,p2,p3);
        return v8::Undefined();
    } catch( std::exception const & ex ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( ... ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
    }
}



// arity: 4, const:

template <typename T,typename T1,typename T2,typename T3,typename T4> v8::Handle<v8::Value> VoidMemberFunctionWrapper(void (T::*MemFunc)(T1,T2,T3,T4), v8::Arguments const & args )
{
    if (args.Length() != 4) {
        return ThrowException(v8::Exception::Error(
                                  v8::String::New("Invalid number of parameters, 4 expected.")));
    }

    T *o = node::ObjectWrap::Unwrap<T>(args.This());
    if (!o) {
        return v8::ThrowException(v8::String::New("Unable to unwrap native object."));
    }

    try {

        ConvertFromJS<T1> p1(args[0]);

        ConvertFromJS<T2> p2(args[1]);

        ConvertFromJS<T3> p3(args[2]);

        ConvertFromJS<T4> p4(args[3]);

        (o->*MemFunc)(p1,p2,p3,p4);
        return v8::Undefined();
    } catch( std::exception const & ex ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( ... ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
    }
}



// arity: 5, const:

template <typename T,typename T1,typename T2,typename T3,typename T4,typename T5> v8::Handle<v8::Value> VoidMemberFunctionWrapper(void (T::*MemFunc)(T1,T2,T3,T4,T5), v8::Arguments const & args )
{
    if (args.Length() != 5) {
        return ThrowException(v8::Exception::Error(
                                  v8::String::New("Invalid number of parameters, 5 expected.")));
    }

    T *o = node::ObjectWrap::Unwrap<T>(args.This());
    if (!o) {
        return v8::ThrowException(v8::String::New("Unable to unwrap native object."));
    }

    try {

        ConvertFromJS<T1> p1(args[0]);

        ConvertFromJS<T2> p2(args[1]);

        ConvertFromJS<T3> p3(args[2]);

        ConvertFromJS<T4> p4(args[3]);

        ConvertFromJS<T5> p5(args[4]);

        (o->*MemFunc)(p1,p2,p3,p4,p5);
        return v8::Undefined();
    } catch( std::exception const & ex ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( ... ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
    }
}



// arity: 6, const:

template <typename T,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6> v8::Handle<v8::Value> VoidMemberFunctionWrapper(void (T::*MemFunc)(T1,T2,T3,T4,T5,T6), v8::Arguments const & args )
{
    if (args.Length() != 6) {
        return ThrowException(v8::Exception::Error(
                                  v8::String::New("Invalid number of parameters, 6 expected.")));
    }

    T *o = node::ObjectWrap::Unwrap<T>(args.This());
    if (!o) {
        return v8::ThrowException(v8::String::New("Unable to unwrap native object."));
    }

    try {

        ConvertFromJS<T1> p1(args[0]);

        ConvertFromJS<T2> p2(args[1]);

        ConvertFromJS<T3> p3(args[2]);

        ConvertFromJS<T4> p4(args[3]);

        ConvertFromJS<T5> p5(args[4]);

        ConvertFromJS<T6> p6(args[5]);

        (o->*MemFunc)(p1,p2,p3,p4,p5,p6);
        return v8::Undefined();
    } catch( std::exception const & ex ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( ... ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
    }
}



// arity: 0, const: const

template <typename T,typename RV> v8::Handle<v8::Value> MemberFunctionWrapper(RV (T::*MemFunc)() const, v8::Arguments const & args )
{
    if (args.Length() != 0) {
        return ThrowException(v8::Exception::Error(
                                  v8::String::New("Invalid number of parameters, 0 expected.")));
    }

    T *o = node::ObjectWrap::Unwrap<T>(args.This());
    if (!o) {
        return v8::ThrowException(v8::String::New("Unable to unwrap native object."));
    }

    try {

        return ConvertToJS<RV>((o->*MemFunc)());
    } catch( std::exception const & ex ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( ... ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
    }
}



// arity: 1, const: const

template <typename T,typename RV,typename T1> v8::Handle<v8::Value> MemberFunctionWrapper(RV (T::*MemFunc)(T1) const, v8::Arguments const & args )
{
    if (args.Length() != 1) {
        return ThrowException(v8::Exception::Error(
                                  v8::String::New("Invalid number of parameters, 1 expected.")));
    }

    T *o = node::ObjectWrap::Unwrap<T>(args.This());
    if (!o) {
        return v8::ThrowException(v8::String::New("Unable to unwrap native object."));
    }

    try {

        ConvertFromJS<T1> p1(args[0]);

        return ConvertToJS<RV>((o->*MemFunc)(p1));
    } catch( std::exception const & ex ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( ... ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
    }
}



// arity: 2, const: const

template <typename T,typename RV,typename T1,typename T2> v8::Handle<v8::Value> MemberFunctionWrapper(RV (T::*MemFunc)(T1,T2) const, v8::Arguments const & args )
{
    if (args.Length() != 2) {
        return ThrowException(v8::Exception::Error(
                                  v8::String::New("Invalid number of parameters, 2 expected.")));
    }

    T *o = node::ObjectWrap::Unwrap<T>(args.This());
    if (!o) {
        return v8::ThrowException(v8::String::New("Unable to unwrap native object."));
    }

    try {

        ConvertFromJS<T1> p1(args[0]);

        ConvertFromJS<T2> p2(args[1]);

        return ConvertToJS<RV>((o->*MemFunc)(p1,p2));
    } catch( std::exception const & ex ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( ... ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
    }
}



// arity: 3, const: const

template <typename T,typename RV,typename T1,typename T2,typename T3> v8::Handle<v8::Value> MemberFunctionWrapper(RV (T::*MemFunc)(T1,T2,T3) const, v8::Arguments const & args )
{
    if (args.Length() != 3) {
        return ThrowException(v8::Exception::Error(
                                  v8::String::New("Invalid number of parameters, 3 expected.")));
    }

    T *o = node::ObjectWrap::Unwrap<T>(args.This());
    if (!o) {
        return v8::ThrowException(v8::String::New("Unable to unwrap native object."));
    }

    try {

        ConvertFromJS<T1> p1(args[0]);

        ConvertFromJS<T2> p2(args[1]);

        ConvertFromJS<T3> p3(args[2]);

        return ConvertToJS<RV>((o->*MemFunc)(p1,p2,p3));
    } catch( std::exception const & ex ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( ... ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
    }
}



// arity: 4, const: const

template <typename T,typename RV,typename T1,typename T2,typename T3,typename T4> v8::Handle<v8::Value> MemberFunctionWrapper(RV (T::*MemFunc)(T1,T2,T3,T4) const, v8::Arguments const & args )
{
    if (args.Length() != 4) {
        return ThrowException(v8::Exception::Error(
                                  v8::String::New("Invalid number of parameters, 4 expected.")));
    }

    T *o = node::ObjectWrap::Unwrap<T>(args.This());
    if (!o) {
        return v8::ThrowException(v8::String::New("Unable to unwrap native object."));
    }

    try {

        ConvertFromJS<T1> p1(args[0]);

        ConvertFromJS<T2> p2(args[1]);

        ConvertFromJS<T3> p3(args[2]);

        ConvertFromJS<T4> p4(args[3]);

        return ConvertToJS<RV>((o->*MemFunc)(p1,p2,p3,p4));
    } catch( std::exception const & ex ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( ... ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
    }
}



// arity: 5, const: const

template <typename T,typename RV,typename T1,typename T2,typename T3,typename T4,typename T5> v8::Handle<v8::Value> MemberFunctionWrapper(RV (T::*MemFunc)(T1,T2,T3,T4,T5) const, v8::Arguments const & args )
{
    if (args.Length() != 5) {
        return ThrowException(v8::Exception::Error(
                                  v8::String::New("Invalid number of parameters, 5 expected.")));
    }

    T *o = node::ObjectWrap::Unwrap<T>(args.This());
    if (!o) {
        return v8::ThrowException(v8::String::New("Unable to unwrap native object."));
    }

    try {

        ConvertFromJS<T1> p1(args[0]);

        ConvertFromJS<T2> p2(args[1]);

        ConvertFromJS<T3> p3(args[2]);

        ConvertFromJS<T4> p4(args[3]);

        ConvertFromJS<T5> p5(args[4]);

        return ConvertToJS<RV>((o->*MemFunc)(p1,p2,p3,p4,p5));
    } catch( std::exception const & ex ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( ... ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
    }
}



// arity: 6, const: const

template <typename T,typename RV,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6> v8::Handle<v8::Value> MemberFunctionWrapper(RV (T::*MemFunc)(T1,T2,T3,T4,T5,T6) const, v8::Arguments const & args )
{
    if (args.Length() != 6) {
        return ThrowException(v8::Exception::Error(
                                  v8::String::New("Invalid number of parameters, 6 expected.")));
    }

    T *o = node::ObjectWrap::Unwrap<T>(args.This());
    if (!o) {
        return v8::ThrowException(v8::String::New("Unable to unwrap native object."));
    }

    try {

        ConvertFromJS<T1> p1(args[0]);

        ConvertFromJS<T2> p2(args[1]);

        ConvertFromJS<T3> p3(args[2]);

        ConvertFromJS<T4> p4(args[3]);

        ConvertFromJS<T5> p5(args[4]);

        ConvertFromJS<T6> p6(args[5]);

        return ConvertToJS<RV>((o->*MemFunc)(p1,p2,p3,p4,p5,p6));
    } catch( std::exception const & ex ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( ... ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
    }
}



// arity: 0, const:  const

template <typename T> v8::Handle<v8::Value> VoidMemberFunctionWrapper(void (T::*MemFunc)() const, v8::Arguments const & args )
{
    if (args.Length() != 0) {
        return ThrowException(v8::Exception::Error(
                                  v8::String::New("Invalid number of parameters, 0 expected.")));
    }

    T *o = node::ObjectWrap::Unwrap<T>(args.This());
    if (!o) {
        return v8::ThrowException(v8::String::New("Unable to unwrap native object."));
    }

    try {

        (o->*MemFunc)();
        return v8::Undefined();
    } catch( std::exception const & ex ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( ... ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
    }
}



// arity: 1, const:  const

template <typename T,typename T1> v8::Handle<v8::Value> VoidMemberFunctionWrapper(void (T::*MemFunc)(T1) const, v8::Arguments const & args )
{
    if (args.Length() != 1) {
        return ThrowException(v8::Exception::Error(
                                  v8::String::New("Invalid number of parameters, 1 expected.")));
    }

    T *o = node::ObjectWrap::Unwrap<T>(args.This());
    if (!o) {
        return v8::ThrowException(v8::String::New("Unable to unwrap native object."));
    }

    try {

        ConvertFromJS<T1> p1(args[0]);

        (o->*MemFunc)(p1);
        return v8::Undefined();
    } catch( std::exception const & ex ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( ... ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
    }
}



// arity: 2, const:  const

template <typename T,typename T1,typename T2> v8::Handle<v8::Value> VoidMemberFunctionWrapper(void (T::*MemFunc)(T1,T2) const, v8::Arguments const & args )
{
    if (args.Length() != 2) {
        return ThrowException(v8::Exception::Error(
                                  v8::String::New("Invalid number of parameters, 2 expected.")));
    }

    T *o = node::ObjectWrap::Unwrap<T>(args.This());
    if (!o) {
        return v8::ThrowException(v8::String::New("Unable to unwrap native object."));
    }

    try {

        ConvertFromJS<T1> p1(args[0]);

        ConvertFromJS<T2> p2(args[1]);

        (o->*MemFunc)(p1,p2);
        return v8::Undefined();
    } catch( std::exception const & ex ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( ... ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
    }
}



// arity: 3, const:  const

template <typename T,typename T1,typename T2,typename T3> v8::Handle<v8::Value> VoidMemberFunctionWrapper(void (T::*MemFunc)(T1,T2,T3) const, v8::Arguments const & args )
{
    if (args.Length() != 3) {
        return ThrowException(v8::Exception::Error(
                                  v8::String::New("Invalid number of parameters, 3 expected.")));
    }

    T *o = node::ObjectWrap::Unwrap<T>(args.This());
    if (!o) {
        return v8::ThrowException(v8::String::New("Unable to unwrap native object."));
    }

    try {

        ConvertFromJS<T1> p1(args[0]);

        ConvertFromJS<T2> p2(args[1]);

        ConvertFromJS<T3> p3(args[2]);

        (o->*MemFunc)(p1,p2,p3);
        return v8::Undefined();
    } catch( std::exception const & ex ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( ... ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
    }
}



// arity: 4, const:  const

template <typename T,typename T1,typename T2,typename T3,typename T4> v8::Handle<v8::Value> VoidMemberFunctionWrapper(void (T::*MemFunc)(T1,T2,T3,T4) const, v8::Arguments const & args )
{
    if (args.Length() != 4) {
        return ThrowException(v8::Exception::Error(
                                  v8::String::New("Invalid number of parameters, 4 expected.")));
    }

    T *o = node::ObjectWrap::Unwrap<T>(args.This());
    if (!o) {
        return v8::ThrowException(v8::String::New("Unable to unwrap native object."));
    }

    try {

        ConvertFromJS<T1> p1(args[0]);

        ConvertFromJS<T2> p2(args[1]);

        ConvertFromJS<T3> p3(args[2]);

        ConvertFromJS<T4> p4(args[3]);

        (o->*MemFunc)(p1,p2,p3,p4);
        return v8::Undefined();
    } catch( std::exception const & ex ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( ... ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
    }
}



// arity: 5, const:  const

template <typename T,typename T1,typename T2,typename T3,typename T4,typename T5> v8::Handle<v8::Value> VoidMemberFunctionWrapper(void (T::*MemFunc)(T1,T2,T3,T4,T5) const, v8::Arguments const & args )
{
    if (args.Length() != 5) {
        return ThrowException(v8::Exception::Error(
                                  v8::String::New("Invalid number of parameters, 5 expected.")));
    }

    T *o = node::ObjectWrap::Unwrap<T>(args.This());
    if (!o) {
        return v8::ThrowException(v8::String::New("Unable to unwrap native object."));
    }

    try {

        ConvertFromJS<T1> p1(args[0]);

        ConvertFromJS<T2> p2(args[1]);

        ConvertFromJS<T3> p3(args[2]);

        ConvertFromJS<T4> p4(args[3]);

        ConvertFromJS<T5> p5(args[4]);

        (o->*MemFunc)(p1,p2,p3,p4,p5);
        return v8::Undefined();
    } catch( std::exception const & ex ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( ... ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
    }
}



// arity: 6, const:  const

template <typename T,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6> v8::Handle<v8::Value> VoidMemberFunctionWrapper(void (T::*MemFunc)(T1,T2,T3,T4,T5,T6) const, v8::Arguments const & args )
{
    if (args.Length() != 6) {
        return ThrowException(v8::Exception::Error(
                                  v8::String::New("Invalid number of parameters, 6 expected.")));
    }

    T *o = node::ObjectWrap::Unwrap<T>(args.This());
    if (!o) {
        return v8::ThrowException(v8::String::New("Unable to unwrap native object."));
    }

    try {

        ConvertFromJS<T1> p1(args[0]);

        ConvertFromJS<T2> p2(args[1]);

        ConvertFromJS<T3> p3(args[2]);

        ConvertFromJS<T4> p4(args[3]);

        ConvertFromJS<T5> p5(args[4]);

        ConvertFromJS<T6> p6(args[5]);

        (o->*MemFunc)(p1,p2,p3,p4,p5,p6);
        return v8::Undefined();
    } catch( std::exception const & ex ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New(ex.what())));
    } catch( ... ) {
        return v8::ThrowException( v8::Exception::Error(v8::String::New("Native function threw an unknown exception.")));
    }
}


