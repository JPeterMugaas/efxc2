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

void print_errno() {
	char errmsg[ERR_SIZE];
#ifdef _WIN32
	strerror_s(errmsg, ERR_SIZE, errno);
#else
	/*int strerror_r(int errnum, char *buf, size_t buflen);*/
	strerror_r(errno, errmsg, ERR_SIZE);
#endif
	fprintf(stderr, "%s\n", errmsg);
	exit(1);
}

/*from: https://stackoverflow.com/questions/14002954/c-programming-how-to-read-the-whole-file-contents-into-a-buffer */
/* Size of each input chunk to be
   read and allocate for. */
#ifndef  READALL_CHUNK
#define  READALL_CHUNK  262144
#endif

#define  READALL_OK          0  /* Success */
#define  READALL_INVALID    -1  /* Invalid parameters */
#define  READALL_ERROR      -2  /* Stream error */
#define  READALL_TOOMUCH    -3  /* Too much input */
#define  READALL_NOMEM      -4  /* Out of memory */

   /* This function returns one of the READALL_ constants above.
	  If the return value is zero == READALL_OK, then:
		(*dataptr) points to a dynamically allocated buffer, with
		(*sizeptr) chars read from the file.
		The buffer is allocated for one extra char, which is NUL,
		and automatically appended after the data.
	  Initial values of (*dataptr) and (*sizeptr) are ignored.
   */
int readall(FILE* in, char** dataptr, size_t* sizeptr)
{
	char* data = NULL, * temp;
	size_t size = 0;
	size_t used = 0;
	size_t n;

	/* None of the parameters can be NULL. */
	if (in == NULL || dataptr == NULL || sizeptr == NULL)
		return READALL_INVALID;

	/* A read error already occurred? */
	if (ferror(in))
		return READALL_ERROR;

	while (1) {
		if (used + READALL_CHUNK + 1 > size) {
			size = used + READALL_CHUNK + 1;

			/* Overflow check. Some ANSI C compilers
			   may optimize this away, though. */
			if (size <= used) {
				free(data);
				return READALL_TOOMUCH;
			}

			temp = (char*)realloc(data, size);
			if (temp == NULL) {
				free(data);
				return READALL_NOMEM;
			}
			data = temp;
		}

		n = fread(data + used, 1, READALL_CHUNK, in);
		if (n == 0)
			break;

		used += n;
	}

	if (ferror(in)) {
		free(data);
		return READALL_ERROR;
	}

	temp = (char*)realloc(data, used + 1);
	if (temp == NULL) {
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
bool parseOpt(const wchar_t* option, int argc, wchar_t* argv[1], int* index, wchar_t** argumentOption) {
#else
bool parseOpt(const char* option, int argc, char* argv[1], int* index, char** argumentOption) {
#endif
	assert(option != NULL);
	if (!index || *index >= argc) {
		return false;
	}
#ifdef _WIN32
	const wchar_t*
#else
	const char*
#endif
		argument = argv[*index];
		if (argument[0] == '-' || argument[0] == '/') {
			argument++;
			if (argument[0] == '-') {
				argument++;
			}
		}
		else
			return false;

#ifdef _WIN32
		size_t optionSize = wcslen(option);
		if (wcsncmp(argument, option, optionSize) != 0) {
#else
		size_t optionSize = strlen(option);
		if (strncmp(argument, option, optionSize) != 0) {
#endif
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
#else
				* argumentOption = strdup(argv[*index]);
#endif
			}
			else {
#ifdef _WIN32
				* argumentOption = M_WCSDUP(argument);
#else
				* argumentOption = strdup(argument);
#endif
			}
		}
		*index += 1;
		return true;
}

char* wcharToChar(const wchar_t* w) {
	size_t len = wcslen(w);
	char* c = NULL;
	c = (char*)malloc(len + 1);
	if (c == NULL) {
		fprintf(stderr, "malloc failed/n");
		print_errno();
		exit(1);
	}
	memset(c, 0, len + 1);
	wcstombs(c, w, len);
	if (errno != 0) {
		fprintf(stderr, "wcstombs failed/n");
		print_errno();
		exit(1);
	}
	return c;
}

/*from: https://stackoverflow.com/questions/5309471/getting-file-extension-in-c */
#ifdef _WIN32
const wchar_t* FileSuffix(const wchar_t path[])
#else
const char* FileSuffix(const char path[])
#endif
{
#ifdef _WIN32
	const wchar_t* result;
#else
	const char* result;
#endif
	size_t i, n;

	assert(path != NULL);
#ifdef _WIN32
	n = wcslen(path);
#else
	n = strlen(path);
#endif
	i = n - 1;
	while ((i > 0) && (path[i] != '.') && (path[i] != '/') && (path[i] != '\\')) {
		i--;
	}
	if ((i > 0) && (i < n - 1) && (path[i] == '.') && (path[i - 1] != '/') && (path[i - 1] != '\\')) {
		result = path + i;
	}
	else {
		result = path + n;
	}
	return result;
}

#ifdef _WIN32
bool IsFileType(const wchar_t path[], const wchar_t _type[]) {
	if (path == NULL) {
		return false;
	}
	if (_type == NULL) {
		return false;
	}
	const wchar_t* _ext = FileSuffix(path);
	size_t len1 = wcslen(_ext);
	size_t len2 = wcslen(_type);
	if (len1 != len2) {
		return false;
	}
	if (wcscmp(_type, _ext) == 0) {
		return true;
	} else {
		return false;
	}

#else  /* _WIN32 */
bool IsFileType(const char path[], const char _type[]) {
	if (path == NULL) {
		return false;
}
	if (_type == NULL) {
		return false;
	}
	const char* _ext = FileSuffix(path);
	size_t len1 = strlen(_ext);
	size_t len2 = strlen(_type);
	if (len1 != len2) {
		return false;
	}
	if (strcmp(_type, _ext) == 0) {
		return true;
	}
	else {
		return false;
	}
#endif /* _WIN32 */
}

#ifdef _WIN32
void FixupFileName(wchar_t* FileName) {
	size_t i;
	if (FileName == NULL) {
		return;
	}
	for (i = 0; FileName[i] != '\0'; i++)
	{
		if (FileName[i] == '/') {
			FileName[i] = '\\';
		}
		else
			continue;
	}
	return;
}
#endif