#ifndef JSONPARSER_H_
#define JSONPARSER_H_

#ifdef __cplusplus
extern "C" {
#endif

#define PACKET_SIZE 1024*2

#include "tiny-json.h"

typedef enum {
    IGNORE = -1,
    SUCCESS_PARSING,
    SUCCESS_RESET,
    SUCCESS_INFINITE_LOOP,
    ERROR_UNSUPPORTED,
    ERROR_FORMAT,
    ERROR_TYPE,
    ERROR_VALUE,
    ERROR_VALUE_MINIMUM,
    ERROR_VALUE_MAXIMUM,
    ERROR_NOT_FOUND,
    ERROR_FAILED,
    ERROR_DEPRECATED,
    ERROR_UNKNOWN,
} ParsingResult;

/* @brief	jsonParse function that sets or prints asked info from Android
 * Function to parse JSON formatted command from android or customer device
 * Responds in JSON format to all communication, even to wrongly formatted or
 * incomplete commands
 *
 * @param input input which to parse, \0 terminated.
 * @param responseBuffer a buffer sized of responseBufferLength where to format the response.
 * @param responseBufferLength the size of the responseBuffer.
 * @return ParsingResult
 */
ParsingResult jsonParse(char* input, char* responseBuffer, const size_t responseBufferLength);


int getAll(char* p, size_t* rem_length);

int setOne(char *p, size_t *rem_length, json_t const *json);

#ifdef __cplusplus
	}
#endif

#endif
