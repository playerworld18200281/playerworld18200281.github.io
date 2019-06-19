/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once


#include <eosio/transaction.hpp>

#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>

#include <string>

namespace eosiosystem {
   class system_contract;
}

namespace eosio {

   using std::string;

   class [[eosio::contract("eosio.token")]] token : public contract {
      public:
         using contract::contract;

       [[eosio::action]]
       void send( name from ,name to, asset amount, string memo);
       
       [[eosio::on_notify("eosio.token::transfer")]]
       void ontransfers( name from ,name to, asset amount, string memo);


      ACTION deferred(name from, const std::string& message,uint64_t delay) {
          require_auth(from);

          print("Printing deferred ", from, message);

          transaction t{};

          t.actions.emplace_back(
                  permission_level(from, "active"_n),
                  _self,
                  "sendms"_n,
                  std::make_tuple(from, message,delay)
          );

          t.delay_sec = delay;

          t.send(eosio::current_time_point().sec_since_epoch(), from);

          print("Sent with a delay of ", delay);

      }

      ACTION sendms(name from, const std::string& message, uint64_t delay) {
           require_auth(from);

           transaction t{};

           t.actions.emplace_back(
           permission_level(from, "active"_n),
           _self,
           "deferred"_n,
           std::make_tuple(from, message)
           );

           t.delay_sec = delay;

           t.send(eosio::current_time_point().sec_since_epoch(), from);

           print("Sent with a delay of ", delay);
      }

         [[eosio::action]]
         void create( name   issuer,
                      asset  maximum_supply);

         [[eosio::action]]
         void issue( name to, asset quantity, string memo );

         [[eosio::action]]
         void retire( asset quantity, string memo );

         [[eosio::action]]
         void transfer( name    from,
                        name    to,
                        asset   quantity,
                        string  memo );

         [[eosio::action]]
         void open( name owner, const symbol& symbol, name ram_payer );

         [[eosio::action]]
         void close( name owner, const symbol& symbol );

         static asset get_supply( name token_contract_account, symbol_code sym_code )
         {
            stats statstable( token_contract_account, sym_code.raw() );
            const auto& st = statstable.get( sym_code.raw() );
            return st.supply;
         }

         static asset get_balance( name token_contract_account, name owner, symbol_code sym_code )
         {
            accounts accountstable( token_contract_account, owner.value );
            const auto& ac = accountstable.get( sym_code.raw() );
            return ac.balance;
         }

      private:
         struct [[eosio::table]] account {
            asset    balance;

            uint64_t primary_key()const { return balance.symbol.code().raw(); }
         };

         struct [[eosio::table]] currency_stats {
            asset    supply;
            asset    max_supply;
            name     issuer;

            uint64_t primary_key()const { return supply.symbol.code().raw(); }
         };

         typedef eosio::multi_index< "accounts"_n, account > accounts;
         typedef eosio::multi_index< "stat"_n, currency_stats > stats;

         void sub_balance( name owner, asset value );
         void add_balance( name owner, asset value, name ram_payer );
   };

} /// namespace eosio
