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
    std::string new_file_name = fileName;
    std::string::size_type file_name_iter = new_file_name.find(".crypted");

    if (file_name_iter == std::string::npos)
        new_file_name += ".crypted";
    else
        new_file_name.replace(file_name_iter, 8, "");

    int ret = 0;

    std::ifstream file(fileName, std::fstream::in);
    if (!file.is_open())
    {
        std::cerr << "[*] unable to open \"" << fileName << "\" file. Omiting." << std::endl;
        return -1;
    }
    else
    {
        /* I don't know if this is the best way to get the size
         * of an opened file. But certainly it's better than
         * open the same file twice at time.
         */
        std::ifstream::pos_type file_size = file.tellg();
        file.seekg(0, std::ios::end);
        file_size += file.tellg();
        file.seekg(0, std::ios::beg);

        if (file_size <= 0)
        {
            std::cerr << "[*] unable to get \"" << fileName << "\" file size. Omiting." << std::endl;
            ret = -1;
        }
        else
        {
            long int blocks = 0;
            while (true)
            {
                long int start = (BLOCK_SIZE * blocks);
                long int end   = start + BLOCK_SIZE;

                if (end > file_size)
                    end = file_size;

                block_chunks.push_back(block_chunk{start, end});
                blocks++;

                if (start > file_size)
                    break;
            }

            std::ofstream encoded_file(new_file_name, std::fstream::out);
            if (!encoded_file.is_open())
            {
                std::cerr << "[*] unable to create \"" << new_file_name << "\" file for writing. Omiting." << std::endl;
                ret = -1;
            }
            else
            {
                long int data_counter = 0;
                std::vector<unsigned char> file_data;

                for (unsigned int i = 0; i < blocks; i++)
                {
                    std::cout << "\r" << fileName << ": Encoding block " << i << " of " << blocks - 1 << std::flush;
                    while (file >> std::noskipws >> a)
                    {
                        k = password[key_index];
                        r = a ^ k;

                        file_data.push_back(r);
                        data_counter++;

                        if ((data_counter + block_chunks[i].start) >= block_chunks[i].end)
                        {
                            key_index = data_counter = 0;
                            for (auto& _bytes : file_data)
                                encoded_file << _bytes;

                            file_data.clear();
                            break;
                        }

                        if (key_index >= key_length)
                            key_index = 0;
                        else
                            key_index++;
                    }
                }

                std::cout << ". Done." << std::endl;

                encoded_file.close();
                file.close();
            }
        }
    }

    return ret;
}

Cipher::~Cipher()
{
    if (block_chunks.size() != 0)
        block_chunks.clear();
}
