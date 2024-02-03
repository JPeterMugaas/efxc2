//--------------------------------------------------------------------------------------
// File: efxc2.cpp
//
// Copyright (c) J. Peter Mugaas
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//--------------------------------------------------------------------------------------
#include "efxc2.h"

void print_version() {
  printf(PROGRAM_DESCRIPTION " version "  PROGRAM_VERSION "\n");
  printf(PROGRAM_COPYRIGHT "\n");
  exit(0);
}
void print_usage_arg() {
  // https://msdn.microsoft.com/en-us/library/windows/desktop/bb509709(v=vs.85).aspx
  printf("You have specified an argument that is not handled by efxc2\n");
  printf("This isn't a sign of disaster, odds are it will be very easy to add support for this argument.\n");
  printf("Review the meaning of the argument in the real fxc program, and then add it into efxc2.\n");
  exit(1);
}
void print_usage_missing(const char* arg) {
  printf("efxc2 is missing the %s argument. We expected to receive this, and it's likely things ", arg);
  printf("will not work correctly without it. Review efxc2 and make sure things will work.\n");
  exit(1);
}
void print_usage_toomany() {
  printf("You specified multiple input files. We did not expect to receive this, and aren't prepared to handle ");
  printf("multiple input files. You'll have to edit the source to behave the way you want.\n");
  exit(1);
}

bool parseOpt( const char* option, int argc, const char** argv, int* index, char** argumentOption )
{
    assert(option != NULL);
    if (!index || *index >= argc) {
      return false;
    }
    const char* argument = argv[*index];
    if (argument[0] == '-' || argument[0] == '/')
      argument++;
    else
      return false;

    size_t optionSize = strlen(option);
    if (strncmp(argument, option, optionSize) != 0) {
      return false;
    }

    if (argumentOption) {
      argument += optionSize;
      if (*argument == '\0') {
        *index += 1;
        if (*index >= argc) {
          printf("Error: missing required argument for option %s\n", option);
          return false;
        }
        *argumentOption = _strdup(argv[*index]);
      } else {
        *argumentOption = _strdup(argument);
      }
    }
    *index += 1;
    return true;
}

