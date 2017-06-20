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

#include "Cipher.h"

#include <iostream>
#include <algorithm>

#include <cryptopp/hex.h>
#include <cryptopp/sha.h>

void WriteToFile(std::ofstream& out_file, std::string fileName, std::vector<unsigned char>& content);

Cipher::Cipher(const std::string& key)
{
    CryptoPP::SHA256 hash;
    unsigned char digest[CryptoPP::SHA256::DIGESTSIZE];

    hash.CalculateDigest(digest, (unsigned char*)key.c_str(), key.length());

    CryptoPP::HexEncoder encoder;
    encoder.Attach(new CryptoPP::StringSink(password));
    encoder.Put(digest, sizeof(digest));
    encoder.MessageEnd();
    std::transform(password.begin(), password.end(), password.begin(), ::tolower);

    key_length = password.length() - 1;
    key_index = 0;
}

void Cipher::XOREncode(std::string fileName, std::vector<unsigned char>& data)
{
    in_file.open(fileName, std::fstream::in);

    if (in_file.is_open())
    {
        key_index = 0;
        while (in_file >> std::noskipws >> a)
        {
            k = password[key_index];
            r = a ^ k;
            data.push_back(r);

            if (key_index >= key_length)
                key_index = 0;
            else
                key_index++;
        }
    }
    else
        std::cerr << "unable to open " << fileName << std::endl;

    in_file.close();
}

void Cipher::Crypt(std::string fileName)
{
    std::vector<unsigned char> out;
    XOREncode(fileName, out);
    WriteToFile(out_file, fileName + ".crypted", out);
}

void Cipher::Decrypt(std::string fileName)
{
    std::vector<unsigned char> out;
    XOREncode(fileName, out);
    WriteToFile(out_file, fileName.erase(fileName.find(".crypted")), out);
}

void WriteToFile(std::ofstream& out_file, std::string fileName, std::vector<unsigned char>& content)
{
    out_file.open(fileName, std::fstream::out);
    if (out_file.is_open())
    {
        for (auto b : content)
        {
            out_file << b;
        }
    }
    else
        std::cerr << "unable to create " << fileName << " and write content on it" << std::endl;
    
    out_file.close();
}

Cipher::~Cipher()
{
    if (in_file.is_open())
        in_file.close();
    if (out_file.is_open())
        out_file.close();
}