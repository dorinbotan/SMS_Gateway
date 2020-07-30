#include "json.h"
#include <json.h>
#include <json_tokener.h>
#include <json_visit.h>

// static int clear_serializer(json_object *jso, int flags,
//                             json_object *parent_jso,
//                             const char *jso_key,
//                             size_t *jso_index, void *userarg)
// {
//     if (jso)
//         json_object_set_serializer(jso, NULL, NULL, NULL);
//     return JSON_C_VISIT_RETURN_CONTINUE;
// }

// static void do_clear_serializer(json_object *jso)
// {
//     json_c_visit(jso, 0, clear_serializer, NULL);
// }

const char * getKeyFromValue(const char *json, const char *key)
{
    json_object *new_obj = json_tokener_parse(json);
    // do_clear_serializer(new_obj);

    json_object *value = json_object_object_get(new_obj, key);
    return json_object_to_json_string(value);
}
