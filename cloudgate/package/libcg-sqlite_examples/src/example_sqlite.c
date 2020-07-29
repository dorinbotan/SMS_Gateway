/* EXAMPLE:
 *  cg_ui
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <libcg/cg_general.h>
#include <libcg/cg_ui.h>
#include <libcg/cg_conf.h>
#include <json-c/json.h>
#include <sqlite3.h>

/* The column indices for TBL_NAMES */
#define ID		0
#define NAME	1

const char *db_name = "/tmp/example_sqlite.db";

/* id points to the ROWID column and will be auto incremented */
const char *create_tbl_names = "CREATE TABLE TBL_NAMES(" \
	"id INTEGER PRIMARY KEY, "\
	"name TEXT "\
	");";

/*******************************************************************************
 * Callback function for sqlite. This function is called for each record in a
 * SELECT sql query.
 * Parameters:
 * 		context: the last parameter in sqlite3_exec
 * 		argc: number of columns
 * 		argv: array of strings representing the fields in the row
 * 		colName: array os strings representing the column names
 ******************************************************************************/
static int sql_callback(void *context, int argc, char **argv, char **colName)
{
	/* Let's build the json data */
	/* [ {id: 1, name: "James"}, {id: 2, name: "John"}, ... ] */
	json_object *obj = NULL;
	json_object *record = NULL;
	json_object *id = NULL;
	json_object *name = NULL;

	if (context) {

		obj = (json_object *)context;

		/* Create a new json object to represent a record */
		record = json_object_new_object();

		/* Get the id and name */
		id = json_object_new_int((int)strtol(argv[ID], NULL, 10));
		name = json_object_new_string(argv[NAME]);

		/* Add these objects to the record object */
		json_object_object_add(record, "id", id);
		json_object_object_add(record, "name", name);

		/* Add the record object to the root object array */
		json_object_array_add(obj, record);

	}

	return SQLITE_OK;
}

/******************************************************************************
 * Function: createDatabase
 * Description: create a new sqlite database with one table TBL_NAMES.
 * Return: 0 success, otherwise 1.
 ******************************************************************************/
int createDatabase() {
	sqlite3 *db;
	int ret;
	int result = 0;
	char *errMsg = NULL;

	/* Create the database */
	if (sqlite3_open(db_name, &db)) {
		printf("Error opening database %s\n", db_name);
		sqlite3_close(db);
		return 1;
	}

	/* Create the table TBL_NAMES */
	ret = sqlite3_exec(db, create_tbl_names, NULL, NULL, &errMsg);

	if (ret != SQLITE_OK) {
		printf("Error creating table: %s\n", errMsg);
		result = 1;
	} else {
		printf("New table created\n");
		result = 0;
	}

	sqlite3_free(errMsg);

	/* Close the database */
	sqlite3_close(db);

	return result;
}

/******************************************************************************
 * Function: saveData
 * Description: insert one new name into the table TBL_NAMES.
 * Return: 0 success, otherwise 1.
 ******************************************************************************/
int saveData(char *name)
{
	sqlite3 *db;
	char sql[200];
	int ret;
	int result = 0;
	char *errMsg = NULL;

	if (!name)
		return 1;

	/* Create the query */
	snprintf(
		sql,
		sizeof(sql) - 1,
		"INSERT INTO TBL_NAMES (name) VALUES ('%s');",
		name
	);

	/* Open the database */
	if (sqlite3_open(db_name, &db)) {
		/* Error opening database */
		sqlite3_close(db);
		return 1;
	}

	ret = sqlite3_exec(db, sql, NULL, NULL, &errMsg);

	if (ret != SQLITE_OK) {
		result = 1;
	} else {
		result = 0;
	}

	sqlite3_free(errMsg);

	/* Close the database */
	sqlite3_close(db);

	return result;
}

/******************************************************************************
 * Function: readData
 * Description: get all the data from TBL_NAMES. This data will be available
 * in a json object array.
 * The json object: [ {id: 1, name: "James"}, {id: 2, name: "John"}, ... ]
 * Return: NULL an error occured, json object array on success
 ******************************************************************************/
