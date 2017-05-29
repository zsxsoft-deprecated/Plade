#ifndef __EXPORTS_V8EXPORT_H_
#define __EXPORTS_V8EXPORT_H_
#include <nan.h>

void fnPladeParser(const Nan::FunctionCallbackInfo<v8::Value>& info);
void GetClangVersion(const Nan::FunctionCallbackInfo<v8::Value>& info);
void GetMainFile(const Nan::FunctionCallbackInfo<v8::Value>& info);
void TerminateParser(const Nan::FunctionCallbackInfo<v8::Value>& info);
void Init(v8::Local<v8::Object> exports);

#endif