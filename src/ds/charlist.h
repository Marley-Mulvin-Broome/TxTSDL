#ifndef TXTSDL_CHARLIST_H
#define TXTSDL_CHARLIST_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "list.h"

#define STRING_STARTING_CAPACITY 50

/**
 * A character that stores utf-16
*/
typedef uint16_t uchar_t;

/**
 * A string of utf-16 characters
*/
typedef struct _String String;

String *StringCreate(void);
String *StringFromCString(const char *str);
String *StringFromFileDescriptor(FILE *file);
String *StringFromFileDescriptorLine(FILE *file);
List *StringSplit(String *string, uchar_t delimiter);
void StringSetAt(String *string, int index, uchar_t c);
void StringAppend(String *string, uchar_t c);
void StringRemove(String *string, uchar_t c);
void StringRemoveAt(String *string, int index);
uchar_t StringGetAt(String *string, int index);
void StringConcat(String *string, const uchar_t *str);
int StringInsert(String *string, int index, uchar_t c);

/**
 * Copies the contents of the source string to the destination string.
 * @param string The destination string.
 * @param source The source string.
 * @returns 0 if successful, -1 if not.
*/
int StringCopy(String *string, String *source);
bool StringContains(String *string, uchar_t c);
bool StringCompare(String *string, const uchar_t *str);
bool StringCompareCString(String *string, const char *str);
bool StringCompareString(String *string, String *other);
void StringWriteToFileDescriptor(String *string, FILE *file);
int StringToInt(String *string);
int StringSize(String *string);
const uchar_t *StringGetData(String *string);
void StringDestroy(String *string);


#endif