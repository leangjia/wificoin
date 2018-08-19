<<<<<<< HEAD
// Copyright (c) 2011-2016 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WIFICOIN_QT_TRANSACTIONRECORD_H
#define WIFICOIN_QT_TRANSACTIONRECORD_H

#include "amount.h"
#include "uint256.h"

#include <QList>
#include <QString>
=======
#ifndef TRANSACTIONRECORD_H
#define TRANSACTIONRECORD_H

#include "uint256.h"

#include <QList>
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

class CWallet;
class CWalletTx;

/** UI model for transaction status. The transaction status is the part of a transaction that will change over time.
 */
class TransactionStatus
{
public:
    TransactionStatus():
<<<<<<< HEAD
        countsForBalance(false), sortKey(""),
        matures_in(0), status(Offline), depth(0), open_for(0), cur_num_blocks(-1)
    { }

    enum Status {
        Confirmed,          /**< Have 6 or more confirmations (normal tx) or fully mature (mined tx) **/
        /// Normal (sent/received) transactions
        OpenUntilDate,      /**< Transaction not yet final, waiting for date */
        OpenUntilBlock,     /**< Transaction not yet final, waiting for block */
        Offline,            /**< Not sent to any other nodes **/
        Unconfirmed,        /**< Not yet mined into a block **/
        Confirming,         /**< Confirmed, but waiting for the recommended number of confirmations **/
        Conflicted,         /**< Conflicts with other transaction or mempool **/
        Abandoned,          /**< Abandoned from the wallet **/
        /// Generated (mined) transactions
        Immature,           /**< Mined but waiting for maturity */
        MaturesWarning,     /**< Transaction will likely not mature because no nodes have confirmed */
        NotAccepted         /**< Mined but not accepted */
    };

    /// Transaction counts towards available balance
    bool countsForBalance;
    /// Sorting key based on status
=======
        confirmed(false), sortKey(""), maturity(Mature),
        matures_in(0), status(Offline), depth(0), open_for(0), cur_num_blocks(-1)
    { }

    enum Maturity
    {
        Immature,
        Mature,
        MaturesWarning, /**< Transaction will likely not mature because no nodes have confirmed */
        NotAccepted
    };

    enum Status {
        OpenUntilDate,
        OpenUntilBlock,
        Offline,
        Unconfirmed,
        HaveConfirmations
    };

    bool confirmed;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    std::string sortKey;

    /** @name Generated (mined) transactions
       @{*/
<<<<<<< HEAD
=======
    Maturity maturity;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    int matures_in;
    /**@}*/

    /** @name Reported status
       @{*/
    Status status;
<<<<<<< HEAD
    qint64 depth;
    qint64 open_for; /**< Timestamp if status==OpenUntilDate, otherwise number
=======
    int64 depth;
    int64 open_for; /**< Timestamp if status==OpenUntilDate, otherwise number
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
                      of additional blocks that need to be mined before
                      finalization */
    /**@}*/

    /** Current number of blocks (to know whether cached status is still valid) */
    int cur_num_blocks;
<<<<<<< HEAD

    bool needsUpdate;
=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
};

/** UI model for a transaction. A core transaction can be represented by multiple UI transactions if it has
    multiple outputs.
 */
class TransactionRecord
{
public:
    enum Type
    {
        Other,
        Generated,
        SendToAddress,
        SendToOther,
        RecvWithAddress,
        RecvFromOther,
        SendToSelf
    };

<<<<<<< HEAD
    /** Number of confirmation recommended for accepting a transaction */
    static const int RecommendedNumConfirmations = 6;
=======
    /** Number of confirmation needed for transaction */
    static const int NumConfirmations = 6;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

    TransactionRecord():
            hash(), time(0), type(Other), address(""), debit(0), credit(0), idx(0)
    {
    }

<<<<<<< HEAD
    TransactionRecord(uint256 _hash, qint64 _time):
            hash(_hash), time(_time), type(Other), address(""), debit(0),
=======
    TransactionRecord(uint256 hash, int64 time):
            hash(hash), time(time), type(Other), address(""), debit(0),
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
            credit(0), idx(0)
    {
    }

<<<<<<< HEAD
    TransactionRecord(uint256 _hash, qint64 _time,
                Type _type, const std::string &_address,
                const CAmount& _debit, const CAmount& _credit):
            hash(_hash), time(_time), type(_type), address(_address), debit(_debit), credit(_credit),
=======
    TransactionRecord(uint256 hash, int64 time,
                Type type, const std::string &address,
                int64 debit, int64 credit):
            hash(hash), time(time), type(type), address(address), debit(debit), credit(credit),
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
            idx(0)
    {
    }

    /** Decompose CWallet transaction to model transaction records.
     */
    static bool showTransaction(const CWalletTx &wtx);
    static QList<TransactionRecord> decomposeTransaction(const CWallet *wallet, const CWalletTx &wtx);

    /** @name Immutable transaction attributes
      @{*/
    uint256 hash;
<<<<<<< HEAD
    qint64 time;
    Type type;
    std::string address;
    CAmount debit;
    CAmount credit;
=======
    int64 time;
    Type type;
    std::string address;
    int64 debit;
    int64 credit;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    /**@}*/

    /** Subtransaction index, for sort key */
    int idx;

    /** Status: can change with block chain update */
    TransactionStatus status;

<<<<<<< HEAD
    /** Whether the transaction was sent/received with a watch-only address */
    bool involvesWatchAddress;

    /** Return the unique identifier for this transaction (part) */
    QString getTxID() const;

    /** Return the output index of the subtransaction  */
    int getOutputIndex() const;
=======
    /** Return the unique identifier for this transaction (part) */
    std::string getTxID();
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

    /** Update status from core wallet tx.
     */
    void updateStatus(const CWalletTx &wtx);

    /** Return whether a status update is needed.
     */
    bool statusUpdateNeeded();
};

<<<<<<< HEAD
#endif // WIFICOIN_QT_TRANSACTIONRECORD_H
=======
#endif // TRANSACTIONRECORD_H
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
