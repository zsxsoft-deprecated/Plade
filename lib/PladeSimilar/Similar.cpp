#include <nan.h>
#include <iostream>
#include "../PladeHelper/Locate.h"
#include <dtl.hpp>
#include <fstream>


double GetSimilarData(const char* fileName1, const char* fileName2) {
	std::ifstream file1Stream(fileName1), file2Stream(fileName2);
	std::string file1((std::istreambuf_iterator<char>(file1Stream)), std::istreambuf_iterator<char>());
	std::string file2((std::istreambuf_iterator<char>(file2Stream)), std::istreambuf_iterator<char>());
	dtl::Diff<char, std::string> d(file1, file2);
	// d.onOnlyEditDistance();
	d.onHuge();
	d.onUnserious();
	d.compose();
	auto data = d.getEditDistance();
	std::cout << data << std::endl;
	auto percent = static_cast<double>(data) / (file1.length() + file2.length());
	return percent;
}

void Similar(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	if (info.Length() < 2) {
		Nan::ThrowTypeError("Wrong number of arguments");
		return;
	}
	v8::String::Utf8Value fileName1(info[0]->ToString());
	if (info[1]->IsString()) {
		if (info.Length() < 2) {
			Nan::ThrowTypeError("Wrong number of arguments");
			return;
		}
		v8::String::Utf8Value fileName2(info[1]->ToString());
		info.GetReturnValue().Set(GetSimilarData(*fileName1, *fileName2));
	}
	if (info[1]->IsArray()) {
		if (info.Length() < 3 || !info[2]->IsFunction()) {
			Nan::ThrowTypeError("Wrong number of arguments");
			return;
		}
		auto fileNameArray = v8::Handle<v8::Array>::Cast(info[1]);
		auto length = fileNameArray->Length();
		auto returnArray = v8::Array::New(info.GetIsolate(), length);
		Nan::Callback callback(v8::Handle<v8::Function>::Cast(info[2]));


		for (uint32_t i = 0; i < length; i++) {
			v8::String::Utf8Value fileName2(fileNameArray->Get(i)->ToString());
			auto returnData = v8::Number::New(info.GetIsolate(), GetSimilarData(*fileName1, *fileName2));
			returnArray->Set(i, returnData);
			v8::Local<v8::Value> data[] = { v8::Integer::New(info.GetIsolate(), i), returnData };
			callback.Call(2, data);
		}
		info.GetReturnValue().Set(returnArray);
	}
}


void Init(v8::Local<v8::Object> exports) {
	exports->Set(Nan::New("similar").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(Similar)->GetFunction());
}

NODE_MODULE(PladeSimilar, Init)