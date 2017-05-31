#include <nan.h>
#include <iostream>
#include "GlobalExport.h"
#include "../../PladeHelper/Locate.h"

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
	auto ret = PladeParser::Exports::GetClangVersion();
	info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), ret));
}


void GetMainFile(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	if (info.Length() < 1) {
		Nan::ThrowTypeError("Wrong number of arguments");
		return;
	}
	auto fileNameV8String = info[0]->ToString();
	v8::String::Utf8Value fileNameString(fileNameV8String);
	auto mainFileArray = PladeParser::Exports::FindMainFile(*fileNameString);

	auto array = v8::Array::New(info.GetIsolate(), mainFileArray.size());
	for (auto it = mainFileArray.begin(); it < mainFileArray.end(); ++it) {
		array->Set(distance(mainFileArray.begin(), it), v8::String::NewFromUtf8(info.GetIsolate(), PladeHelper::Locate::LocateToUTF8(it->c_str())));
	}

	info.GetReturnValue().Set(array);
}


void Init(v8::Local<v8::Object> exports) {
	exports->Set(Nan::New("parse").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(fnPladeParser)->GetFunction());
	exports->Set(Nan::New("version").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(GetClangVersion)->GetFunction());
	exports->Set(Nan::New("main").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(GetMainFile)->GetFunction());
}

NODE_MODULE(PladeParser, Init)