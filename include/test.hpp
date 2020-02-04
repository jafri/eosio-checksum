#pragma once

#include <eosio/eosio.hpp>
#include "eosio/datastream.hpp"
#include <div.hpp>

using uint256_t = intx::uint256;
using uint512_t = intx::uint512;

using namespace eosio;
using namespace std;

/**
 *  Serialize a fixed size int
 *
 *  @param ds - The stream to write
 *  @param v - The value to serialize
 *  @tparam Stream - Type of datastream buffer
 *  @tparam T - Type of the object contained in the array
 *  @tparam N - Number of bits
 *  @return datastream<Stream>& - Reference to the datastream
 */
template<typename Stream>
eosio::datastream<Stream>& operator<< ( eosio::datastream<Stream>& ds, const uint256_t& v ) {
    auto bytes = intx::as_bytes(v);
    for(auto i = 0; i < 32; i++) {
      ds << bytes[i];
    }
    return ds;
}

/**
 *  Deserialize a fixed size int
 *
 *  @brief Deserialize a fixed size std::array
 *  @param ds - The stream to read
 *  @param v - The destination for deserialized value
 *  @tparam Stream - Type of datastream buffer
 *  @tparam N - Number of bits
 *  @return datastream<Stream>& - Reference to the datastream
 */
template<typename Stream>
eosio::datastream<Stream>& operator>> ( eosio::datastream<Stream>& ds, uint256_t& v ) {
    uint8_t tmp[32];
    for( auto& i : tmp )
        ds >> i;
    v = intx::be::load<uint256_t>(tmp);
    return ds;
}

CONTRACT test : public contract {
  public:
    using contract::contract;

    test( name receiver, name code, datastream<const char*> ds )
      : contract(receiver, code, ds),
        _accounts(receiver, receiver.value) {}

    TABLE Account {
      uint64_t id;
      uint256_t nonce = 0u;
      uint64_t primary_key() const { return id; };

      EOSLIB_SERIALIZE(Account, (id)(nonce));
    };
    typedef multi_index<"account"_n, Account> account_table;
    account_table _accounts;
};