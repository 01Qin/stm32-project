
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "jsonParser.h"

#include "json-maker.h"

#include "alldata.h"

// API Command: Lists commands.
static const char* CMD_help = "help";
//// API Command: Lists hardware revisions, device UUID, software version.
//static const char* CMD_version = "version";
// API Command: ListAll all the data
static const char* CMD_list_all = "listall";
// API Command: SetOne data
static const char* CMD_set_one = "setone";


enum { MAX_FIELDS = 6 };
json_t pool[ MAX_FIELDS ];
char g_magic_val[8];

/*	@brief	Lowercases string array
 * 	@param1 Pointer to string array
 * 	@param2 maximum number of characters
 *	@return	None
 */
void toLowercaseN(char* word,int n){
	int j = strnlen(word,n);
	for(int i = 0;i<j;i++){
		word[i] = tolower(word[i]);
	}
}

static void createResponse(char* p, size_t* rem_length, ParsingResult type) {
    const char* response;

    switch (type) {
    case SUCCESS_PARSING:
    case SUCCESS_RESET:
    case SUCCESS_INFINITE_LOOP:
        response = "success";
        break;
    case ERROR_UNSUPPORTED:
        response = "unsupported";
        break;
    case ERROR_FORMAT:
        response = "format error";
        break;
    case ERROR_TYPE:
        response = "type error";
        break;
    case ERROR_VALUE:
        response = "value error";
        break;
    case ERROR_VALUE_MINIMUM:
        response = "value less than minimum";
        break;
    case ERROR_VALUE_MAXIMUM:
        response = "value more than maximum";
        break;
    case ERROR_NOT_FOUND:
        response = "not found";
        break;
    case ERROR_FAILED:
        response = "failed";
        break;
    default:
        response = "unknown error";
    }

    p = json_objOpen(p, NULL, rem_length);
    p = json_str(p, "command", response, rem_length);
    p = json_objClose(p, rem_length);
    p = json_end(p, rem_length);
}

//static char* addStrId(char* p, size_t* rem_length, const char* id, const char* value, const char* unit) {
//    p = json_objOpen(p, NULL, rem_length);
//    p = json_str(p, "id",id, rem_length);
//    p = json_str(p, "value", value, rem_length);
//    p = json_str(p, "unit", unit, rem_length);
//    p = json_objClose(p, rem_length);
//    return p;
//}

static char* addIntId(char* p, size_t* rem_length, const char* id, int value, const char* unit) {
    p = json_objOpen(p, NULL, rem_length);
    p = json_str(p, "id", id,rem_length);
    p = json_int(p, "value", value, rem_length);
    p = json_str(p, "unit", unit, rem_length);
    p = json_objClose(p, rem_length);
    return p;
}


//static char* addBoolId(char* p, size_t* rem_length, const char* id, bool value, const char* unit) {
//    p = json_objOpen(p, NULL, rem_length);
//    p = json_str(p, "id", id, rem_length);
//    p = json_bool(p, "value", value, rem_length);
//    p = json_str(p, "unit", unit, rem_length);
//    p = json_objClose(p, rem_length);
//    return p;
//}



//static char* addNullId(char* p, size_t* rem_length, const char* id, const char* unit) {
//    p = json_objOpen(p, NULL, rem_length);
//    p = json_str(p, "id", id, rem_length);
//    p = json_null(p, "value", rem_length);
//    p = json_str(p, "unit", unit, rem_length);
//    p = json_objClose(p, rem_length);
//    return p;
//}


int getHelp(char* p, size_t* rem_length) {
    p = json_objOpen(p, NULL, rem_length);
    p = json_arrOpen(p, "commands", rem_length);

    p = json_str(p, NULL, CMD_help, rem_length);
    p = json_str(p, NULL, CMD_list_all, rem_length);
    p = json_str(p, NULL, CMD_set_one, rem_length);

    p = json_arrClose(p, rem_length);
    p = json_objClose(p, rem_length);
    p = json_end(p , rem_length);

	return IGNORE;
}

/**
 * @brief	jsonParse function that sets or prints asked info from Android
 * Function to parse JSON formatted command from android or customer device
 * Responds in JSON format to all communication, even to wrongly formatted or
 * incomplete commands
 *
 * @param	string input which to parse, \0 or NULL terminated
 * @return	none
 */
