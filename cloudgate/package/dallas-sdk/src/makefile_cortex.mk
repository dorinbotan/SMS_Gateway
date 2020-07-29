#Dallas-sdk application
DALLAS_SDK_APP_SOURCE_PATH=apps/tstfind

DALLAS_SDK_APP_OBJ= $(DALLAS_SDK_APP_SOURCE_PATH)/tstfind.o

DALLAS_SDK_APP_TEMP_SOURCE_PATH=apps/temp

DALLAS_SDK_APP_TEMP_OBJ= $(DALLAS_SDK_APP_TEMP_SOURCE_PATH)/temp.o

# Dallas-sdk common/ objects
# Add here files from common directory 
DALLAS_SDK_COMMON_SOURCE_PATH=common

DALLAS_SDK_COMMON_OBJ= $(DALLAS_SDK_COMMON_SOURCE_PATH)/crcutil.o $(DALLAS_SDK_COMMON_SOURCE_PATH)/ioutil.o $(DALLAS_SDK_COMMON_SOURCE_PATH)/owerr.o $(DALLAS_SDK_COMMON_SOURCE_PATH)/temp10.o $(DALLAS_SDK_COMMON_SOURCE_PATH)/findtype.o 

# Dallas-sdk lib/general/shared/ objects
# Add here files from lib/general/shared/ directory 
DALLAS_SDK_SHARED_SOURCE_PATH=lib/general/shared

DALLAS_SDK_SHARED_OBJ= $(DALLAS_SDK_SHARED_SOURCE_PATH)/ownet.o $(DALLAS_SDK_SHARED_SOURCE_PATH)/owtran.o

#*************************************************************

OBJS= $(DALLAS_SDK_COMMON_OBJ) $(DALLAS_SDK_SHARED_OBJ) $(DALLAS_SDK_APP_OBJ)
OBJS_TEMP= $(DALLAS_SDK_COMMON_OBJ) $(DALLAS_SDK_SHARED_OBJ) $(DALLAS_SDK_APP_TEMP_OBJ) 

$(OW_APP_FILE_NAME): $(OBJS)
	$(CC) $(CFLAGS) -Wall -o $@ $^ $(OWIRE_LDFLAGS) -L. -lowirecortex

$(OW_APP_FILE_TEMP_NAME): $(OBJS_TEMP)
	$(CC) $(CFLAGS)  -Wall -o $@ $^ $(OWIRE_LDFLAGS) -L. -lowirecortex

application_example: $(OW_APP_FILE_NAME) $(OW_APP_FILE_TEMP_NAME)

clean:
	rm -f $(OW_APP_FILE_NAME) $(OW_APP_FILE_TEMP_NAME) $(LIB_OWCORTEX_FILENAME) *.o 

