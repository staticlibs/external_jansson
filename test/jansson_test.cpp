/* 
 * File:   jansson_test.cpp
 * Author: alex
 *
 * Created on February 15, 2015, 7:40 PM
 */

#include <cassert>
#include <cstdlib>
#include <string>

#include "jansson.h"

int main() {
    auto obj = json_object();
    assert(obj);
    auto st1 = json_string("foo");
    assert(st1);
    auto err = json_object_set(obj, "bar", st1);
    assert(!err);
    auto res = json_dumps(obj, 0);
    assert(std::string("{\"bar\": \"foo\"}") == std::string(res));

    json_decref(obj);
    json_decref(st1);
    free(res);

    return 0;
}

