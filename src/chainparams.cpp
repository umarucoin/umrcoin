// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "UMR Hi! UMR Hi! Not UMA but UMAR*! August 1, 2017: 2:09 PM ET, Bitcoin split in two";//Dec. 31th 2013 Japan, The winning numbers of the 2013 Year-End Jumbo Lottery:23-130916
    const CScript genesisOutputScript = CScript() << ParseHex("040184710fa689ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9") << OP_CHECKSIG;//
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 1051200;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("56e2c06fd3b2769fa84424b774bf37f75b98b9c0e38b74be3ac6176f1440eddb");//ff9f1c0116d19de7c9963845e129f9ed1bfc0b376eb54fd7afa42e0d418c8bb6
        consensus.BIP65Height = -1;//977759
        consensus.BIP66Height = -1;//977759
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); 
        consensus.nPowTargetTimespan = 1.1 * 24 * 60 * 60; // 1.1 days
        consensus.nPowTargetSpacing = 1.5 * 60; // 1.5 minutes
        consensus.nPowTargetTimespanDigisheld = 1.5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 7560; // 75% of 10080
        consensus.nMinerConfirmationWindow = 10080; // 3.5 days / nPowTargetSpacing * 4 * 0.75
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1501513200;//1488931200; // March 8, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1533049200;//1520467200; // March 8, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1501513200;//1488931200; // March 8, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1533049200;//1520467200; // March 8, 2018

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0");//00000000000000000000000000000000000000000000000031918300dcee3655

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x56e2c06fd3b2769fa84424b774bf37f75b98b9c0e38b74be3ac6176f1440eddb"); //0xdfe283f8cd1560ae7ac69d37a7443138dc4d31748800d81bb0369fabbe4df4aa//1037698

        // Hardfork params
        nSwitchKGWblock = 2;//80000;
        nSwitchDIGIblock = 4;//140000;
        nSwitchLyra2REv2_DGW = 6;//450000;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xdb;//fb
        pchMessageStart[1] = 0xb6;//c0
        pchMessageStart[2] = 0xc0;//b6
        pchMessageStart[3] = 0xfb;//db
        nDefaultPort = 9441;
        nPruneAfterHeight = 100000;
        vAlertPubKey = ParseHex("04ff63b03aff58708537ddb427ed956c9f7ec610cfca5e509a64a0c71cec3edad02006ae5b137906528bf4e317db8988475819132192139317b356ab019323e742");//vAlertPubKey = ParseHex("04fc55d919cdea21e8171fea24c3cf23864ff64a53404829ad50af86e1be1b8217115701b348d50c6aaba6983bc148d3c9e6fa8c11365889774fc1db6da6840c06");

        genesis = CreateGenesisBlock(1502715660, 1030733, 0x1e0ffff0, 1, 50 * COIN);//genesis = CreateGenesisBlock(1388479472, 1234534, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x56e2c06fd3b2769fa84424b774bf37f75b98b9c0e38b74be3ac6176f1440eddb"));//assert(consensus.hashGenesisBlock == uint256S("0xff9f1c0116d19de7c9963845e129f9ed1bfc0b376eb54fd7afa42e0d418c8bb6"));
        assert(genesis.hashMerkleRoot == uint256S("0x78e73c1d046e2e4cb4043ed5c73e7bc4455c066957d9ca05731e021beea7224a"));//assert(genesis.hashMerkleRoot == uint256S("0x35e405a8a46f4dbc1941727aaf338939323c3b955232d0317f8731fe07ac4ba6"));

        // Note that of those with the service bits flag, most only support a subset of possible options
        vSeeds.clear();//vSeeds.push_back(CDNSSeedData("umaru.com", "dnsseed.umaru.com"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,68);  // U
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,75);  // X
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,176);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        base58Prefixes[OLD_SECRET_KEY] = std::vector<unsigned char>(1,178);

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));//vFixedSeeds.clear();//

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
/*               ( 1500, uint256S("0x9f42d51d18d0a8914a00664c433a0ca4be3eed02f9374d790bffbd3d3053d41d"))
                ( 4000, uint256S("0x2c60edac7d9f44d90d1e218af2a8085e78b735185c5bf42f9fe9dbd0e604c97b"))
                ( 8000, uint256S("0x61d4d053b1a4c6deb4c7e806cedd876f25b51da6c51b209109579c7b9892e5c2"))
                ( 16000, uint256S("0x3c4a8887bb3ae0599abfefe765f7c911fbfe98b3f23d7f70b05bf49cf62ebdaf"))
                ( 32000, uint256S("0xc0703986c1c6a9052478db5e52432e5a1e55d6b6362b85f0ffdbb61ce3311b77"))
                ( 58700, uint256S("0xa9c5d9878864b77ba52b068787b83ce2fcf526c5899f40af51c9d441eeb4c84d"))
                ( 80000, uint256S("0xc99b83da7328b58251d16f4646da222b0280f180bd208efa5e3256c9eb6ea2be"))
                ( 115000, uint256S("0x75e642c003e5bd748b679472e981b7b2f81f344b3f197029f84470256cef33e4"))
                ( 189250, uint256S("0x1bea3d5c25a8097eef2e70ece4beb6c502b895fe00056552948309beb3497c99"))
                ( 300000, uint256S("0x11095515590421444ba29396d9122c234baced79be8b32604acc37cf094558ab"))
                ( 444000, uint256S("0x3ed05516cdce4db93b135189592c7e2b37d768f99a1819a1d2ea3a8e5b8439a8"))
                ( 904000, uint256S("0x353f5b7f9440e1d830bd1c265c69fb0e7c7988e343b2202a704406d04a8cd02e"))*/
                ( 0, uint256S("0x56e2c06fd3b2769fa84424b774bf37f75b98b9c0e38b74be3ac6176f1440eddb")),//( 1045000, uint256S("0x562593372b8cee7d2eac325ef8874532d93cc031a75a00cec0086d2d943f40dc")),
        };

        chainTxData = ChainTxData{
            // Data as of block b44bc5ae41d1be67227ba9ad875d7268aa86c965b1d64b47c35be6e8d5c352f4 (height 1155626).
            1502715660,//1499630269, // * UNIX timestamp of last known number of transactions
            0,//2603377,  // * total number of transactions between genesis and that timestamp
                    //   (the tx=... number in the SetBestChain debug.log lines)
            0.01//0.0313     // * estimated number of transactions per second after that timestamp
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v4)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 1051200;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("fb6ca1b2c0ca50d3f3fe57acc0576f13a39e32bdb8c8398863df58ebd023c27e");//a2b106ceba3be0c6d097b2a6a6aacf9d638ba8258ae478158f449c321061e0b2
        consensus.BIP65Height = -1;
        consensus.BIP66Height = -1;
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 1.1 * 24 * 60 * 60; // 1.1 days
        consensus.nPowTargetSpacing = 1.5 * 60; // 2.5 minutes
        consensus.nPowTargetTimespanDigisheld = 1.5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 75; // 75% for testchains
        consensus.nMinerConfirmationWindow = 100; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1488931200; // March 8, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1520467200; // March 8, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1488931200; // March 1, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1520467200; // March 8, 2018

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x0");

        pchMessageStart[0] = 0xf1;//fd
        pchMessageStart[1] = 0xc8;//d2
        pchMessageStart[2] = 0xd2;//c8
        pchMessageStart[3] = 0xfd;//f1

        // Hardfork params
        nSwitchKGWblock = 10;
        nSwitchDIGIblock = 20;
        nSwitchLyra2REv2_DGW = 30;

        nDefaultPort = 19443;
        nPruneAfterHeight = 1000;
        vAlertPubKey = ParseHex("045c02b563109b89dc2b7596daf43d53e81eca5ca0e0d6af08d9cb332212d582c1785f8360b7797a61d6b4fd6ffc461aeb72453c4c968801a078c9cb271860753a");//vAlertPubKey = ParseHex("04887665070e79d20f722857e58ec8f402733f710135521a0b63441419bf5665ba4623bed13fca0cb2338682ab2a54ad13ce07fbc81c3c2f0912a4eb8521dd3cfb");

        genesis = CreateGenesisBlock(1502716411, 726155, 0x1e0ffff0, 1, 50 * COIN);//genesis = CreateGenesisBlock(1488924140, 2122860, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0xfb6ca1b2c0ca50d3f3fe57acc0576f13a39e32bdb8c8398863df58ebd023c27e"));//assert(consensus.hashGenesisBlock == uint256S("0xa2b106ceba3be0c6d097b2a6a6aacf9d638ba8258ae478158f449c321061e0b2"));
        assert(genesis.hashMerkleRoot == uint256S("0x78e73c1d046e2e4cb4043ed5c73e7bc4455c066957d9ca05731e021beea7224a"));//assert(genesis.hashMerkleRoot == uint256S("0x35e405a8a46f4dbc1941727aaf338939323c3b955232d0317f8731fe07ac4ba6"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        //vSeeds.push_back(CDNSSeedData("umaru.com", "testnet-dnsseed.umaru.com"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,130);//111
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,137);//117
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        base58Prefixes[OLD_SECRET_KEY] = std::vector<unsigned char>(1,239);

        //vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
                ( 0, uint256S("fb6ca1b2c0ca50d3f3fe57acc0576f13a39e32bdb8c8398863df58ebd023c27e")),//( 0, uint256S("a2b106ceba3be0c6d097b2a6a6aacf9d638ba8258ae478158f449c321061e0b2")),
        };

        chainTxData = ChainTxData{
                1502716411,//1488924140,
                0,
                0.01
        };


    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = -1; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = -1; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 1.1 * 24 * 60 * 60; // 1.5 days
        consensus.nPowTargetSpacing = 1.5 * 60; // 1.5 minutes
        consensus.nPowTargetTimespanDigisheld = 1.5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        // Hardfork params
        nSwitchKGWblock = 20;
        nSwitchDIGIblock = 40;
        nSwitchLyra2REv2_DGW = 60;

        pchMessageStart[0] = 0xda;//fa
        pchMessageStart[1] = 0xb5;//bf
        pchMessageStart[2] = 0xbf;//b5
        pchMessageStart[3] = 0xfa;//da
        nDefaultPort = 20444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1503223380, 0, 0x207fffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0xecf4b07b4eaab11aa51dded7cc4fe3d55129d97a4e4959eb6dc90ce0b8b3b028"));//0xeaa6e60873e6eb045e910fb0f6f62efbf2f137e409abe97ae4bb6ed0eeb9d8c3
//        assert(genesis.hashMerkleRoot == uint256S("0x36c22ac7cb9e7de936ab750d6762629f5a878386aeafde3a6f7e1224d5efb114"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true; 

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("54e8278a038637531cf969e0930f42224df765d49e5ac072335146c550fb3153")),//0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,130);//111
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,137);//117
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        base58Prefixes[OLD_SECRET_KEY] = std::vector<unsigned char>(1,239);
    }

    void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    regTestParams.UpdateBIP9Parameters(d, nStartTime, nTimeout);
}