ParsingResult jsonParse(char* input, char* responseBuffer, const size_t responseBufferLength) {
	int ret = -1;

	size_t length = responseBufferLength;
	size_t* rem_length = &length;
	char* p = responseBuffer;

	// Check if the string contains json object start, and assume it is json.
	// Then Jsonparser can handle format and incomplete json data structs.
	char* isJson = strchr(input, '{');

	if(isJson == NULL){
		ret = ERROR_FORMAT;
		goto exit_jsonParse;
	}

	json_t const* json = json_create(input, pool, MAX_FIELDS);

	if(json == NULL){
		// JSON is empty or doesn't close correctly.
		ret = ERROR_FORMAT;
		goto exit_jsonParse;
	}

	json_t const* commandField = json_getProperty(json, "command");

	if (commandField == NULL && json_getType(commandField) != JSON_TEXT){
		ret = ERROR_VALUE;
		goto exit_jsonParse;
	}
	char commandValue[64] = {0};
	strncpy(commandValue, json_getValue(commandField), sizeof(commandValue) - 1);

	toLowercaseN(commandValue,64);
	/*-------------------------- CALL HANDLER -----------------------*/
	/* All calls should use only pointers p and rem_length, not the exact values
	 * of rem_length
	 *
	 * Calls should return -1 if they have added a response to p to send back.
	 * 0 if it should return command success
	 * positive integers for errors from enum
	 *
	 * Easier command flow and single end point.
	 */
    if (strcmp(commandValue, CMD_help) == 0) {
        ret = getHelp(p, rem_length);
    }
    else if (strcmp(commandValue, CMD_list_all) == 0) {
        ret = getAll(p, rem_length);
    }
    else if (strcmp(commandValue, CMD_set_one) == 0) {
        ret = setOne(p, rem_length, json);
    }
    else {
//        debugPrintf("Unsupported json command type\r\n");
        ret = ERROR_UNSUPPORTED;
    }

	/*------------------------- CALL HANDLER --------------------------*/
    exit_jsonParse:
	/* EXIT HANDLER*/

	if (ret == IGNORE) {
		// Fixes the misuse of return types by clearing input.
		ret = SUCCESS_PARSING;
	} else {
		memset(responseBuffer, 0, responseBufferLength);
		*rem_length = responseBufferLength;
		createResponse(responseBuffer, rem_length, ret);
	}

	strcat(responseBuffer, "\r\n");

	return ret;
}


int getAll(char* p, size_t* rem_length) {
    p = json_objOpen(p, NULL, rem_length);
    p = json_arrOpen(p, "all", rem_length);


    p = addIntId(p, rem_length, "pwm0", allData.pwmValues[0], "%");
    p = addIntId(p, rem_length, "pwm1", allData.pwmValues[1], "%");
    p = addIntId(p, rem_length, "pwm2", allData.pwmValues[2], "%");

    p = addIntId(p, rem_length, "freq0", allData.freqValues[0], "Hz");
    p = addIntId(p, rem_length, "freq1", allData.freqValues[1], "Hz");
    p = addIntId(p, rem_length, "freq2", allData.freqValues[2], "Hz");

    p = addIntId(p, rem_length, "T1", allData.tempValues[0], "'C");
    p = addIntId(p, rem_length, "T2", allData.tempValues[1], "'C");
    p = addIntId(p, rem_length, "T3", allData.tempValues[2], "'C");
    p = addIntId(p, rem_length, "T4", allData.tempValues[3], "'C");
    p = addIntId(p, rem_length, "T5", allData.tempValues[4], "'C");
    p = addIntId(p, rem_length, "T6", allData.tempValues[5], "'C");
    p = addIntId(p, rem_length, "T7", allData.tempValues[6], "'C");
    p = addIntId(p, rem_length, "T8", allData.tempValues[7], "'C");

    p = addIntId(p, rem_length, "D01", allData.digitOutValues[0], "b");
    p = addIntId(p, rem_length, "D02", allData.digitOutValues[1], "b");
    p = addIntId(p, rem_length, "D03", allData.digitOutValues[2], "b");
    p = addIntId(p, rem_length, "D04", allData.digitOutValues[3], "b");
    p = addIntId(p, rem_length, "D05", allData.digitOutValues[4], "b");
    p = addIntId(p, rem_length, "D06", allData.digitOutValues[5], "b");
    p = addIntId(p, rem_length, "D07", allData.digitOutValues[6], "b");
    p = addIntId(p, rem_length, "D08", allData.digitOutValues[7], "b");

    p = addIntId(p, rem_length, "D09", allData.digitOutValues[8], "b");
    p = addIntId(p, rem_length, "D10", allData.digitOutValues[9], "b");
    p = addIntId(p, rem_length, "D11", allData.digitOutValues[10], "b");
    p = addIntId(p, rem_length, "D12", allData.digitOutValues[11], "b");
    p = addIntId(p, rem_length, "D13", allData.digitOutValues[12], "b");
    p = addIntId(p, rem_length, "D14", allData.digitOutValues[13], "b");
    p = addIntId(p, rem_length, "D15", allData.digitOutValues[14], "b");
    p = addIntId(p, rem_length, "D16", allData.digitOutValues[15], "b");

    p = json_arrClose(p, rem_length);
    p = json_objClose(p, rem_length);
    p = json_end(p, rem_length);

    return IGNORE;
}

