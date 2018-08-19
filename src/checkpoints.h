<<<<<<< HEAD
// Copyright (c) 2009-2016 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WIFICOIN_CHECKPOINTS_H
#define WIFICOIN_CHECKPOINTS_H

#include "uint256.h"

#include <map>

class CBlockIndex;
struct CCheckpointData;

/**
 * Block-chain checkpoints are compiled-in sanity checks.
=======
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef BITCOIN_CHECKPOINT_H
#define BITCOIN_CHECKPOINT_H

#include <map>

class uint256;
class CBlockIndex;

/** Block-chain checkpoints are compiled-in sanity checks.
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
 * They are updated every release or three.
 */
namespace Checkpoints
{
<<<<<<< HEAD

//! Returns last CBlockIndex* in mapBlockIndex that is a checkpoint
CBlockIndex* GetLastCheckpoint(const CCheckpointData& data);

} //namespace Checkpoints

#endif // WIFICOIN_CHECKPOINTS_H
=======
    // Returns true if block passes checkpoint checks
    bool CheckBlock(int nHeight, const uint256& hash);

    // Return conservative estimate of total number of blocks, 0 if unknown
    int GetTotalBlocksEstimate();

    // Returns last CBlockIndex* in mapBlockIndex that is a checkpoint
    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex);

    double GuessVerificationProgress(CBlockIndex *pindex);
}

#endif
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
