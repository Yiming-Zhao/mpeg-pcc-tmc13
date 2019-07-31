/* The copyright in this software is being made available under the BSD
 * Licence, included below.  This software may be subject to other third
 * party and contributor rights, including patent rights, and no such
 * rights are granted under this licence.
 *
 * Copyright (c) 2019, ISO/IEC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * * Neither the name of the ISO/IEC nor the names of its contributors
 *   may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <array>

namespace pcc {

struct AttributeParameterSet;
struct AttributeBrickHeader;

//============================================================================
// Quantisation methods

class Quantizer {
public:
  // Derives step sizes from qp
  Quantizer(int qp);
  Quantizer(const Quantizer&) = default;
  Quantizer& operator=(const Quantizer&) = default;

  // The quantizer's step size
  int stepSize() const { return _stepSize; }

  // Quantise a value
  int64_t quantize(int64_t x) const;

  // Scale (inverse quantise) a quantised value
  int64_t scale(int64_t x) const;

private:
  // Quantisation step size
  int _stepSize;
};

//---------------------------------------------------------------------------

inline int64_t
Quantizer::quantize(int64_t x) const
{
  const int64_t shift = _stepSize / 3;

  if (x >= 0) {
    return (x + shift) / _stepSize;
  }
  return -((shift - x) / _stepSize);
}

//---------------------------------------------------------------------------

inline int64_t
Quantizer::scale(int64_t x) const
{
  return x * _stepSize;
}

//============================================================================
// Encapslation of multi-component attribute quantizer values.

typedef std::array<Quantizer, 2> Quantizers;

//============================================================================
// Derive quantisation step sizes for each component given attribute

Quantizers deriveQuantizers(
  const AttributeParameterSet& attr_aps, const AttributeBrickHeader& abh);

//============================================================================

}  // namespace pcc