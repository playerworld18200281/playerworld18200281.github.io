/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>

class hello {
public:
    using eosio::contract;
    
    void hello::sendtoken(name from, name to){
        require_auth(from);
    
        auto quantity = asset(1, symbol("EOS", 4));
        //构造一个内联action
        auto inline_action = action(
            permission_level{from, "active"_n},
            "eosio.token"_n,
            "transfer"_n,
            std::make_tuple(from, to, quantity, std::string(""))
            );
        //发送
        inline_action.send();
    }
};

    

    
 
