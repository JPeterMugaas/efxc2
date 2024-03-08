//--------------------------------------------------------------------------------------
// File: efxc2Utils.cpp
//
// Copyright (c) J. Peter Mugaas
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//--------------------------------------------------------------------------------------

#include "efxc2Utils.h"

[[noreturn]] void print_errno(errno_t _errno) {
	char errmsg[ERR_SIZE];
#ifdef _WIN32
	strerror_s(errmsg, ERR_SIZE, _errno);
#else  /* _WIN32 */
	/*int strerror_r(int errnum, char *buf, size_t buflen);*/
	strerror_r(errno, errmsg, ERR_SIZE);
#endif /* _WIN32 */
	fprintf(stderr, "%s\n", errmsg);
	exit(1);
}

[[noreturn]] void print_errno(void) {
	print_errno(errno);
}

char* GetFileName(_In_ char* path) {
	if (path == nullptr) {
		return nullptr;
	}
	char* pCur = path;
	char* pFileName = nullptr;

	while (*pCur != '\0') {
		if (*pCur == '/' || *pCur == '\\') {
			pFileName = pCur + 1;
		}
		pCur++;
	}
	if (pFileName != nullptr) {
		if (strcmp(pFileName, "..") == 0) {
			return nullptr;
		}
		if (strcmp(pFileName, ".") == 0) {
			return nullptr;
		}
	}
	return pFileName;
}

#ifdef _WIN32
wchar_t* GetFileName(_In_ wchar_t* path) {
	if (path == nullptr) {
		return nullptr;
	}
	wchar_t* pCur = path;
	wchar_t* pFileName = nullptr;

	while (*pCur != '\0') {
		if (*pCur == '/' || *pCur == '\\') {
			pFileName = pCur + 1;
		}
		pCur++;
	}
	if (pFileName != nullptr) {
		if (wcscmp(pFileName, L"..") == 0) {
			return nullptr;
		}
		if (wcscmp(pFileName, L".") == 0) {
			return nullptr;
		}
	}
	return pFileName;
}
#endif

/*from: https://stackoverflow.com/questions/14002954/c-programming-how-to-read-the-whole-file-contents-into-a-buffer */
/* Size of each input chunk to be
   read and allocate for. */
const size_t  READALL_CHUNK = 262144;

const int  READALL_OK = 0;  /* Success */
const int  READALL_INVALID = -1;  /* Invalid parameters */
const int  READALL_ERROR  = -2;  /* Stream error */
const int  READALL_TOOMUCH = -3;  /* Too much input */
const int  READALL_NOMEM  = -4;  /* Out of memory */

   /* This function returns one of the READALL_ constants above.
	  If the return value is zero == READALL_OK, then:
		(*dataptr) points to a dynamically allocated buffer, with
		(*sizeptr) chars read from the file.
		The buffer is allocated for one extra char, which is NUL,
		and automatically appended after the data.
	  Initial values of (*dataptr) and (*sizeptr) are ignored.
   */
int readall(_In_ FILE* in, _Out_writes_bytes_(*sizeptr) char** dataptr, _Out_opt_ size_t* sizeptr)
{
	char* data = nullptr;
	char* temp;
	size_t size = 0;
	size_t used = 0;
	size_t n;

	/* None of the parameters can be NULL. */
	if (in == nullptr || dataptr == nullptr || sizeptr == nullptr)
		return READALL_INVALID;

	*dataptr = nullptr;
	*sizeptr = 0;

	/* A read error already occurred? */
	if (ferror(in))
		return READALL_ERROR;

	while (TRUE) {
		if (used + READALL_CHUNK + 1 > size) {
			size = used + READALL_CHUNK + 1;

			/* Overflow check. Some ANSI C compilers
			   may optimize this away, though. */
			if (size <= used) {
				free(data);
				return READALL_TOOMUCH;
			}

			temp = (char*)realloc(data, size);
			if (temp == nullptr) {
				free(data);
				return READALL_NOMEM;
			}
			data = temp;
		}

		n = fread(data + used, 1, READALL_CHUNK, in);
		if (n == 0) {
			break;
		}
		used += n;
	}

	if (ferror(in)) {
		free(data);
		return READALL_ERROR;
	}

	temp = (char*)realloc(data, used + 1);
	if (temp == nullptr) {
		free(data);
		return READALL_NOMEM;
	}
	data = temp;
	data[used] = '\0';

	*dataptr = data;
	*sizeptr = used;

	return READALL_OK;
}

#ifdef _WIN32
bool parseOpt(_In_ const wchar_t* option, _In_ int argc, _In_ wchar_t* argv[1], _Inout_ int* index, _Inout_opt_ wchar_t** argumentOption) {
#else  /* _WIN32 */
bool parseOpt(_In_ const char* option, _In_ int argc, _In_ char* argv[1], _Inout_ int* index, _Inout_opt_ char** argumentOption) {
#endif /* _WIN32 */
	assert(option != nullptr);
	if (!index || *index >= argc) {
		return false;
	}
#ifdef _WIN32
	const wchar_t* argument = argv[*index];
#else  /* _WIN32 */
	const char* argument = argv[*index];
#endif /* _WIN32 */
	if (argument[0] == '-' || argument[0] == '/') {
		argument++;
		if (argument[0] == '-') {
			argument++;
		}
	}
	else {
		return false;
	}

#ifdef _WIN32
	size_t optionSize = wcslen(option);
	if (wcsncmp(argument, option, optionSize) != 0) {
#else  /* _WIN32 */
	size_t optionSize = strlen(option);
	if (strncmp(argument, option, optionSize) != 0) {
#endif /* _WIN32 */
		return false;
	}

	if (argumentOption) {
		argument += optionSize;
		if (*argument == '\0') {
			*index += 1;
			if (*index >= argc) {
				fprintf(stderr, "Error: missing required argument for option %ls\n", option);
				return false;
			}
#ifdef _WIN32
			* argumentOption = M_WCSDUP(argv[*index]);
#else  /*_WIN32 */
			* argumentOption = strdup(argv[*index]);
#endif /* _WIN32 */
		}
		else {
#ifdef _WIN32
			* argumentOption = M_WCSDUP(argument);
#else  /* _WIN32 */
			* argumentOption = strdup(argument);
#endif /* _WIN32 */
		}
	}
	*index += 1;
	return true;
}

#ifdef _WIN32
char* wcharToChar(_In_ LPCWSTR w) {
	size_t len = wcslen(w);
	char* c = nullptr;
	c = (char*)malloc(len + 1);
	if (c == nullptr) {
		fprintf(stderr, "malloc failed/n");
		print_errno();
		exit(1);
	}
	memset(c, 0, len + 1);
	size_t dummy = len;
	wcstombs_s(&dummy, c, len + 1, w, len);
	if (errno != 0) {
		fprintf(stderr, "wcstombs failed/n");
		print_errno();
		exit(1);
	}
	return c;
}

void FixupFileName(_Inout_ wchar_t* FileName) {
	if (FileName == nullptr) {
		return;
	}
	for (int i = 0; FileName[i] != '\0'; i++)
	{
		if (FileName[i] == '/') {
			FileName[i] = '\\';
		}
		else {
			continue;
		}
	}
	return;
}

#endif /* _WIN32 */