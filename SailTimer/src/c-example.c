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
    struct json_object_iterator *it;
    struct json_object_iterator *itEnd;
    ssize_t a_size;
    int ret, i;
    char *foo;

    json = json_object_from_file("./mode-files/mode-library.json");

    ret = json_object_object_get_ex(json, "current mode", &json_out);
    ret = json_object_get_int(json_out);
    fprintf(stderr, "current mode = %i\n", ret);

    ret = json_object_object_get_ex(json, "mode descriptors", &json_out);

    printf("ret from Mode descriptors search = %i\n", ret);

    it = json_object_iter_begin(json_out);

    printf("begin = %li\n", it);

    itEnd = json_object_iter_end(json_out);

    printf("end = %li\n", itEnd);

    while (!json_object_iter_equal(it, itEnd)) {
        printf("item name = %s\n", json_object_iter_peek_name(&it));
        json_object_iter_next(&it);
    }

    return 0;

}