int setOne(char *p, size_t *rem_length, json_t const *json) {
    (void) p;
    (void) rem_length;
    json_t const *nameField = json_getProperty(json, "name");
    json_t const *valueField = json_getProperty(json, "value");
    const char *name = json_getValue(nameField);

    if (nameField == NULL || valueField == NULL) {
        return ERROR_FORMAT;
    }

    if(json_getType(valueField) == JSON_INTEGER) {
    	if (strcmp(name, "t1") == 0){
			allData.tempValues[0] = json_getInteger(valueField);
			return SUCCESS_PARSING;
		} else if (strcmp(name, "t2") == 0){
			allData.tempValues[1] = json_getInteger(valueField);
			return SUCCESS_PARSING;
		} else if (strcmp(name, "t3") == 0){
			allData.tempValues[2] = json_getInteger(valueField);
			return SUCCESS_PARSING;
		} else if (strcmp(name, "t4") == 0){
			allData.tempValues[3] = json_getInteger(valueField);
			return SUCCESS_PARSING;
		} else if (strcmp(name, "t5") == 0){
			allData.tempValues[4] = json_getInteger(valueField);
			return SUCCESS_PARSING;
		} else if (strcmp(name, "t6") == 0){
			allData.tempValues[5] = json_getInteger(valueField);
			return SUCCESS_PARSING;
		} else if (strcmp(name, "t7") == 0){
			allData.tempValues[6] = json_getInteger(valueField);
			return SUCCESS_PARSING;
		} else if (strcmp(name, "t8") == 0){
			allData.tempValues[7] = json_getInteger(valueField);
			return SUCCESS_PARSING;
		} else if (strcmp(name, "d01") == 0){
			allData.digitOutValues[0] = json_getInteger(valueField);
			return SUCCESS_PARSING;
		} else if (strcmp(name, "d02") == 0){
			allData.digitOutValues[1] = json_getInteger(valueField);
			return SUCCESS_PARSING;
		} else if (strcmp(name, "d03") == 0){
			allData.digitOutValues[2] = json_getInteger(valueField);
			return SUCCESS_PARSING;
		} else if (strcmp(name, "d04") == 0){
			allData.digitOutValues[3] = json_getInteger(valueField);
			return SUCCESS_PARSING;
		} else if (strcmp(name, "d05") == 0){
			allData.digitOutValues[4] = json_getInteger(valueField);
			return SUCCESS_PARSING;
		} else if (strcmp(name, "d06") == 0){
			allData.digitOutValues[5] = json_getInteger(valueField);
			return SUCCESS_PARSING;
		} else if (strcmp(name, "d07") == 0){
			allData.digitOutValues[6] = json_getInteger(valueField);
			return SUCCESS_PARSING;
		} else if (strcmp(name, "d08") == 0){
			allData.digitOutValues[7] = json_getInteger(valueField);
			return SUCCESS_PARSING;
		} else if (strcmp(name, "d09") == 0){
			allData.digitOutValues[8] = json_getInteger(valueField);
			return SUCCESS_PARSING;
		} else if (strcmp(name, "d10") == 0){
			allData.digitOutValues[9] = json_getInteger(valueField);
			return SUCCESS_PARSING;
		} else if (strcmp(name, "d11") == 0){
			allData.digitOutValues[10] = json_getInteger(valueField);
			return SUCCESS_PARSING;
		} else if (strcmp(name, "d12") == 0){
			allData.digitOutValues[11] = json_getInteger(valueField);
			return SUCCESS_PARSING;
		} else if (strcmp(name, "d13") == 0){
			allData.digitOutValues[12] = json_getInteger(valueField);
			return SUCCESS_PARSING;
		} else if (strcmp(name, "d14") == 0){
			allData.digitOutValues[13] = json_getInteger(valueField);
			return SUCCESS_PARSING;
		} else if (strcmp(name, "d15") == 0){
			allData.digitOutValues[14] = json_getInteger(valueField);
			return SUCCESS_PARSING;
		} else if (strcmp(name, "d16") == 0){
			allData.digitOutValues[15] = json_getInteger(valueField);
			return SUCCESS_PARSING;
		} else {
			return ERROR_NOT_FOUND;
		}

    }

    return ERROR_VALUE;
}
