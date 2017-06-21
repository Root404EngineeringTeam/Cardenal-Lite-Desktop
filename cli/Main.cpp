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

#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>

#if defined(__GNUC__) || defined(__clang__)
    #include <getopt.h>
    #if !defined _GETOPT_H
        #include <unistd.h>
    #endif
#endif

#include "../core/Cipher.h"

#define VERSION_STR "0.0.1"

static void usage(const char* argv);

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << argv[0] << ": missing operand" << std::endl
                  << "Try '" << argv[0] << " --help' for more information." << std::endl;
        std::exit(1);
    }

    int c;
    int hflag = 0, vflag = 0;

    static std::string key = "";

    while (true)
    {
        static struct option long_opts[] = {
            {"password", required_argument, 0, 'p'},
            {"help"    , no_argument      , 0, 'h'},
            {"version" , no_argument      , 0, 'v'},
            {0, 0, 0, 0}
        };

        int opt_index = 0;
        c = getopt_long(argc, argv, "p::hv", long_opts, &opt_index);
        if (c == -1)
            break;

        switch (c)
        {
            case 0:
                break;
            case 'p':
                key = optarg;
                break;
            case 'v':
                std::cout << argv[0] << " v" << VERSION_STR << std::endl;
                vflag = 1;
                break;
            case 'h':
                usage(argv[0]);
                hflag = 1;
                break;
            case '?':
                break;
            default:
                abort();
                break;
        }
    }

    if (vflag || hflag)
        return 0;

    // Ésta comprobación sobra pero yo no me ando con mamadas :v
    if ((key.length() == 0) || (key == ""))
    {
        std::cerr << argv[0] << ": the password is empty. Exiting..." << std::endl;
        std::exit(1);
    }
    
    std::vector<std::string> files = {};
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
            continue;
        files.push_back(argv[i]);
    }

    short int ret = 0;
    Cipher crypter(key);
    for (auto file : files)
        ret = crypter.Encode(file);

    return ret;
}

static void usage(const char* argv)
{
    std::printf("How to use: %s [OPTION]... FILE...\n"
                "Encrypt or decrypt the FILE(s).\n\n"

                "If file extension is \".crypted\" then the FILE(s) will be\n"
                "decrypted. Otherwise, the files will be crypted.\n\n"
                
                "Mandatory arguments for long options are also mandatory\n"
                "for short options.\n"
                "  --password=PASSWORD set the encryption or decryption password\n"
                "  --help              show this help and exit\n"
                "  --version           show version and exit\n\n"
                
                "Ayude sobre errores y/o aportes en:\n"
                "<https://github.com/Root404EngineeringTeam/Cardenal-Lite-Desktop>\n", argv);
}