/*
 * Copyright (c) 2015, Barcelona Supercomputing Center
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer; redistributions in binary form
 * must reproduce the above copyright notice, this list of conditions and the
 * following disclaimer in the documentation and/or other materials provided with
 * the distribution; neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <cstdio>
#include <vector>
#include <algorithm>


int main(int argc, char** argv)
{
    if (argc != 3) {
        printf("Usage: traceSorter oldFile newFile\n");
        return 1;
    }
    std::vector<std::vector<long long int> > v;
    std::vector<long long int> tmp(5);
    FILE* pold = fopen(argv[1], "r");

    // We write 1st core data
    while (!feof(pold)) {
        fscanf(pold, "%lld:%lld:%lld:%lld:%lld\n", &tmp[0], &tmp[1], &tmp[2],
               &tmp[3], &tmp[4]);
        v.push_back(tmp);
    }
    fclose(pold);
    std::sort(v.begin(), v.end());
    FILE* pnew = fopen(argv[2], "w");
    for (unsigned int i = 0; i < v.size(); i++) {
        fprintf(pnew, "%.6d:%lld:%lld:%lld:%lld\n", static_cast<int>(v[i][0]), v[i][1],
                v[i][2], v[i][3], v[i][4]);
    }
    fclose(pnew);
    return 0;
  }
