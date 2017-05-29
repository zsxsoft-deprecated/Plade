#include <nan.h>
#include <iostream>
#include "GlobalExport.h"
#include "../ASTParser/ASTParser.h"

void fnPladeParser(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	if (info.Length() < 1) {
		Nan::ThrowTypeError("Wrong number of arguments");
		return;
	}
	auto fileNameV8String = info[0]->ToString();
	v8::String::Utf8Value fileNameString(fileNameV8String);
	auto returnString = PladeParser::Exports::ParseCode(*fileNameString);//
	auto returnData = returnString.c_str();
	info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), returnData));
}

void GetClangVersion(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	auto ret = PladeParser::ASTParser::GetClangVersion();
	info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), ret));
}


void GetMainFile(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	if (info.Length() < 1) {
		Nan::ThrowTypeError("Wrong number of arguments");
		return;
	}
	auto fileNameV8String = info[0]->ToString();
	v8::String::Utf8Value fileNameString(fileNameV8String);
	PladeParser::Exports::GetMainFile(*fileNameString);
	info.GetReturnValue().Set(true);
}

void TerminateParser(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	PladeParser::ASTParser::Terminate();
	info.GetReturnValue().Set(true);
}

void Init(v8::Local<v8::Object> exports) {
  exports->Set(Nan::New("parse").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(fnPladeParser)->GetFunction());
  exports->Set(Nan::New("version").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(GetClangVersion)->GetFunction());
  exports->Set(Nan::New("terminate").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(TerminateParser)->GetFunction());
  exports->Set(Nan::New("main").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(GetMainFile)->GetFunction());
}

NODE_MODULE(PladeParser, Init)