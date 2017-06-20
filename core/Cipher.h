/*
 * Copyright 2017 Root404, Co
 *                Alvaro Stagg [alvarostagg@openmailbox.org]
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <fstream>
#include <string>
#include <vector>

class Cipher
{
public:
    Cipher(const std::string& key);
    virtual ~Cipher();

    void Crypt(std::string fileName);
    void Decrypt(std::string fileName);
private:
    void XOREncode(std::string fileName, std::vector<unsigned char>& data);

    unsigned char r, a, k;

    size_t key_length;
    size_t key_index;

    std::ifstream in_file;
    std::ofstream out_file;
    std::string password;
};