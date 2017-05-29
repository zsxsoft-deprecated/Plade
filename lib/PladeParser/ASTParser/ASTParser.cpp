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
	using namespace rapidjson;
	using namespace std;

	ASTParser::ASTParser() {
		ret = new Document();
		ret->Parse("[]");
		singleItem = new Value();
	}

	ASTParser::~ASTParser() {
		delete singleItem;
		delete ret;
	}

	void ASTParser::GetSpell() {
		auto spell = clang_getCursorSpelling(*cursor);
		auto copiedText = GetTextWrapper(spell);
		Value string;
		string.SetString(copiedText, static_cast<SizeType>(strlen(copiedText)), ret->GetAllocator());
		singleItem->AddMember("text", string, ret->GetAllocator());
	}

	void ASTParser::GetType() {
		auto type = clang_getCursorType(*cursor);
		auto typeName = clang_getTypeSpelling(type);
		auto typeKind = type.kind;
		auto typeKindName = clang_getTypeKindSpelling(typeKind);
		auto copiedTypeNameString = GetTextWrapper(typeName);
		auto copiedTypeKindNameString = GetTextWrapper(typeKindName);
		Value typeObject, typeDataObject, typeKindObject;
		typeObject.SetObject();
		typeDataObject.SetString(copiedTypeNameString, static_cast<SizeType>(strlen(copiedTypeNameString)), ret->GetAllocator());
		typeKindObject.SetString(copiedTypeKindNameString, static_cast<SizeType>(strlen(copiedTypeKindNameString)), ret->GetAllocator());
		typeObject.AddMember("type", typeDataObject, ret->GetAllocator());
		typeObject.AddMember("kind", typeKindObject, ret->GetAllocator());
		singleItem->AddMember("type", typeObject, ret->GetAllocator());
	}

	void ASTParser::GetLinkage() {
		auto linkage = clang_getCursorLinkage(*cursor);
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
		linkObject.SetString(linkageName.c_str(), static_cast<SizeType>(linkageName.length()), ret->GetAllocator());
		singleItem->AddMember("link", linkObject, ret->GetAllocator());
	}

	void ASTParser::GetParent() {
		auto semaParent = clang_getCursorSemanticParent(*cursor);
		auto lexParent = clang_getCursorLexicalParent(*cursor);
		auto parentName = clang_getCursorSpelling(*parentCursor);
		auto semaParentName = clang_getCursorSpelling(semaParent);
		auto lexParentName = clang_getCursorSpelling(lexParent);

		Value returnObject, parentObject, semaParentObject, lexParentObject;
		returnObject.SetObject();
		auto copiedParentNameString = GetTextWrapper(parentName);
		auto copiedSemaParentNameString = GetTextWrapper(semaParentName);
		auto copiedLexParentNameString = GetTextWrapper(lexParentName);

		parentObject.SetString(copiedParentNameString, static_cast<SizeType>(strlen(copiedParentNameString)), ret->GetAllocator());
		semaParentObject.SetString(copiedSemaParentNameString, static_cast<SizeType>(strlen(copiedSemaParentNameString)), ret->GetAllocator());
		lexParentObject.SetString(copiedLexParentNameString, static_cast<SizeType>(strlen(copiedLexParentNameString)), ret->GetAllocator());
		returnObject.AddMember("parent", parentObject, ret->GetAllocator());
		returnObject.AddMember("semantic", semaParentObject, ret->GetAllocator());
		returnObject.AddMember("lexicial", lexParentObject, ret->GetAllocator());
		singleItem->AddMember("parent", returnObject.Move(), ret->GetAllocator());

	}


	bool ASTParser::GetFileNameAndCheckCanWeContinue() {
		auto loc = clang_getCursorLocation(*cursor);
		if (clang_Location_isFromMainFile(clang_getCursorLocation(*cursor)) == 0) return false;
		Value location;
		CXFile file;
		unsigned line, column, offset;
		clang_getSpellingLocation(loc, &file, &line, &column, &offset);
		if (file == nullptr) return false;

		auto fileName = clang_getFileName(file);
		auto fileNameString = GetTextWrapper(fileName);
		if (Helpers::isInSystemInclude(fileNameString)) return false;
		/*

		Value lineValue, columnValue, offsetValue, filenameValue;
		lineValue.SetInt(line);
		columnValue.SetInt(column);
		offsetValue.SetInt(offset);
		filenameValue.SetString(fileNameString, static_cast<SizeType>(strlen(fileNameString)), ret->GetAllocator());
		location.SetObject();
		location.AddMember("line", lineValue, ret->GetAllocator());
		location.AddMember("column", columnValue, ret->GetAllocator());
		location.AddMember("offset", offsetValue, ret->GetAllocator());
		location.AddMember("filename", filenameValue, ret->GetAllocator());

		singleItem->AddMember("location", location.Move(), ret->GetAllocator());
		*/
		return true;
	}

	void ASTParser::GetUsr() {
		auto usr = clang_getCursorUSR(*cursor);
		auto text = GetTextWrapper(usr);

		Value string;
		string.SetString(text, static_cast<SizeType>(strlen(text)), ret->GetAllocator());
		singleItem->AddMember("usr", string, ret->GetAllocator());
	}

	void ASTParser::GetCursorKind() {
		auto curKind = clang_getCursorKind(*cursor);
		auto curKindName = clang_getCursorKindSpelling(curKind);
		const char *type;
		auto curKindString = GetTextWrapper(curKindName);

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
		KindObject.SetString(type, static_cast<SizeType>(strlen(type)), ret->GetAllocator());
		KindTypeObject.SetString(curKindString, static_cast<SizeType>(strlen(curKindString)), ret->GetAllocator());
		Kind.AddMember("kind", KindObject, ret->GetAllocator());
		Kind.AddMember("type", KindTypeObject, ret->GetAllocator());
		singleItem->AddMember("kind", Kind, ret->GetAllocator());
	}

	void ASTParser::GetIncludedFile() {
		auto included = clang_getIncludedFile(*cursor);
		if (included == nullptr) return;
		auto includedFileName = clang_getFileName(included);
		auto fileName = GetTextWrapper(includedFileName);
		Document* document = Helpers::OpenClangUnit<Document*>(fileName, [](CXTranslationUnit unit) {
			auto cursor = clang_getTranslationUnitCursor(unit);
			auto parser = new ASTParser();
			clang_visitChildren(cursor, parser->visitChildrenCallback, parser);
			return move(parser->GetJSONDocument());
		});
		// Value string;
		// string.SetString(fileName, static_cast<SizeType>(strlen(fileName)), ret->GetAllocator());
		singleItem->AddMember("included", document->Move(), ret->GetAllocator());
		// delete document;
	}

	CXChildVisitResult ASTParser::visitChildrenCallback(CXCursor cursor, CXCursor parent, CXClientData client_data) {
		// auto level = *static_cast<unsigned *>(client_data);
		auto self = static_cast<ASTParser*>(client_data);
		
		self->singleItem->SetObject();
		self->cursor = &cursor;
		self->parentCursor = &parent;

		if (!self->GetFileNameAndCheckCanWeContinue()) {
			return CXChildVisit_Continue;
		}

		Value Level;
		Level.SetInt(self->currentLevel);
		self->singleItem->AddMember("level", Level.Move(), self->ret->GetAllocator());

		self->GetSpell();
		self->GetLinkage();
		self->GetCursorKind();
		self->GetType();
		// self->GetParent();
		self->GetIncludedFile();
		// self->GetUsr();

		self->ret->PushBack(self->singleItem->Move(), self->ret->GetAllocator());
		//self->singleItem->;
		self->singleItem->SetObject();
		self->currentLevel++;
		clang_visitChildren(cursor, visitChildrenCallback, static_cast<void*>(self));
		self->currentLevel--;
		return CXChildVisit_Recurse;
	}

	const char* ASTParser::GetClangVersion() {
		auto version = clang_getClangVersion();
		auto returnVersion = GetTextWrapper(version);
		return returnVersion;
	}

	Document* ASTParser::GetJSONDocument() const {
 		return ret;
	}
#undef GetTextWrapper
}