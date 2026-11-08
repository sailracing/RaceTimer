#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <json.h>   /* libjson header */

int main(void) {
    struct json_object *json;
    struct json_object *json_out;
    struct array_list *foo_list;
    ssize_t a_size;
    int ret, i;
    char *foo;

    json = json_object_from_file("foo.json");

    ret = json_object_object_get_ex(json, "people", &json_out);

    fprintf(stderr, "ret = %i\n", ret);

    foo = json_object_get_string(json);
    
    fprintf(stderr, "foo1 = %s\n", foo);

    foo = json_object_get_string(json_out);
    
    fprintf(stderr, "foo2 = %s\n", foo);

    foo_list = json_object_get_array(json_out);

    a_size = json_object_array_length(json_out);

    fprintf(stderr, "array size = %li\n", a_size);

    for(i = 0; i < a_size; i++){
 //       foo = json_object_get_string(foo_list[i]);
        fprintf(stderr, "index %i - string %s\n", i, foo);
    }

    return 0;
}
