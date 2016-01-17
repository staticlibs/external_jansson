/*
 * Copyright 2015, alex at staticlibs.net
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* 
 * File:   jansson_test.cpp
 * Author: alex
 *
 * Created on February 15, 2015, 7:40 PM
 */

#include <iostream>
#include <string>
#include <cstdlib>

#include "jansson.h"

#include "staticlib/config/assert.hpp"

void test_jansson() {
    auto obj = json_object();
    slassert(obj);
    auto st1 = json_string("foo");
    slassert(st1);
    auto err = json_object_set(obj, "bar", st1);
    slassert(!err);
    auto res = json_dumps(obj, 0);
    slassert(std::string("{\"bar\": \"foo\"}") == std::string(res));

    json_decref(obj);
    json_decref(st1);
    free(res);
}

int main() {
    try {
        test_jansson();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}

