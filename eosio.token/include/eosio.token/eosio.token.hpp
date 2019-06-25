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
        //-----

       ACTION testcheck(){
           user_index users(get_self(), get_first_receiver().value);
           check(users.get_code() == "test"_n, "Codes don't match");
           check(users.get_scope() == name{"test"}.value, "Scopes don't match");
           print("match");
       }
       ACTION reguser(name account){
            user_index users(get_self(), get_first_receiver().value);
            users.emplace(get_self(), [&](auto& user){
                user.account = account;
                user.account_type = account.value;
                user.parent = 1;
                user.regtime = time_point_sec(current_time_point());

//                user.leijitouru               =0;//累计投入
//                user.leijitixian              =0; //累计提现
//                user.leijishouyi              =0;//累计收益
//                user.weitixianshouyi          =0;//未提现收益
//                user.tuijianjiangli           =0;  //推荐奖励
//                user.dajiangchileijishouyi    =0;//大奖池累计收益
//                user.xiaojiangchileijishouyi  =0;//小奖池累计收益
//                user.zhituishuliang           =0;  //直推数量
//                user.erdaishuliang            =0;   //二代数量
//                user.sandaishuliang           =0; //三代数量

            });
       }
       ACTION adduser(name account, uint64_t phone, std::string signature){
           require_auth(account);
           user_index users(get_self(), get_first_receiver().value);
           users.emplace(account, [&](auto& userg){
               userg.account = account;
               userg.phone = phone;
               userg.regtime = time_point_sec(current_time_point());
               userg.signature = signature;
           });
       }

       ACTION addpost(name account, std::string content){
           require_auth(account);
           user_index users(get_self(), get_first_receiver().value);
           auto itr = users.find(account.value);
           eosio::check(itr != users.end(), "account not found");

           post_index posts(get_self(), get_first_receiver().value);
           posts.emplace(account, [&](auto& post){
               post.id = posts.available_primary_key();
               post.author = account;
               post.content = content;
           });
       }


        //-----
         //start
         ACTION deferred(name from, const std::string& message ,uint64_t delay) {
             require_auth(from);

             print("Printing deferred ", from, message);
             transaction t{};

             t.actions.emplace_back(
                     permission_level(from, "active"_n),
                     _self,
                     "sendms"_n,
                     std::make_tuple(from, message)
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
       void sends( name from ,name to, asset amount, string memo);

       [[eosio::on_notify("eosio.token::transfer")]]
       void ontransfers( name from ,name to, asset amount, string memo);

       //end
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

         using create_action = eosio::action_wrapper<"create"_n, &token::create>;
         using issue_action = eosio::action_wrapper<"issue"_n, &token::issue>;
         using retire_action = eosio::action_wrapper<"retire"_n, &token::retire>;
         using transfer_action = eosio::action_wrapper<"transfer"_n, &token::transfer>;
         using open_action = eosio::action_wrapper<"open"_n, &token::open>;
         using close_action = eosio::action_wrapper<"close"_n, &token::close>;
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
         ///
       TABLE user {
               name account;
               uint64_t phone;
               time_point_sec regtime;
               std::string signature;
               uint64_t primary_key() const { return account.value; }
               uint64_t get_secondary_1() const { return phone; }
               uint64_t get_secondary_2() const { return regtime.sec_since_epoch(); }
       };

       TABLE post {
               uint64_t id;
               name author;
               std::string content;
               uint64_t primary_key() const { return id; }
               uint64_t get_secondary_1() const { return author.value; }
       };

       typedef multi_index<"user"_n, user,
               indexed_by<"byphone"_n, const_mem_fun<user, uint64_t, &user::get_secondary_1>>,
       indexed_by<"byregtime"_n, const_mem_fun<user, uint64_t, &user::get_secondary_2>>
       > user_index;

       typedef multi_index<"post"_n, post,
               indexed_by<"byauthor"_n, const_mem_fun<post, uint64_t, &post::get_secondary_1>>
       > post_index;
       ///
   };

} /// namespace eosio
