#include <experimental/filesystem>
#include <iostream>
#include <string>
#include <map>
#include <queue>
#include <vector>

#include <clang-c/Index.h>
#include "IncludeParser.h"
#include "../Helpers/LibClangHelper.h"

namespace PladeParser {
	namespace IncludeScanner {
		using namespace std;
		using namespace experimental;

		/**
		 * \brief Get a list of the included filename from the param
		 * \param fileName
		 * \return
		 */
		vector<string> GetIncludeFiles(const char* fileName) {
			Initialize();
			return Helpers::OpenClangUnit<vector<string>>(fileName, [](CXTranslationUnit unit) {
				auto cursor = clang_getTranslationUnitCursor(unit);
				clang_visitChildren(cursor, IncludeVisitCallback, nullptr);
				return GetData();
			}, CXTranslationUnit_DetailedPreprocessingRecord | CXTranslationUnit_SkipFunctionBodies);
		}

		/**
		 * \brief Find the entrypoint in a project
		 * \param directionaryPath
		 * \return
		 */
		vector<string> FindMainFile(const char* directionaryPath) {

			map<string, vector<string>> graph;
			map<string, string> extensions;
			map<string, int> indegrees;
			queue<string> setOfIndegrees;
			vector<string> results;
			auto edges = 0;

			// Build include graph
			for (auto &p : filesystem::recursive_directory_iterator(directionaryPath)) {
				filesystem::path path = p.path();
				auto fileEntirePath = path.string();
				auto ext = path.extension().string();
				transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
				transform(fileEntirePath.begin(), fileEntirePath.end(), fileEntirePath.begin(), ::tolower);
				if (fileEntirePath.find("cmake") != string::npos) continue;
				if (ext == ".cpp" || ext == ".c" || ext == ".cc" || ext == ".m" || ext == ".cxx" || ext == ".c++" || ext == ".hpp" || ext == ".h") {

					auto fileNameStem = filesystem::canonical(path.parent_path().string() + "/" + path.filename().stem().string()).string();
					auto ret = GetIncludeFiles(path.string().c_str());
					if (ext[1] != 'h') {
						extensions[fileNameStem] = path.extension().string();
					}
					if (graph.find(fileNameStem) == graph.end()) {
						graph.insert(pair<string, vector<string>>(fileNameStem, vector<string>()));
					}
					if (indegrees.find(fileNameStem) == indegrees.end()) {
						indegrees.insert(pair<string, int>(fileNameStem, 0));
					}
					for (auto &w : ret) {
						filesystem::path returnPath(w);
						auto removeExtensionPath = filesystem::canonical(
							returnPath.parent_path().string() + "/" + returnPath.filename().stem().string()).string();
						graph[fileNameStem].push_back(removeExtensionPath);

						if (indegrees.find(removeExtensionPath) == indegrees.end()) {
							indegrees.insert(pair<string, int>(removeExtensionPath, 1));
						}
						else {
							indegrees[removeExtensionPath]++;
						}
						edges++;
					}
				}
			}

#ifdef DEBUG
			for (auto &w : graph) {
				cout << "******************************" << endl << w.first << endl;
				for (auto &p : w.second) {
					cout << p << endl;
				}
			}
#endif

			// Get a set of Zero Indegree
			for (auto &w : indegrees) {
				if (w.second == 0) {
					setOfIndegrees.push(w.first);
				}
			}

			// Kahn Topological
			while (!setOfIndegrees.empty()) {
				auto v = setOfIndegrees.front();
				setOfIndegrees.pop();
				results.push_back(v + extensions[v]);
				auto vec = graph.find(v);
				if (vec != graph.end()) {
					for (auto &w : vec->second) {
						edges--;
						if (--indegrees[w] == 0) {
							setOfIndegrees.push(w);
						}
					}
				}
			}

			/*
			if (edges != 0) { // Has cycle
				return vector<string>();
			}
			*/

			return results;
		}
	}
}
