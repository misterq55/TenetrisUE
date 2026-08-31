#pragma once

#include "Tenetris/Common/TenetrisType.h"
#include "Tenetris/Common/TenetrisEnum.h"
#include "TNAiEvaluator.h"

// FTNAiBoardSnapshot
//
// Pure conversion helpers that turn the model's authoritative collision data into the
// normalized occupancy grid consumed by FTNAiEvaluator (Phase A).
//
// The authoritative collision data is the CheckBuffer (via getValueFromCheckBuffer),
// NOT LockedGrid (which is view-only). A cell counts as occupied when its type is not None
// (Obstacle included), matching FTNFieldModel::checkMino.
//
// The integration seam supplies cells through a lambda so this file needs no FTNFieldModel
// reference and stays fully testable:
//   BuildFromCellSampler(width, height, [&](int32 x, int32 y){ return model.SampleCheckCell(x, y); });

namespace FTNAiBoardSnapshot
{
  // Sampler returns the cell at logical (x, y), x in [0,Width), y in [0,Height).
  // This mirrors getValueFromCheckBuffer's coordinate space (y increases upward).
  template <typename TCellSampler>
  FTNAiOccupancyGrid BuildFromCellSampler(int32 width, int32 height, TCellSampler&& sampler)
  {
    FTNAiOccupancyGrid grid;
    grid.Reserve(height);

    for (int32 y = 0; y < height; ++y)
    {
      TArray<bool> row;
      row.Reserve(width);
      for (int32 x = 0; x < width; ++x)
      {
        const FTNCellInfo cell = sampler(x, y);
        // Non-None (including Obstacle) counts as occupied, matching checkMino.
        row.Add(cell.Type != E_TNTetrominoType::None);
      }
      grid.Add(MoveTemp(row));
    }

    return grid;
  }
}