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

#ifdef __GNUC__
    #include <getopt.h>
#endif

#include "../core/Cardenal.h"

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
    int dflag = 0, eflag = 0;

    static std::string key = "";

    while (true)
    {
        static struct option long_opts[] = {
            {"encrypt", required_argument, 0, 'e'},
            {"decrypt", required_argument, 0, 'd'},
            {"help"   , no_argument      , 0, 'h'},
            {"version", no_argument      , 0, 'v'},
            {0, 0, 0, 0}
        };

        int opt_index = 0;
        c = getopt_long(argc, argv, "e::d::hv", long_opts, &opt_index);
        if (c == -1)
            break;

        switch (c)
        {
            case 0:
                break;
            case 'e':
                key = optarg;
                eflag = 1;
                break;
            case 'd':
                key = optarg;
                dflag = 1;
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

    Cipher crypter(key);
    if (eflag)
    {
        for (auto file : files)
            crypter.Crypt(file);
    }
    else if (dflag)
    {
        for (auto file : files)
            crypter.Decrypt(file);
    }
    else
    {
        std::cerr << argv[0] << ": I don't know what the fuck are you doing. Exiting..." << std::endl;
        std::exit(1);
    }

    return 0;
}

static void usage(const char* argv)
{
    std::printf("How to use: %s [OPTION]... FILE...\n"
                "Encrypt or decrypt the FILE(s).\n\n"
                
                "Mandatory arguments for long options are also mandatory\n"
                "for short options.\n"
                "  --encrypt=PASSWORD  encrypt the FILE(s)\n"
                "  --decrypt=PASSWORD  decrypt the FILE(s)\n"
                "  --help              show this help and exit\n"
                "  --version           show version and exit\n\n"
                
                "Ayude sobre errores y/o aportes en:\n"
                "<https://github.com/Root404EngineeringTeam/Cardenal-Lite-Desktop>\n", argv);
}