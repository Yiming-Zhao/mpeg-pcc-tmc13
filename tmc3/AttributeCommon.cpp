/* The copyright in this software is being made available under the BSD
 * Licence, included below.  This software may be subject to other third
 * party and contributor rights, including patent rights, and no such
 * rights are granted under this licence.
 *
 * Copyright (c) 2017-2019, ISO/IEC
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

#include "AttributeCommon.h"

#include "PCCTMC3Common.h"

namespace pcc {

//============================================================================
// AttributeLods methods

void
AttributeLods::generate(
  const AttributeParameterSet& aps,
  int geom_num_points,
  int minGeomNodeSizeLog2,
  const PCCPointSet3& cloud)
{
  _aps = aps;

  if (minGeomNodeSizeLog2 > 0)
    assert(aps.scalable_lifting_enabled_flag);

  buildPredictorsFast(
    aps, cloud, minGeomNodeSizeLog2, geom_num_points, predictors,
    numPointsInLod, indexes);

  assert(predictors.size() == cloud.getPointCount());
  for (auto& predictor : predictors)
    predictor.computeWeights();
}

//----------------------------------------------------------------------------

bool
AttributeLods::isReusable(const AttributeParameterSet& aps) const
{
  // No LoDs cached => can be reused by anything
  if (numPointsInLod.empty())
    return true;

  // If the other aps doesn't use LoDs, it is compatible.
  // Otherwise, if both use LoDs, check each parameter
  if (!(_aps.lodParametersPresent() && aps.lodParametersPresent()))
    return true;

  // NB: the following comparison order needs to be the same as the i/o
  // order otherwise comparisons may involve undefined values

  if (_aps.num_pred_nearest_neighbours != aps.num_pred_nearest_neighbours)
    return false;

  if (_aps.search_range != aps.search_range)
    return false;

  if (_aps.num_detail_levels != aps.num_detail_levels)
    return false;

  if (_aps.lod_neigh_bias != aps.lod_neigh_bias)
    return false;

  // until this feature is stable, always generate LoDs.
  if (_aps.scalable_lifting_enabled_flag || aps.scalable_lifting_enabled_flag)
    return false;

  if (_aps.lod_decimation_enabled_flag != aps.lod_decimation_enabled_flag)
    return false;

  if (_aps.dist2 != aps.dist2)
    return false;

  if (
    _aps.intra_lod_prediction_enabled_flag
    != aps.intra_lod_prediction_enabled_flag)
    return false;

  return true;
}

//============================================================================

}  // namespace pcc
