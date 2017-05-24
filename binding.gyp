{
  "targets": [
    {
      "target_name": "PladeParser",
      "sources": [
        "lib/PladeParser/ASTParser/ASTParser.h",
        "lib/PladeParser/ASTParser/ASTParser.cpp",
        "lib/PladeParser/Exports/DllExport.h",
        "lib/PladeParser/Exports/DllExport.cpp",
        "lib/PladeParser/Exports/GlobalExport.h",
        "lib/PladeParser/Exports/GlobalExport.cpp",
        "lib/PladeParser/Exports/V8Export.h",
        "lib/PladeParser/Exports/V8Export.cpp",
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "third-party/clang/include",
        "third-party/rapidjson/include"
      ],
      "libraries": [
        "libclang.lib"
      ],
      "conditions" : [
        ["target_arch=='ia32'", {
          "library_dirs": [ "third-party/x86/clang/" ]
        }],
        ["target_arch=='x64'", {
          "library_dirs": [ "third-party/x64/clang/" ]
        }]
      ]
    }
  ]
}