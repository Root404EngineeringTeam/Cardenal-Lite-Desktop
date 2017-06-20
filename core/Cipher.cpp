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

/*
 * Abrir un archivo sólo para leerle los bloques y luego
 * cerrarlo para volver abrirlo y ahí sí leer su contenido
 * es una completa mamada. Mejor todo de golpe.
 */
void Cipher::Process(std::string fileName)
{
    in_file.open(fileName, std::fstream::in);

    if (in_file.is_open())
    {
        long int blocks = 0; /* ¿Tal vez sólo int? */
        long int file_size = GetFileSize(fileName);
        std::cout << "file_size: " << file_size << std::endl;

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
    }
    else
        std::cerr << "[*] Unable to open " << fileName << std::endl;

    in_file.close();
}

void Cipher::Crypt(std::string fileName)
{
    // std::vector<unsigned char> out;
    Process(fileName);
    // WriteToFile(out_file, fileName + ".crypted", out);
}

void Cipher::Decrypt(std::string fileName)
{
    // std::vector<unsigned char> out;
    Process(fileName);
    // WriteToFile(out_file, fileName.erase(fileName.find(".crypted")), out);
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
        std::cerr << "[*] Unable to create " << fileName << " and write content on it" << std::endl;
    
    out_file.close();
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