#include "charlist.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "list.h"
#include "../txtsdl_constants.h"

typedef struct _String {
    uchar_t *data;
    int size;
    int capacity;
} String;

String *StringCreate(void) {
    String *string = malloc(sizeof(String));

    if (!string) {
        fprintf(stderr, "Failed to allocate memory for string\n");
        return NULL;
    }

    string->data = malloc(sizeof(uchar_t) * STRING_STARTING_CAPACITY);

    if (!string->data) {
        fprintf(stderr, "Failed to allocate memory for string data\n");
        return NULL;
    }

    string->size = 0;
    string->capacity = STRING_STARTING_CAPACITY;

    return string;
}

String *StringFromCString(const char *str) {
    String *string = StringCreate();

    if (!string) {
        fprintf(stderr, "Failed to create string\n");
        return NULL;
    }

    for (int i = 0; str[i] != '\0'; i++) {
        StringAppend(string, str[i]);
    }

    return string;

}

String *StringFromFileDescriptor(FILE *file) {
    String *string = StringCreate();

    if (!string) {
        fprintf(stderr, "Failed to create string\n");
        return NULL;
    }

    int c;

    while ((c = fgetc(file)) != EOF) {
        StringAppend(string, c);
    }

    return string;
}

String *StringFromFileDescriptorLine(FILE *file) {
    String *string = StringCreate();

    if (!string) {
        fprintf(stderr, "Failed to create string\n");
        return NULL;
    }

    // If C is anything unsigned, it will never be less than 0 and therefore never be EOF
    // So it CANNOT be uchar_t, uint, etc
    // Fuck this bug man.
    int c;

    while ((c = fgetc(file)) != EOF && c != '\n') {
        StringAppend(string, c);
    }

    if (StringSize(string) == 0) {
        StringDestroy(string);
        return NULL;
    }

    return string;
}

List *StringSplit(String *string, uchar_t delimiter) {
    List *list = ListCreate();

    if (!list) {
        fprintf(stderr, "Failed to create list when splitting string\n");
        return NULL;
    }

    String *current = StringCreate();

    if (!current) {
        fprintf(stderr, "Failed to create string when splitting string\n");
        ListDestroy(list);
        return NULL;
    }

    for (int i = 0; i < string->size; i++) {
        if (string->data[i] == delimiter) {
            ListAdd(list, current);
            current = StringCreate();

            if (!current) {
                fprintf(stderr, "Failed to create string when splitting string\n");
                ListIterate(list, (void (*)(void *))StringDestroy);
                ListDestroy(list);
                return NULL;
            }

            continue;
        }

        StringAppend(current, string->data[i]);
    }

    ListAdd(list, current);

    return list;
}

void StringAppend(String *string, uchar_t c) {
    if (string->size == string->capacity) {
        string->capacity *= 2;
        string->data = realloc(string->data, sizeof(uchar_t) * string->capacity);

        if (!string->data) {
            fprintf(stderr, "Failed to reallocate memory for string data\n");
            return;
        }
    }

    string->data[string->size++] = c;
}

void StringRemove(String *string, uchar_t c) {
    for (int i = 0; i < string->size; i++) {
        if (string->data[i] == c) {
            StringRemoveAt(string, i);
            return;
        }
    }
}

void StringRemoveAt(String *string, int index) {
    for (int i = index; i < string->size - 1; i++) {
        string->data[i] = string->data[i + 1];
    }

    string->size--;
}

uchar_t StringGetAt(String *string, int index) {
    if (index < 0 || index >= string->size) {
        fprintf(stderr, "Index out of bounds in string\n");
        exit(EXIT_FAILURE);
    }

    return string->data[index];
}

void StringConcat(String *string, const uchar_t *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        StringAppend(string, str[i]);
    }
}

int StringInsert(String *string, int index, uchar_t c) {
    if (index < 0 || index >= string->size) {
        fprintf(stderr, "Index out of bounds\n");
        return TXTSDL_FAILURE;
    }

    if (string->size == string->capacity) {
        string->capacity *= 2;
        string->data = realloc(string->data, sizeof(uchar_t) * string->capacity);

        if (!string->data) {
            fprintf(stderr, "Failed to reallocate memory for string data\n");
            return TXTSDL_FAILURE;
        }
    }

    for (int i = string->size; i > index; i--) {
        string->data[i] = string->data[i - 1];
    }

    string->data[index] = c;
    string->size++;

    return TXTSDL_SUCCESS;
}

int StringCopy(String *string, String *source) {
    if (string->capacity < source->size) {
        string->capacity = source->size;
        string->data = realloc(string->data, sizeof(uchar_t) * string->capacity);

        if (!string->data) {
            fprintf(stderr, "Failed to reallocate memory for string data\n");
            return TXTSDL_FAILURE;
        }
    }

    string->size = source->size;

    for (int i = 0; i < string->size; i++) {
        string->data[i] = source->data[i];
    }

    return TXTSDL_SUCCESS;
}

bool StringContains(String *string, uchar_t c) {
    for (int i = 0; i < string->size; i++) {
        if (string->data[i] == c) {
            return true;
        }
    }

    return false;
}

bool StringCompareCString(String *string, const char *str) {
    if (string->size != strlen(str)) {
        return false;
    }

    for (int i = 0; i < string->size; i++) {
        if (string->data[i] != str[i]) {
            return false;
        }
    }

    return true;
}

bool StringCompareString(String *string, String *other) {
    if (string->size != other->size) {
        return false;
    }

    for (int i = 0; i < string->size; i++) {
        if (string->data[i] != other->data[i]) {
            return false;
        }
    }

    return true;
}

void StringWriteToFileDescriptor(String *string, FILE *file) {
    for (int i = 0; i < string->size; i++) {
        fputc(string->data[i], file);
    }
}

int StringToInt(String *string) {
    int result = 0;

    for (int i = 0; i < string->size; i++) {
        if (string->data[i] < '0' || string->data[i] > '9') {
            fprintf(stderr, "Invalid character in string\n");
            return TXTSDL_FAILURE;
        }

        result *= 10;
        // This calculates how many places up the character is from 0 on the ascii-table
        // Since it is in order, it will give us the integer value of the number
        result += string->data[i] - '0';
    }

    return result;
}

int StringSize(String *string) {
    return string->size;
}

const uchar_t *StringGetData(String *string) {
    return string->data;
}

void StringDestroy(String *string) {
    free(string->data);
    free(string);
}