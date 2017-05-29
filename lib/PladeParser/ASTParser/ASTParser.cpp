#include "ASTParser.h"
#include <cstdio>
#include <cstring>
#include <stack>
#include <clang-c/Index.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <memory>
#include "../Helpers/LibClangHelper.h"

namespace PladeParser {
	namespace ASTParser {
#define GetTextWrapper LIBCLANGHELPER_WRAPTEXTTOCHAR
		using namespace rapidjson;
		using namespace std;
		Document ret;
		Document::AllocatorType& retAllocator = ret.GetAllocator();
		LIBCLANGHELPER_USINGCHARDESTRUCTOR

		void Initialize() {
			ret.Parse("[]");
		}

		void Terminate() {
			TempStringDestructor();
		}

		void GetSpell(Value& single, const CXCursor &cursor) {
			auto spell = clang_getCursorSpelling(cursor);
			GetTextWrapper(spell, copiedText);
			Value string;
			string.SetString(copiedText, static_cast<SizeType>(strlen(copiedText)), retAllocator);
			single.AddMember("text", string, retAllocator);
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
			typeDataObject.SetString(copiedTypeNameString, static_cast<SizeType>(strlen(copiedTypeNameString)), retAllocator);
			typeKindObject.SetString(copiedTypeKindNameString, static_cast<SizeType>(strlen(copiedTypeKindNameString)), retAllocator);
			typeObject.AddMember("type", typeDataObject, retAllocator);
			typeObject.AddMember("kind", typeKindObject, retAllocator);
			single.AddMember("type", typeObject, retAllocator);
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
			linkObject.SetString(linkageName.c_str(), static_cast<SizeType>(linkageName.length()), retAllocator);
			single.AddMember("link", linkObject, retAllocator);
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

			parentObject.SetString(copiedParentNameString, static_cast<SizeType>(strlen(copiedParentNameString)), retAllocator);
			semaParentObject.SetString(copiedSemaParentNameString, static_cast<SizeType>(strlen(copiedSemaParentNameString)), retAllocator);
			lexParentObject.SetString(copiedLexParentNameString, static_cast<SizeType>(strlen(copiedLexParentNameString)), retAllocator);
			returnObject.AddMember("parent", parentObject, retAllocator);
			returnObject.AddMember("semantic", semaParentObject, retAllocator);
			returnObject.AddMember("lexicial", lexParentObject, retAllocator);
			single.AddMember("parent", returnObject.Move(), retAllocator);

		}


		bool GetFileNameAndCheckCanWeContinue(Value& single, const CXCursor &cursor) {
			auto loc = clang_getCursorLocation(cursor);
			if (clang_Location_isFromMainFile(clang_getCursorLocation(cursor)) == 0) return false;
			Value location;
			CXFile file;
			unsigned line, column, offset;
			clang_getSpellingLocation(loc, &file, &line, &column, &offset);
			if (file == nullptr) return false;

			auto fileName = clang_getFileName(file);
			GetTextWrapper(fileName, fileNameString);
			if (Helpers::isInSystemInclude(fileNameString)) return false;
			

			Value lineValue, columnValue, offsetValue, filenameValue;
			lineValue.SetInt(line);
			columnValue.SetInt(column);
			offsetValue.SetInt(offset);
			filenameValue.SetString(fileNameString, static_cast<SizeType>(strlen(fileNameString)), retAllocator);
			location.SetObject();
			location.AddMember("line", lineValue, retAllocator);
			location.AddMember("column", columnValue, retAllocator);
			location.AddMember("offset", offsetValue, retAllocator);
			location.AddMember("filename", filenameValue, retAllocator);

			printf("%s, %d, %d, %d\n", fileNameString, line, column, offset);

			single.AddMember("location", location.Move(), retAllocator);
			return true;
		}

		void GetUsr(Value& single, const CXCursor &cursor) {
			auto usr = clang_getCursorUSR(cursor);
			GetTextWrapper(usr, text);

			Value string;
			string.SetString(text, static_cast<SizeType>(strlen(text)), retAllocator);
			single.AddMember("usr", string, retAllocator);
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
			KindObject.SetString(type, static_cast<SizeType>(strlen(type)), retAllocator);
			KindTypeObject.SetString(curKindString, static_cast<SizeType>(strlen(curKindString)), retAllocator);
			Kind.AddMember("kind", KindObject, retAllocator);
			Kind.AddMember("type", KindTypeObject, retAllocator);
			single.AddMember("kind", Kind, retAllocator);
		}

		void GetIncludedFile(Value& single, const CXCursor &cursor) {
			auto included = clang_getIncludedFile(cursor);
			if (included == nullptr) return;
			auto includedFileName = clang_getFileName(included);
			GetTextWrapper(includedFileName, fileName);
			Value string;
			string.SetString(fileName, static_cast<SizeType>(strlen(fileName)), retAllocator);
			single.AddMember("included", string, retAllocator);
		}

		CXChildVisitResult visitChildrenCallback(CXCursor cursor,
			CXCursor parent,
			CXClientData client_data) {
			auto level = *static_cast<unsigned *>(client_data);

			Value single, Level;
			single.SetObject();

			if (!GetFileNameAndCheckCanWeContinue(single, cursor)) {
				return CXChildVisit_Continue;
			}

			Level.SetInt(level);
			single.AddMember("level", Level.Move(), retAllocator);

			GetSpell(single, cursor);
			GetLinkage(single, cursor);
			GetCursorKind(single, cursor);
			GetType(single, cursor);
			GetParent(single, cursor, parent);
			GetIncludedFile(single, cursor);
			GetUsr(single, cursor);

			auto next = level + 1;
			clang_visitChildren(cursor, visitChildrenCallback, &next);
			printf("%d", level);
			ret.PushBack(single.Move(), retAllocator);

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
#undef GetTextWrapper
}