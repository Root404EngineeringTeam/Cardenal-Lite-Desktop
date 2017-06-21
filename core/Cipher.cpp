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
#include <sys/stat.h>
#include <sys/types.h>

#include <cryptopp/hex.h>
#include <cryptopp/sha.h>

void WriteToFile(std::ofstream& out_file, std::string fileName, std::vector<unsigned char>& content);
std::ifstream::pos_type GetFileSize(std::string fileName);

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

void Cipher::Process(std::string fileName)
{
    in_file.open(fileName, std::fstream::in);
    auto iter = fileName.find(".crypted");
    if (iter != std::string::npos)
        new_file_name = fileName.erase(iter, fileName.length());
    else
        new_file_name = fileName + ".crypted";

    if (in_file.is_open())
    {
        unsigned int blocks = 0;
        long int file_size = GetFileSize(fileName); /* ZFS */

        while (true)
        {
            long int start = BLOCK_SIZE * blocks,
                     end   = start + BLOCK_SIZE;
        
            if (end > file_size)
                end = file_size;

            parts.push_back(part{start, end, blocks});
            blocks++;
            
            if ((blocks * BLOCK_SIZE) > file_size)
                break;
        }

        std::vector<unsigned char> data = {};
        for (unsigned int i = 0; i < blocks; i++)
        {
            long int block_char_index = 0;

            while (in_file >> std::noskipws >> a)
            {
                std::cout << "password[" << key_index << "] = " << password[key_index] << std::endl;
                k = password[key_index];
                r = a ^ k;
                data.push_back(r);
                block_char_index++;

                if (block_char_index >= parts[i].len)
                {
                    out_file.open(new_file_name, std::fstream::out | std::fstream::app);
                    if (out_file.is_open())
                    {
                        for (auto c: data)
                            out_file << c;
                    }
                    else
                        std::cerr << "[*] Unable to create " << new_file_name << " file." << std::endl;

                    out_file.close();
                    data.clear();
                }

                if (key_index >= key_length)
                    key_index = 0;
                else
                    key_index++;
            }
        }
    }
    else
        std::cerr << "[*] Unable to open " << fileName << std::endl;

    in_file.close();
}

void Cipher::Encode(std::string fileName)
{
    Process(fileName);
}

std::ifstream::pos_type GetFileSize(std::string fileName)
{
    std::ifstream in(fileName, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

Cipher::~Cipher()
{
    if (in_file.is_open())
        in_file.close();
    if (out_file.is_open())
        out_file.close();
}