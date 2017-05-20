#include "ASTParser.h"
#include <cstdio>
#include <cstdlib>
#include <stack>
#include <clang-c/Index.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <memory>

namespace PladeParser {
	namespace ASTParser {
#define GetTextWrapper(originalObject, newString)\
char* newString;\
	{\
auto originalString = clang_getCString(originalObject);\
auto len = strlen(originalString) + 1;\
newString = new char[len];\
strncpy_s(newString, len, originalString, len);\
clang_disposeString(originalObject);\
tempStringObjects.push(newString);\
	}
		using namespace rapidjson;
		using namespace std;
		Document ret;
		stack<char*> tempStringObjects;

		void Initialize() {
			ret.Parse("[]");
		}

		void Terminate() {
			while (!tempStringObjects.empty()) {
				auto ptr = tempStringObjects.top();
				tempStringObjects.pop();
				delete[] ptr;
			}
		}

		void GetSpell(Value& single, const CXCursor &cursor) {
			auto spell = clang_getCursorSpelling(cursor);
			GetTextWrapper(spell, copiedText);
			Value string;
			string.SetString(copiedText, static_cast<SizeType>(strlen(copiedText)), ret.GetAllocator());
			single.AddMember("text", string, ret.GetAllocator());
		}

		void GetType(Value& single, const CXCursor &cursor) {
			auto type = clang_getCursorType(cursor);
			auto typeName = clang_getTypeSpelling(type);
			auto typeKind = type.kind;
			auto typeKindName = clang_getTypeKindSpelling(typeKind);
			GetTextWrapper(typeName, copiedTypeNameString);
			GetTextWrapper(typeKindName, copiedTypeKindNameString);
			Value typeObject, typeDataObject, typeKindObject;
			typeObject.SetObject();
			typeDataObject.SetString(copiedTypeNameString, strlen(copiedTypeNameString), ret.GetAllocator());
			typeKindObject.SetString(copiedTypeKindNameString, strlen(copiedTypeKindNameString), ret.GetAllocator());
			typeObject.AddMember("type", typeDataObject, ret.GetAllocator());
			typeObject.AddMember("kind", typeKindObject, ret.GetAllocator());
			single.AddMember("type", typeObject, ret.GetAllocator());
		}

		void GetLinkage(Value& single, const CXCursor &cursor) {

			auto linkage = clang_getCursorLinkage(cursor);
			string linkageName;
			switch (linkage) {
				case CXLinkage_Invalid:        linkageName = "Invalid"; break;
				case CXLinkage_NoLinkage:      linkageName = "NoLinkage"; break;
				case CXLinkage_Internal:       linkageName = "Internal"; break;
				case CXLinkage_UniqueExternal: linkageName = "UniqueExternal"; break;
				case CXLinkage_External:       linkageName = "External"; break;
				default:                       linkageName = "Unknown"; break;
			}
			Value linkObject;
			linkObject.SetString(linkageName.c_str(), static_cast<SizeType>(linkageName.length()), ret.GetAllocator());
			single.AddMember("link", linkObject, ret.GetAllocator());
		}

		void GetParent(Value& single, const CXCursor &cursor, const CXCursor &parent) {
			auto semaParent = clang_getCursorSemanticParent(cursor);
			auto lexParent = clang_getCursorLexicalParent(cursor);
			auto parentName = clang_getCursorSpelling(parent);
			auto semaParentName = clang_getCursorSpelling(semaParent);
			auto lexParentName = clang_getCursorSpelling(lexParent);

			Value returnObject, parentObject, semaParentObject, lexParentObject;
			returnObject.SetObject();
			GetTextWrapper(parentName, copiedParentNameString);
			GetTextWrapper(semaParentName, copiedSemaParentNameString);
			GetTextWrapper(lexParentName, copiedLexParentNameString);

			parentObject.SetString(copiedParentNameString, strlen(copiedParentNameString), ret.GetAllocator());
			semaParentObject.SetString(copiedSemaParentNameString, strlen(copiedSemaParentNameString), ret.GetAllocator());
			lexParentObject.SetString(copiedLexParentNameString, strlen(copiedLexParentNameString), ret.GetAllocator());
			returnObject.AddMember("parent", parentObject, ret.GetAllocator());
			returnObject.AddMember("semantic", semaParentObject, ret.GetAllocator());
			returnObject.AddMember("lexicial", lexParentObject, ret.GetAllocator());
			single.AddMember("parent", returnObject.Move(), ret.GetAllocator());

		}

