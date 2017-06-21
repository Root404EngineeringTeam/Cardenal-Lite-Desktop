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

int Cipher::Process(std::string fileName)
{
    auto fileName_iter = fileName.find(".crypted");
    if (fileName_iter != std::string::npos)
        new_file_name = fileName.erase(fileName_iter, fileName.length());
    else
        new_file_name = fileName + ".crypted";
        

    unsigned int blocks = 0;
    long int file_size = GetFileSize(fileName); /* ZFS */
    if (file_size < 0)
    {
        std::cerr << "[*] unable to get \"" << fileName << "\" file size. Omiting." << std::endl;
        return -1;
    }

    /* definimos los bloques */
    while (true)
    {
        long int end = (BLOCK_SIZE * blocks) + BLOCK_SIZE;
        
        if (end > file_size)
            end = file_size;

        ends.push_back(end);
        blocks++;
            
        if ((blocks * BLOCK_SIZE) > file_size)
            break;
    }

    std::ifstream file(fileName.c_str(), std::fstream::in);
    if (file.is_open())
    {
        std::vector<unsigned char> file_data;
        long int data_counter = 0;

        std::ofstream encoded_file(new_file_name, std::fstream::out | std::fstream::app);
        if (!encoded_file.is_open())
        {
            std::cerr << "[*] Unable to create the " << new_file_name << " file for writing. Omiting." << std::endl;
            return -1;
        }

        for (unsigned int i = 0; i < blocks; i++)
        {
            while (file >> std::noskipws >> a)
            {
                k = password[key_index];
                r = a ^ k;

                file_data.push_back(r);
                data_counter++;

                if (data_counter >= ends[i])
                {
                    key_index = 0;
                    for (auto _bytes : file_data)
                        encoded_file << _bytes;

                    file_data.clear();
                }

                if (key_index >= key_length)
                    key_index = 0;
                else
                    key_index++;
            }
        }

        encoded_file.close();
        file.close();
    }
    else
    {
        std::cerr << "[*] Unable to open \"" << fileName << "\" file. Omiting." << std::endl;
        return -1;
    }

    return 0;
}

std::ifstream::pos_type GetFileSize(std::string fileName)
{
    std::ifstream in(fileName, std::ifstream::ate | std::ifstream::binary);
    std::ifstream::pos_type size = in.tellg();
    in.close();

    return size;
}

Cipher::~Cipher()
{
}