int main(int argc, const char* argv[])
{
  // ====================================================================================
  // Process Command Line Arguments

  int verbose = 1;

  char* model = NULL;
  wchar_t* inputFile = NULL;
  char* entryPoint = NULL;
  char* variableName = NULL;
  char* outputFile = NULL;
  char* defineOption = NULL;
  UINT flags1 = 0;
  UINT cmd = 0;
  size_t dummy = 0;

  int numDefines = 1;
  D3D_SHADER_MACRO* defines = new D3D_SHADER_MACRO[numDefines];
  defines[numDefines-1].Name = NULL;
  defines[numDefines-1].Definition = NULL;

  int index = 1; 
  while (1) {
    D3D_SHADER_MACRO* newDefines;

    /* Detect the end of the options. */
    if (index >= argc)
      break;

    if (parseOpt("nologo", argc, argv, &index, NULL)) {
      continue;
    } else if (parseOpt("T", argc, argv, &index, &model)) {
      if(verbose) {
        printf ("option -T (Shader Model/Profile) with arg '%s'\n", model);
      }
        continue;
    } else if (parseOpt("E", argc, argv, &index, &entryPoint)) {
      if(verbose) {
        printf ("option -E (Entry Point) with arg '%s'\n", entryPoint);
      }
      continue;
    } else if (parseOpt("D", argc, argv, &index, &defineOption)) {
      numDefines++;
      //Copy the old array into the new array, but put the new definition at the beginning
      newDefines = new D3D_SHADER_MACRO[numDefines];
      for(int i=1; i<numDefines; i++)
        newDefines[i] = defines[i-1];
      delete[] defines;
      defines = newDefines;
      defines[0].Name = defineOption;
      defines[0].Definition = "1";
      if(verbose) {
        printf ("option -D with arg %s\n", defineOption);
      }
      continue;
    } else if (parseOpt("Vn", argc, argv, &index, &variableName)) {
      if(verbose) {
        printf ("option -Vn (Variable Name) with arg '%s'\n", variableName);
      }
      continue;
    } else if (parseOpt("Vi", argc, argv, &index, NULL)) {
      if(verbose) {
        printf("option -Vi (Output include process details) acknowledged but ignored.\n");
      }
      continue;
    }  else if (parseOpt("Fh", argc, argv, &index, &outputFile)) {
        if (cmd != 0) {
            printf("You cannot specify both an object and header");
            return 1;
        }
        cmd = CMD_WRITE_HEADER;
        if (verbose) {
            printf("option -Fh (Output File) with arg %s\n", outputFile);
        }
        continue;
    } else if (parseOpt("Fo", argc, argv, &index, NULL)) {
        if (cmd != 0) {
            printf("You cannot specify both an object and header");
            return 1;
        }
        cmd = CMD_WRITE_OBJECT;
    } else if (parseOpt("Fd", argc, argv, &index, NULL)) {
        printf("option -Fd ignored");
        continue;
    } else if (parseOpt("Zi", argc, argv, &index, NULL)) {
        if (verbose) {
            printf("option -Zi D3DCOMPILE_DEBUG\n");
        }
        flags1 = flags1 | D3DCOMPILE_DEBUG;
        continue;
    } else if (parseOpt("Vd", argc, argv, &index, NULL)) {
        if (verbose) {
            printf("option -Zi D3DCOMPILE_SKIP_VALIDATION\n");
        }
        flags1 = flags1 | D3DCOMPILE_SKIP_VALIDATION;
        continue;
    } else if (parseOpt("Vd", argc, argv, &index, NULL)) {
        if (verbose) {
            printf("option -Zi D3DCOMPILE_SKIP_OPTIMIZATION\n");
        }
        flags1 = flags1 | D3DCOMPILE_SKIP_OPTIMIZATION;
        continue;
    } else if (parseOpt("Zpr", argc, argv, &index, NULL)) {
        if (verbose) {
            printf("option -Zpc D3DCOMPILE_PACK_MATRIX_ROW_MAJOR\n");
        }
        flags1 = flags1 | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
        continue;
    } else if (parseOpt("Zpc", argc, argv, &index, NULL)) {
        if (verbose) {
            printf("option -Zpc D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR\n");
        }
        flags1 = flags1 | D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;
        continue;
    } else if (parseOpt("Gpp", argc, argv, &index, NULL)) {
        if (verbose) {
            printf("option -Gpp D3DCOMPILE_PARTIAL_PRECISION\n");
        }
        flags1 = flags1 | D3DCOMPILE_PARTIAL_PRECISION;
        continue;
    }
    else if (parseOpt("Op", argc, argv, &index, NULL)) {
        if (verbose) {
            printf("option -Op D3DCOMPILE_NO_PRESHADER\n");
        }
        flags1 = flags1 | D3DCOMPILE_NO_PRESHADER;
        continue;
    } else if (parseOpt("Gfa", argc, argv, &index, NULL)) {
        if (verbose) {
            printf("option -Gfa D3DCOMPILE_AVOID_FLOW_CONTROL\n");
        }
        flags1 = flags1 | D3DCOMPILE_AVOID_FLOW_CONTROL;
        continue;
    } else if (parseOpt("WX", argc, argv, &index, NULL)) {
        if (verbose) {
            printf("option -WX D3DCOMPILE_WARNINGS_ARE_ERRORS\n");
        }
        flags1 = flags1 | D3DCOMPILE_WARNINGS_ARE_ERRORS;
        continue;
    } else if (parseOpt("Ges", argc, argv, &index, NULL)) {
        if (verbose) {
            printf("option -Ges D3DCOMPILE_ENABLE_STRICTNESS\n");
        }
        flags1 = flags1 | D3DCOMPILE_ENABLE_STRICTNESS;
        continue;
    } else if (parseOpt("Gis", argc, argv, &index, NULL)) {
        if (verbose) {
            printf("option -Gis D3DCOMPILE_IEEE_STRICTNESS\n");
        }
        flags1 = flags1 | D3DCOMPILE_IEEE_STRICTNESS;
        continue;
    } else if (parseOpt("Gec", argc, argv, &index, NULL)) {
        if (verbose) {
            printf("option -Gec D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY\n");
        }
        flags1 = flags1 | D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY;
        continue;
    } else if (parseOpt("O0", argc, argv, &index, NULL)) {
        if (verbose) {
            printf("option -O0 D3DCOMPILE_OPTIMIZATION_LEVEL0\n");
        }
        flags1 = flags1 | D3DCOMPILE_OPTIMIZATION_LEVEL0;
        continue;
    } else if (parseOpt("O1", argc, argv, &index, NULL)) {
        if (verbose) {
            printf("option -O1 D3DCOMPILE_OPTIMIZATION_LEVEL1\n");
        }
        flags1 = flags1 | D3DCOMPILE_OPTIMIZATION_LEVEL1;
        continue;
    } else if (parseOpt("O2", argc, argv, &index, NULL)) {
        if (verbose) {
            printf("option -O1 D3DCOMPILE_OPTIMIZATION_LEVEL2\n");
        }
        flags1 = flags1 | D3DCOMPILE_OPTIMIZATION_LEVEL2;
        continue;
    } else if (parseOpt("O2", argc, argv, &index, NULL)) {
        if (verbose) {
            printf("option -O1 D3DCOMPILE_OPTIMIZATION_LEVEL3\n");
        }
        flags1 = flags1 | D3DCOMPILE_OPTIMIZATION_LEVEL3;
        continue;
    } else if (parseOpt("res_may_alias", argc, argv, &index, NULL)) {
        if (verbose) {
            printf("option -res_may_alias D3DCOMPILE_RESOURCES_MAY_ALIAS\n");
        }
        flags1 = flags1 | D3DCOMPILE_RESOURCES_MAY_ALIAS;
        continue;
    } else if (parseOpt("enable_unbounded_descriptor_tables", argc, argv, &index, NULL)) {
        if (verbose) {
            printf("option -enable_unbounded_descriptor_tables D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES\n");
        }
        flags1 = flags1 | D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES;
        continue;
    } else if (parseOpt("all_resources_bound", argc, argv, &index, NULL)) {
        if (verbose) {
            printf("option -all_resources_bound D3DCOMPILE_ALL_RESOURCES_BOUND\n");
        }
        flags1 = flags1 | D3DCOMPILE_ALL_RESOURCES_BOUND;
        continue;
    }  else if (parseOpt("Zss", argc, argv, &index, NULL)) {
        if (verbose) {
            printf("option -Zss D3DCOMPILE_DEBUG_NAME_FOR_SOURCE\n");
        }
        flags1 = flags1 | D3DCOMPILE_DEBUG_NAME_FOR_SOURCE;
        continue;
    } else if (parseOpt("Zsb", argc, argv, &index, NULL)) {
        if (verbose) {
            printf("option -Zsb D3DCOMPILE_DEBUG_NAME_FOR_BINARY\n");
        }
        flags1 = flags1 | D3DCOMPILE_DEBUG_NAME_FOR_BINARY;
        continue;
    } else if (parseOpt("?", argc, argv, &index, NULL)) {
        print_usage_arg();
        continue;
    } else if (parseOpt("Qstrip_reflect", argc, argv, &index, NULL)) {
        printf("option -Qstrip_reflect ignored");
        continue;
    } else if (parseOpt("Qstrip_debug", argc, argv, &index, NULL)) {
        printf("option -Qstrip_debug ignored");
        continue;
    } else if (parseOpt("version", argc, argv, &index, NULL)) {
        print_version();
        return 0;		
    } else {
      if (!inputFile)
      {
        inputFile = new wchar_t[strlen(argv[index])+1];
        mbstowcs_s(&dummy, inputFile, strlen(argv[index]) + 1, argv[index], strlen(argv[index]) + 1);
        if(verbose) {
          wprintf(L"input file: %ls\n", inputFile);
        }
        index += 1;
      } else {
        print_usage_toomany();
        return 1;
      }
    }
  }

  if(inputFile == NULL)
    print_usage_missing("inputFile");
  if(model == NULL)
    print_usage_missing("model");
  if(entryPoint == NULL)
    print_usage_missing("entryPoint");
  if(defines == NULL)
    print_usage_missing("defines");
  if(outputFile == NULL)
    print_usage_missing("outputFile");

  //Default output variable name
  if (variableName == NULL) {
      const char* prefix = "g";
      for (int i = 0; g_profilePrefixTable[i].name != NULL; i++) {
          if (strcmp(g_profilePrefixTable[i].name, model) == 0) {
              prefix = g_profilePrefixTable[i].prefix;
              break;
          }
      }
      variableName = (char*)malloc(strlen(prefix) + strlen(entryPoint) + 2);
      sprintf_s(variableName, strlen(prefix) + strlen(entryPoint) + 2, "%s_%s", prefix, entryPoint);
  }

  // ====================================================================================
  // Shader Compilation

  HMODULE h = LoadLibrary(DLL_NAME);
  if (h == NULL) {

      //Find the WINDOWS dll
      wchar_t dllPath[MAX_PATH];
      size_t bytes = GetModuleFileName(NULL, dllPath, MAX_PATH);
      if (bytes == 0) {
          printf("Could not retrieve the directory of the running executable.\n");
          return 1;
      }
      //Fill rest of the buffer with NULLs
      memset(dllPath + bytes, '\0', MAX_PATH - bytes);
      //Copy the dll location over top efxc2.exe
      wcscpy_s(dllPath + bytes, MAX_PATH, DLL_NAME);
/*      wcscpy(wcsrchr(dllPath, '\\') + 1, DLL_NAME); */

      HMODULE h = LoadLibrary(dllPath);
      if (h == NULL) {
         wprintf("Error: could not load " DLL_NAME " from %s\n", dllPath);
          return 1;
      }
  }

  pCompileFromFileg ptr = (pCompileFromFileg)GetProcAddress(h, "D3DCompileFromFile");
  if(ptr == NULL) {
    printf("Error: could not get the address of D3DCompileFromFile.\n");
    return 1;
  }

  HRESULT hr;
  ID3DBlob* output = NULL;
  ID3DBlob* errors = NULL;

  if(verbose) {
    printf("Calling D3DCompileFromFile(\n");
    
    wprintf(L"\t %ls,\n", inputFile);
    
    printf("\t");
    for(int i=0; i<numDefines-1; i++)
      printf(" %s=%s", defines[i].Name, defines[i].Definition);
    printf(",\n");

    printf("\t D3D_COMPILE_STANDARD_FILE_INCLUDE,\n");

    printf("\t %s,\n", entryPoint);

    printf("\t %s,\n", model);

    printf("\t " PRIx64 ",\n", flags1);
    printf("\t 0,\n");
    printf("\t &output,\n");
    printf("\t &errors);\n");
  }

  /*
  HRESULT WINAPI D3DCompileFromFile(
  in      LPCWSTR pFileName,
  in_opt  const D3D_SHADER_MACRO pDefines,
  in_opt  ID3DInclude pInclude,
  in      LPCSTR pEntrypoint,
  in      LPCSTR pTarget,
  in      UINT Flags1,
  in      UINT Flags2,
  out     ID3DBlob ppCode,
  out_opt ID3DBlob ppErrorMsgs
  );
  */
  hr = ptr(
    inputFile,
    defines,
    D3D_COMPILE_STANDARD_FILE_INCLUDE,
    entryPoint,
    model,
    flags1,
    0,
    &output,
    &errors
    );

  // ====================================================================================
  // Output (or errors)

  if (FAILED(hr)) {
   if (errors) {
    char* error = (char*)errors->GetBufferPointer();
    printf("Got an error (%li) while compiling:\n%s\n", hr, error);
    errors->Release();
   } else {
     printf("Got an error (%li) while compiling, but no error message from the function.\n", hr);

     LPSTR messageBuffer = nullptr;
     size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
     std::string message(messageBuffer, size);
     LocalFree(messageBuffer);
     printf("Windows Error Message: %s\n", messageBuffer);
   }

   if (output)
     output->Release();

   return hr;
  } else {
    char * outString = (char*)output->GetBufferPointer();
    size_t len = output->GetBufferSize();

    FILE* f;
    errno_t err = fopen_s(&f, outputFile, "w");
    if (err != 0) {
        char errmsg[ERR_SIZE];
        strerror_s(errmsg, ERR_SIZE, errno);
		printf("File open failed: err (%s)\n", errmsg);
		return 1;
    }
    if (cmd == CMD_WRITE_HEADER) {
      fprintf(f, "const signed char %s[] =\n{\n", variableName);
      for (int i = 0; i < len; i++) {
        fprintf(f, "%4i", outString[i]);
        if (i != len - 1)
          fprintf(f, ",");
        if (i % 6 == 5)
          fprintf(f, "\n");
      }

      fprintf(f, "\n};\n");
    }
    if (cmd == CMD_WRITE_OBJECT) {
        fwrite(outString, len, 1, f);
    }
    fclose(f);
    if(verbose) {
#ifdef _WIN64
      printf("Wrote " PRIu64 " bytes of shader output to %s\n", len, outputFile);
#else
      printf("Wrote %u bytes of shader output to %s\n", len, outputFile);
#endif
    }
  }

  return 0;
}