		void GetLocation(Value& single, const CXCursor &cursor) {
			auto loc = clang_getCursorLocation(cursor);
			Value location;
			CXFile file;
			unsigned line, column, offset;
			clang_getSpellingLocation(loc, &file, &line, &column, &offset);
			auto fileName = clang_getFileName(file);
			GetTextWrapper(fileName, fileNameString);

			Value lineValue, columnValue, offsetValue, filenameValue;
			lineValue.SetInt(line);
			columnValue.SetInt(column);
			offsetValue.SetInt(offset);
			filenameValue.SetString(fileNameString, static_cast<SizeType>(strlen(fileNameString)), ret.GetAllocator());
			location.SetObject();
			location.AddMember("line", lineValue, ret.GetAllocator());
			location.AddMember("column", columnValue, ret.GetAllocator());
			location.AddMember("offset", offsetValue, ret.GetAllocator());
			location.AddMember("filename", filenameValue, ret.GetAllocator());

			single.AddMember("location", location.Move(), ret.GetAllocator());

		}

		void GetUsr(Value& single, const CXCursor &cursor) {
			auto usr = clang_getCursorUSR(cursor);
			GetTextWrapper(usr, text)

				Value string;
			string.SetString(text, static_cast<SizeType>(strlen(text)), ret.GetAllocator());
			single.AddMember("usr", string, ret.GetAllocator());
		}

		void GetCursorKind(Value& single, const CXCursor &cursor) {
			auto curKind = clang_getCursorKind(cursor);
			auto curKindName = clang_getCursorKindSpelling(curKind);
			const char *type;
			GetTextWrapper(curKindName, curKindString);

			if (clang_isAttribute(curKind))            type = "Attribute";
			else if (clang_isDeclaration(curKind))     type = "Declaration";
			else if (clang_isExpression(curKind))      type = "Expression";
			else if (clang_isInvalid(curKind))         type = "Invalid";
			else if (clang_isPreprocessing(curKind))   type = "Preprocessing";
			else if (clang_isReference(curKind))       type = "Reference";
			else if (clang_isStatement(curKind))       type = "Statement";
			else if (clang_isTranslationUnit(curKind)) type = "TranslationUnit";
			else if (clang_isUnexposed(curKind))       type = "Unexposed";
			else                                       type = "Unknown";
			Value Kind, KindObject, KindTypeObject;
			Kind.SetObject();
			KindObject.SetString(type, strlen(type), ret.GetAllocator());
			KindTypeObject.SetString(curKindString, strlen(curKindString), ret.GetAllocator());
			Kind.AddMember("kind", KindObject, ret.GetAllocator());
			Kind.AddMember("type", KindTypeObject, ret.GetAllocator());
			single.AddMember("kind", Kind, ret.GetAllocator());

		}

		void GetIncludedFile(Value& single, const CXCursor &cursor) {
			auto included = clang_getIncludedFile(cursor);
			if (included == nullptr) return;

			auto includedFileName = clang_getFileName(included);
			GetTextWrapper(includedFileName, fileName);
			Value string;
			string.SetString(fileName, static_cast<SizeType>(strlen(fileName)), ret.GetAllocator());
			single.AddMember("included", string, ret.GetAllocator());
		}

		CXChildVisitResult visitChildrenCallback(CXCursor cursor,
			CXCursor parent,
			CXClientData client_data) {
			auto level = *static_cast<unsigned *>(client_data);
			Value single, Level;
			single.SetObject();
			Level.SetInt(level);
			single.AddMember("level", Level, ret.GetAllocator());

			GetIncludedFile(single, cursor);
			GetSpell(single, cursor);
			GetLinkage(single, cursor);
			GetCursorKind(single, cursor);
			GetType(single, cursor);
			GetParent(single, cursor, parent);
			GetLocation(single, cursor);
			GetUsr(single, cursor);

			// visit children recursively
			auto next = level + 1;
			clang_visitChildren(cursor, visitChildrenCallback, &next);

			ret.PushBack(single, ret.GetAllocator());
			return CXChildVisit_Recurse;
		}

		const char* GetClangVersion() {
			auto version = clang_getClangVersion();
			GetTextWrapper(version, returnVersion);
			return returnVersion;
		}

		Document* GetJSONDocument() {
			return &ret;
		}
	}
}