json_object * readData() {
	sqlite3 *db;
	json_object *root = NULL;
	char *errMsg = NULL;
	char sql[200] = "SELECT * FROM TBL_NAMES;";
	int ret;

	/* Open the database */
	if (sqlite3_open(db_name, &db)) {
		/* Error opening the database */
		goto out;
	}

	/* Create a new json object array for our data */
	if ((root = json_object_new_array()) == NULL) {
		/* Error creating the json object */
		goto out;
	}

	/*
	 * Execute the query.
	 * Now we use the sql_callback to fill the json array with our data.
	 */
	ret = sqlite3_exec(db, sql, sql_callback, root, &errMsg);

	if (ret != SQLITE_OK) {
		/* An error occured when executing the query */
	} else {
		/* The data is now available in  the root json object  */
	}

	sqlite3_free(errMsg);

out:
	sqlite3_close(db);
	return root;
}

static char *demo_callback(char *json_data, int logged_in, void *context)
{
	json_object *obj;
	json_object *objMyName;
	json_object *root;
	char myName[100];
	char *json_string = NULL;

	/* Process only if we are logged in */
	if (logged_in) {
		/* When json data isn't NULL, parse the data */
		if (json_data) {
			obj = json_tokener_parse(json_data);
			objMyName = json_object_object_get(obj, "myName");
			if (objMyName && json_object_is_type(objMyName, json_type_string)) {
				/* Copy the json string value */
				strncpy(myName, json_object_get_string(objMyName), sizeof(myName) - 1);
				/* Save the data to the database table */
				if (saveData(myName) == 0) {
					/* Successfully saved the data */
					/* Get all the data from the database table TBL_NAMES */
					if ((root = readData()) != NULL) {
						/*
						 * Convert the data to a json string. Use strdup to allocate a new char *.
						 * This pointer is freed automatically by the SDK.
						 */
						json_string = strdup((char *)json_object_to_json_string(root));
						json_object_put(root);
					}
				}
			}
			json_object_put(obj);
			json_object_put(objMyName);
		}
	}

	if (!json_string) {
		asprintf(&json_string, "{}");
	}

	return json_string;
}

int
main (void)
{
	cg_status_t cg_status;
	int32_t level;
	char command[100];
	struct stat sstat;
	char *buffer = NULL;
	int size = 100;

	cg_init("example_sqlite");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	/* Remove the database file */
	if (stat(db_name, &sstat) == 0) {
		printf("Removing existing database\n");
		snprintf(command, sizeof(command) - 1, "rm %s", db_name);
		system(command);
	}

	/* Check if the symlink in /www already exists */
	buffer = malloc(sizeof(char) * size);

	if (readlink("/www/demo", buffer, size) < 0) {
		system("uci set m2mweb.demo=default");
		system("uci set m2mweb.demo.path=/rom/mnt/cust/examples");
		system("uci commit m2mweb");
		printf("You have to reboot to apply the new symlink. Restart this example after rebooting\n");
	} else {

		if (createDatabase()) {
			printf("Error creating database. Exiting ... \n");
			goto faulty;
		}

		cg_status = cg_ui_register_json_callback("example_sqlite", &demo_callback, "The Context");
		if(cg_status != CG_STATUS_OK){
			printf("Error register callback\n");
			goto faulty;
		}

		cg_status = cg_ui_register_page("example_sqlite", "demo/example_sqlite.html");
		if(cg_status != CG_STATUS_OK){
			printf("Error registering example_sqlite\n");
			goto faulty;
		}

		printf("Press any key to continue. Example_sqlite  will be deregistered\n");
		getchar();

		cg_status = cg_ui_deregister_page("example_sqlite");
		if(cg_status != CG_STATUS_OK){
			printf("Error deregistering example_sqlite\n");
			goto faulty;
		}

		cg_status = cg_ui_deregister_json_callback("example_sqlite");
		if(cg_status != CG_STATUS_OK){
			printf("Error deregister JSON callback example_sqlite\n");
			goto faulty;
		}

		printf("Press any key to exit.");
		getchar();

	}

faulty:
	free(buffer);
	cg_deinit();

	return 0;
}
