/**
 * \file
 * \brief     iRODS microservice to generate a randam ID of x digits and (capital) letters
 * \author    Lazlo Westerhof
 * \author    Paul Frederiks
 * \copyright Copyright (c) 2017, Utrecht University
 *
 * Copyright (c) 2017, Utrecht University
 *
 * This file is part of irods-uu-microservices.
 *
 * irods-uu-microservices is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * irods-uu-microservices is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with irods-uu-microservices.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "irods_includes.hh"
#include "reGlobalsExtern.hpp"

#include <string>
#include <vector>
#include <random>
#include <functional>
#include <algorithm>

extern "C" {
  int msiGenerateRandomID(msParam_t* lengthIn,
			 msParam_t* randomIdOut,
			 ruleExecInfo_t *rei)
  {
    /* Check input parameters. */
    if (strcmp(lengthIn->type, INT_MS_T)) {
      return SYS_INVALID_INPUT_PARAM;
    }

    /* Define character set to use in YoDa random ID. */
    std::vector<char> charSet = {
      '0','1','2','3','4',
      '5','6','7','8','9',
      'A','B','C','D','E','F',
      'G','H','I','J','K',
      'L','M','N','O','P',
      'Q','R','S','T','U',
      'V','W','X','Y','Z',
    };

    /* Create a non-deterministic random number generator. */
    std::random_device seed;
    std::default_random_engine rng(seed());

    /* Generate uniform integer distribution, to transform a random number into integer
       in the closed interval of [0, charSet.size()-1] */
    std::uniform_int_distribution<> dist(0, (int)charSet.size()-1);

    /* Create a non-deterministic uniform distribution of the YoDa ID character set. */
    auto randChar = [charSet, &dist, &rng](){return charSet[dist(rng)];};

    /* Set the length of the random string. */
    int length = parseMspForPosInt(lengthIn);
    std::string randomStr(length, 0);

    /* Generate the random string. */
    std::generate_n(randomStr.begin(), length, randChar);

    fillStrInMsParam(randomIdOut, randomStr.c_str());

    return 0;
  }

  irods::ms_table_entry* plugin_factory() {
    irods::ms_table_entry *msvc = new irods::ms_table_entry(2);

    msvc->add_operation("msiGenerateRandomID", "msiGenerateRandomID");

    return msvc;
  }
}
