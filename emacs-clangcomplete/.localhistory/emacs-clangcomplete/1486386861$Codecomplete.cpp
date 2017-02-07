#include <cstdio>
#include <cstdlib>
#include <clang-c/Index.h>
#include <cstring>
#include <string>
#include <map>
#include <iostream>
#include <vector> 

const char *
_getCompleteChunkKindSpelling(CXCompletionChunkKind chunkKind) {
	switch (chunkKind) {
	case CXCompletionChunk_Optional:
		return "Optional";
		break;
	case CXCompletionChunk_TypedText:
		return "TypedText";
		break;
	case CXCompletionChunk_Text:
		return "Text";
		break;
	case CXCompletionChunk_Placeholder:
		return "Placeholder";
		break;
	case CXCompletionChunk_Informative:
		return "Informative";
		break;
	case CXCompletionChunk_CurrentParameter:
		return "CurrentParameter";
		break;
	case CXCompletionChunk_LeftParen:
		return "LeftParen";
		break;
	case CXCompletionChunk_RightParen:
		return "RightParen";
		break;
	case CXCompletionChunk_LeftBracket:
		return "LeftBracket";
		break;
	case CXCompletionChunk_RightBracket:
		return "RightBracket";
		break;
	case CXCompletionChunk_LeftBrace:
		return "LeftBrace";
		break;
	case CXCompletionChunk_RightBrace:
		return "RightBrace";
		break;
	case CXCompletionChunk_LeftAngle:
		return "LeftAngle";
		break;
	case CXCompletionChunk_RightAngle:
		return "RightAngle";
		break;
	case CXCompletionChunk_Comma:
		return "Comma";
		break;
	case CXCompletionChunk_ResultType:
		return "ResultType";
		break;
	case CXCompletionChunk_Colon:
		return "Colon";
		break;
	case CXCompletionChunk_SemiColon:
		return "SemiColon";
		break;
	case CXCompletionChunk_Equal:
		return "Equal";
		break;
	case CXCompletionChunk_HorizontalSpace:
		return "HorizontalSpace";
		break;
	case CXCompletionChunk_VerticalSpace:
		return "VerticalSpace";
		break;
	default:
		return "Unknown";
		break;
	}
}

const char *
_getCompletionAvailabilitySpelling(CXAvailabilityKind availavility) {
	switch (availavility) {
	case CXAvailability_Available:
		return "Available";
		break;
	case CXAvailability_Deprecated:
		return "Deprecated";
		break;
	case CXAvailability_NotAvailable:
		return "NotAvailable";
		break;
	case CXAvailability_NotAccessible:
		return "NotAccessible";
		break;
	default:
		return "Unknown";
		break;
	}
}

const char *_getKindTypeName(CXCursor cursor) {
	CXCursorKind curKind = clang_getCursorKind(cursor);
	const char *type;
	if (clang_isAttribute(curKind)) {
		type = "Attribute";
	}
	else if (clang_isDeclaration(curKind)) {
		type = "Declaration";
	}
	else if (clang_isExpression(curKind)) {
		type = "Expression";
	}
	else if (clang_isInvalid(curKind)) {
		type = "Invalid";
	}
	else if (clang_isPreprocessing(curKind)) {
		type = "Preprocessing";
	}
	else if (clang_isReference(curKind)) {
		type = "Reference";
	}
	else if (clang_isStatement(curKind)) {
		type = "Statement";
	}
	else if (clang_isTranslationUnit(curKind)) {
		type = "TranslationUnit";
	}
	else if (clang_isUnexposed(curKind)) {
		type = "Unexposed";
	}
	else {
		type = "Unknown";
	}
	return type;
}


const std::string  intellisense(CXCodeCompleteResults *compResults) {

	std::string results;

	for (auto i = 0U; i < compResults->NumResults; i++) {
		const CXCompletionResult &result = compResults->Results[i];
		const CXCompletionString &compString = result.CompletionString;
		const CXCursorKind kind = result.CursorKind;
		CXString comment = clang_getCompletionBriefComment(compString);

		unsigned numChunks = clang_getNumCompletionChunks(compString);
#ifdef _DEBUG
		printf("B4 loop\n");
#endif

		for (auto j = 0U; j < numChunks; j++) {

			CXCompletionChunkKind chunkKind =
				clang_getCompletionChunkKind(compString, j);

			CXString chunkText = clang_getCompletionChunkText(compString, j);


#ifdef _DEBUG
			unsigned priority = clang_getCompletionPriority(compString);
			printf("priority %d   Kind: %s Text: %s Comment: %s\n", priority,
				_getCompleteChunkKindSpelling(chunkKind),
				clang_getCString(chunkText), clang_getCString(comment));
#endif 

			if (memcmp(_getCompleteChunkKindSpelling(chunkKind), "TypedText", 9) == 0)
			{
				results += clang_getCString(chunkText);
				results += " ";
			}
			clang_disposeString(chunkText);
			clang_disposeString(comment);


		}
		results += "\n";
	}


	return results;
}
extern "C" {
	__declspec(dllexport) const char *clang_complete(char *filename,
		int line_number, int column_number) {

		CXIndex index = clang_createIndex(0, 0);

		printf("filename is %s line %d col %d\n", filename,
			line_number,column_number);


		// create Translation Unit
		CXTranslationUnit tu = clang_parseTranslationUnit(
			index, filename, NULL, NULL, NULL, 0,
			CXTranslationUnit_PrecompiledPreamble |
			CXTranslationUnit_Incomplete);
		if (tu == NULL) {
			printf("Cannot parse translation unit\n");
			static const char mesg[] = "Cannot parse translation unit";
			return mesg;

		}

		// Code Completion
		CXCodeCompleteResults *compResults;
		compResults =
			clang_codeCompleteAt(tu, filename, line_number, column_number,
				NULL, 0, clang_defaultCodeCompleteOptions());
		if (compResults == NULL) {
			static  const char mesg[] = "Invalid results";
			return mesg;

		}

		std::string s = intellisense(compResults);
		char* r = (char*)malloc(s.size());
		memcpy(r, s.data(), s.size());


		clang_disposeCodeCompleteResults(compResults);
		clang_disposeTranslationUnit(tu);
		clang_disposeIndex(index);
#ifdef _DEBUG
		printf("complete %*s", s.length(), r);
#endif 

		return r;

	}
}

#ifdef _DEBUG
int main(int argc, char **argv) {
	if (argc < 4) {
		printf("CodeComplete filename line column [options ...]\n");
		exit(1);
	}


	const auto filename = argv[1];
	unsigned lineno = atoi(argv[2]);
	unsigned columnno = atoi(argv[3]);
	const auto cmdArgs = &argv[4];
	auto numArgs = argc - 4;
	clang_complete(filename, lineno, columnno);

}
#endif 