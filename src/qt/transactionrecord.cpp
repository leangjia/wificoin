<<<<<<< HEAD
// Copyright (c) 2011-2016 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "transactionrecord.h"

#include "base58.h"
#include "consensus/consensus.h"
#include "validation.h"
#include "timedata.h"
#include "wallet/wallet.h"

#include <stdint.h>

=======
#include "transactionrecord.h"

#include "wallet.h"
#include "base58.h"
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

/* Return positive answer if transaction should be shown in list.
 */
bool TransactionRecord::showTransaction(const CWalletTx &wtx)
{
<<<<<<< HEAD
    // There are currently no cases where we hide transactions, but
    // we may want to use this in the future for things like RBF.
=======
    if (wtx.IsCoinBase())
    {
        // Ensures we show generated coins / mined transactions at depth 1
        if (!wtx.IsInMainChain())
        {
            return false;
        }
    }
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    return true;
}

/*
 * Decompose CWallet transaction to model transaction records.
 */
QList<TransactionRecord> TransactionRecord::decomposeTransaction(const CWallet *wallet, const CWalletTx &wtx)
{
    QList<TransactionRecord> parts;
<<<<<<< HEAD
    int64_t nTime = wtx.GetTxTime();
    CAmount nCredit = wtx.GetCredit(ISMINE_ALL);
    CAmount nDebit = wtx.GetDebit(ISMINE_ALL);
    CAmount nNet = nCredit - nDebit;
=======
    int64 nTime = wtx.GetTxTime();
    int64 nCredit = wtx.GetCredit(true);
    int64 nDebit = wtx.GetDebit();
    int64 nNet = nCredit - nDebit;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    uint256 hash = wtx.GetHash();
    std::map<std::string, std::string> mapValue = wtx.mapValue;

    if (nNet > 0 || wtx.IsCoinBase())
    {
        //
        // Credit
        //
<<<<<<< HEAD
        for(unsigned int i = 0; i < wtx.tx->vout.size(); i++)
        {
            const CTxOut& txout = wtx.tx->vout[i];
            isminetype mine = wallet->IsMine(txout);
            if(mine)
            {
                TransactionRecord sub(hash, nTime);
                CTxDestination address;
                sub.idx = i; // vout index
                sub.credit = txout.nValue;
                sub.involvesWatchAddress = mine & ISMINE_WATCH_ONLY;
                if (ExtractDestination(txout.scriptPubKey, address) && IsMine(*wallet, address))
                {
                    // Received by WiFicoin Address
                    sub.type = TransactionRecord::RecvWithAddress;
                    sub.address = CWiFicoinAddress(address).ToString();
=======
        BOOST_FOREACH(const CTxOut& txout, wtx.vout)
        {
            if(wallet->IsMine(txout))
            {
                TransactionRecord sub(hash, nTime);
                CTxDestination address;
                sub.idx = parts.size(); // sequence number
                sub.credit = txout.nValue;
                if (ExtractDestination(txout.scriptPubKey, address) && IsMine(*wallet, address))
                {
                    // Received by Bitcoin Address
                    sub.type = TransactionRecord::RecvWithAddress;
                    sub.address = CBitcoinAddress(address).ToString();
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
                }
                else
                {
                    // Received by IP connection (deprecated features), or a multisignature or other non-simple transaction
                    sub.type = TransactionRecord::RecvFromOther;
                    sub.address = mapValue["from"];
                }
                if (wtx.IsCoinBase())
                {
                    // Generated
                    sub.type = TransactionRecord::Generated;
                }

                parts.append(sub);
            }
        }
    }
    else
    {
<<<<<<< HEAD
        bool involvesWatchAddress = false;
        isminetype fAllFromMe = ISMINE_SPENDABLE;
        for (const CTxIn& txin : wtx.tx->vin)
        {
            isminetype mine = wallet->IsMine(txin);
            if(mine & ISMINE_WATCH_ONLY) involvesWatchAddress = true;
            if(fAllFromMe > mine) fAllFromMe = mine;
        }

        isminetype fAllToMe = ISMINE_SPENDABLE;
        for (const CTxOut& txout : wtx.tx->vout)
        {
            isminetype mine = wallet->IsMine(txout);
            if(mine & ISMINE_WATCH_ONLY) involvesWatchAddress = true;
            if(fAllToMe > mine) fAllToMe = mine;
        }
=======
        bool fAllFromMe = true;
        BOOST_FOREACH(const CTxIn& txin, wtx.vin)
            fAllFromMe = fAllFromMe && wallet->IsMine(txin);

        bool fAllToMe = true;
        BOOST_FOREACH(const CTxOut& txout, wtx.vout)
            fAllToMe = fAllToMe && wallet->IsMine(txout);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

        if (fAllFromMe && fAllToMe)
        {
            // Payment to self
<<<<<<< HEAD
            CAmount nChange = wtx.GetChange();

            parts.append(TransactionRecord(hash, nTime, TransactionRecord::SendToSelf, "",
                            -(nDebit - nChange), nCredit - nChange));
            parts.last().involvesWatchAddress = involvesWatchAddress;   // maybe pass to TransactionRecord as constructor argument
=======
            int64 nChange = wtx.GetChange();

            parts.append(TransactionRecord(hash, nTime, TransactionRecord::SendToSelf, "",
                            -(nDebit - nChange), nCredit - nChange));
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        }
        else if (fAllFromMe)
        {
            //
            // Debit
            //
<<<<<<< HEAD
            CAmount nTxFee = nDebit - wtx.tx->GetValueOut();

            for (unsigned int nOut = 0; nOut < wtx.tx->vout.size(); nOut++)
            {
                const CTxOut& txout = wtx.tx->vout[nOut];
                TransactionRecord sub(hash, nTime);
                sub.idx = nOut;
                sub.involvesWatchAddress = involvesWatchAddress;
=======
            int64 nTxFee = nDebit - wtx.GetValueOut();

            for (unsigned int nOut = 0; nOut < wtx.vout.size(); nOut++)
            {
                const CTxOut& txout = wtx.vout[nOut];
                TransactionRecord sub(hash, nTime);
                sub.idx = parts.size();
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

                if(wallet->IsMine(txout))
                {
                    // Ignore parts sent to self, as this is usually the change
                    // from a transaction sent back to our own address.
                    continue;
                }

                CTxDestination address;
                if (ExtractDestination(txout.scriptPubKey, address))
                {
<<<<<<< HEAD
                    // Sent to WiFicoin Address
                    sub.type = TransactionRecord::SendToAddress;
                    sub.address = CWiFicoinAddress(address).ToString();
=======
                    // Sent to Bitcoin Address
                    sub.type = TransactionRecord::SendToAddress;
                    sub.address = CBitcoinAddress(address).ToString();
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
                }
                else
                {
                    // Sent to IP, or other non-address transaction like OP_EVAL
                    sub.type = TransactionRecord::SendToOther;
                    sub.address = mapValue["to"];
                }

<<<<<<< HEAD
                CAmount nValue = txout.nValue;
=======
                int64 nValue = txout.nValue;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
                /* Add fee to first output */
                if (nTxFee > 0)
                {
                    nValue += nTxFee;
                    nTxFee = 0;
                }
                sub.debit = -nValue;

                parts.append(sub);
            }
        }
        else
        {
            //
            // Mixed debit transaction, can't break down payees
            //
            parts.append(TransactionRecord(hash, nTime, TransactionRecord::Other, "", nNet, 0));
<<<<<<< HEAD
            parts.last().involvesWatchAddress = involvesWatchAddress;
=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        }
    }

    return parts;
}

void TransactionRecord::updateStatus(const CWalletTx &wtx)
{
<<<<<<< HEAD
    AssertLockHeld(cs_main);
    // Determine transaction status

    // Find the block the tx is in
    CBlockIndex* pindex = nullptr;
    BlockMap::iterator mi = mapBlockIndex.find(wtx.hashBlock);
=======
    // Determine transaction status

    // Find the block the tx is in
    CBlockIndex* pindex = NULL;
    std::map<uint256, CBlockIndex*>::iterator mi = mapBlockIndex.find(wtx.hashBlock);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    if (mi != mapBlockIndex.end())
        pindex = (*mi).second;

    // Sort order, unrecorded transactions sort to the top
    status.sortKey = strprintf("%010d-%01d-%010u-%03d",
        (pindex ? pindex->nHeight : std::numeric_limits<int>::max()),
        (wtx.IsCoinBase() ? 1 : 0),
        wtx.nTimeReceived,
        idx);
<<<<<<< HEAD
    status.countsForBalance = wtx.IsTrusted() && !(wtx.GetBlocksToMaturity() > 0);
    status.depth = wtx.GetDepthInMainChain();
    status.cur_num_blocks = chainActive.Height();

    if (!CheckFinalTx(wtx))
    {
        if (wtx.tx->nLockTime < LOCKTIME_THRESHOLD)
        {
            status.status = TransactionStatus::OpenUntilBlock;
            status.open_for = wtx.tx->nLockTime - chainActive.Height();
=======
    status.confirmed = wtx.IsConfirmed();
    status.depth = wtx.GetDepthInMainChain();
    status.cur_num_blocks = nBestHeight;

    if (!wtx.IsFinal())
    {
        if (wtx.nLockTime < LOCKTIME_THRESHOLD)
        {
            status.status = TransactionStatus::OpenUntilBlock;
            status.open_for = wtx.nLockTime - nBestHeight + 1;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        }
        else
        {
            status.status = TransactionStatus::OpenUntilDate;
<<<<<<< HEAD
            status.open_for = wtx.tx->nLockTime;
        }
    }
    // For generated transactions, determine maturity
    else if(type == TransactionRecord::Generated)
    {
        if (wtx.GetBlocksToMaturity() > 0)
        {
            status.status = TransactionStatus::Immature;
=======
            status.open_for = wtx.nLockTime;
        }
    }
    else
    {
        if (GetAdjustedTime() - wtx.nTimeReceived > 2 * 60 && wtx.GetRequestCount() == 0)
        {
            status.status = TransactionStatus::Offline;
        }
        else if (status.depth < NumConfirmations)
        {
            status.status = TransactionStatus::Unconfirmed;
        }
        else
        {
            status.status = TransactionStatus::HaveConfirmations;
        }
    }

    // For generated transactions, determine maturity
    if(type == TransactionRecord::Generated)
    {
        int64 nCredit = wtx.GetCredit(true);
        if (nCredit == 0)
        {
            status.maturity = TransactionStatus::Immature;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

            if (wtx.IsInMainChain())
            {
                status.matures_in = wtx.GetBlocksToMaturity();

                // Check if the block was requested by anyone
                if (GetAdjustedTime() - wtx.nTimeReceived > 2 * 60 && wtx.GetRequestCount() == 0)
<<<<<<< HEAD
                    status.status = TransactionStatus::MaturesWarning;
            }
            else
            {
                status.status = TransactionStatus::NotAccepted;
=======
                    status.maturity = TransactionStatus::MaturesWarning;
            }
            else
            {
                status.maturity = TransactionStatus::NotAccepted;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
            }
        }
        else
        {
<<<<<<< HEAD
            status.status = TransactionStatus::Confirmed;
        }
    }
    else
    {
        if (status.depth < 0)
        {
            status.status = TransactionStatus::Conflicted;
        }
        else if (GetAdjustedTime() - wtx.nTimeReceived > 2 * 60 && wtx.GetRequestCount() == 0)
        {
            status.status = TransactionStatus::Offline;
        }
        else if (status.depth == 0)
        {
            status.status = TransactionStatus::Unconfirmed;
            if (wtx.isAbandoned())
                status.status = TransactionStatus::Abandoned;
        }
        else if (status.depth < RecommendedNumConfirmations)
        {
            status.status = TransactionStatus::Confirming;
        }
        else
        {
            status.status = TransactionStatus::Confirmed;
        }
    }
    status.needsUpdate = false;
=======
            status.maturity = TransactionStatus::Mature;
        }
    }
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

bool TransactionRecord::statusUpdateNeeded()
{
<<<<<<< HEAD
    AssertLockHeld(cs_main);
    return status.cur_num_blocks != chainActive.Height() || status.needsUpdate;
}

QString TransactionRecord::getTxID() const
{
    return QString::fromStdString(hash.ToString());
}

int TransactionRecord::getOutputIndex() const
{
    return idx;
}
=======
    return status.cur_num_blocks != nBestHeight;
}

std::string TransactionRecord::getTxID()
{
    return hash.ToString() + strprintf("-%03d", idx);
}

>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
