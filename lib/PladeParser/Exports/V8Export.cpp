#include <nan.h>
#include "GlobalExport.h"
#include "../ASTParser/ASTParser.h"

void fnPladeParser(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	if (info.Length() < 1) {
		Nan::ThrowTypeError("Wrong number of arguments");
		return;
	}
	auto fileNameV8String = info[0]->ToString();
	v8::String::Utf8Value fileNameString(fileNameV8String);
	auto returnData = PladeParser::Exports::fnPladeParser(*fileNameString);
	info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), returnData));
}

void GetClangVersion(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	auto ret = PladeParser::ASTParser::GetClangVersion();
	info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), ret));
}

void TerminateParser(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	PladeParser::ASTParser::Terminate();
	info.GetReturnValue().Set(true);
}

void Init(v8::Local<v8::Object> exports) {
  exports->Set(Nan::New("parse").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(fnPladeParser)->GetFunction());
  exports->Set(Nan::New("version").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(GetClangVersion)->GetFunction());
  exports->Set(Nan::New("terminate").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(TerminateParser)->GetFunction());
}

NODE_MODULE(PladeParser, Init)