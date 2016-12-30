/* Copyright 2016 Kristofer Björnson
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

/** @file D2HexagonalPrimitive.cpp
 *
 *  @author Kristofer Björnson
 */

#include "D2HexagonalPrimitive.h"

#include <cmath>

using namespace std;

namespace TBTK{
namespace Lattice{
namespace D2{

HexagonalPrimitive::HexagonalPrimitive(double side0Length) :
	MonoclinicPrimitive(side0Length, side0Length, 2.*M_PI/3.)
{
}

HexagonalPrimitive::~HexagonalPrimitive(){
}

};	//End of namespace D2
};	//End of namespace Lattice
};	//End of namespace TBTK