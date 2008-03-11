/*****************************************************************[SDFVariableHeuristic.cc]
Copyright (c) 2007, Universite d'Orleans - Jeremie Vautard, Marco Benedetti,
Arnaud Lallouet.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*****************************************************************************/
#include "SDFVariableHeuristic.hh"

int SmallestDomainFirst::score(QSpace* qs, int var) {
    switch (qs->type_of_v[var]) {
        case VTYPE_BOOL : 
            return 2;
            break;
        case VTYPE_INT :
            return static_cast<IntVar*>(qs->v[var])->size();
            break;
        default :
            return 1;
            break;
    }
}

int LargestDomainFirst::score(QSpace* qs, int var) {
    switch (qs->type_of_v[var]) {
        case VTYPE_BOOL : 
            return -2;
            break;
        case VTYPE_INT :
            return (0-(static_cast<IntVar*>(qs->v[var])->size()));
            break;
        default :
            return 1;
            break;
    }
}
