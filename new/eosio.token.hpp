/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once
#include <eosio/transaction.hpp>
#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <eosio/singleton.hpp>

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

        [[eosio::action]]
        void send(name to, asset amount, string memo);

        [[eosio::action]]
        void sendtoken(name to, asset amount, string memo);

        [[eosio::action]]
        void  adduser(name account,name addaccount);
        [[eosio::action]]
        void setuser(name account,name addaccount,asset quantity);
        [[eosio::action]]
        void deleteuser(name account ,name delaccount);
        //static void on_tranfer(name from, name to, asset quantity, std::string memo);
        //  void deposit(name from, name to, asset quantity, std::string memo);
        //  [[eosio::on_notify("eosio.token::transfer")]]
        //    void deposit(name from, name to, asset quantity, std::string memo) ;

        [[eosio::on_notify("eosio.token::transfer")]]
        void on_token_transfer(name from, name to, asset quantity, std::string memo);
        //[[eosio::action]]
//  void senddeferred(name from,name to, const std::string& message, uint64_t delay) ;

        ACTION deferred(name from, const std::string& message,uint64_t delay) {
            require_auth(from);

            print("Printing deferred ", from, message);


            transaction t{};

            t.actions.emplace_back(
                    permission_level(from, "active"_n),
                    _self,
                    "sendd"_n,
                    std::make_tuple(from, message, delay)
            );

            t.delay_sec = delay;

            t.send(eosio::current_time_point().sec_since_epoch(), from);
        }

        ACTION sendd(name from, const std::string& message, uint64_t delay) {
            require_auth(from);

            transaction t{};

            t.actions.emplace_back(
                    permission_level(from, "active"_n),
                    _self,
                    "deferred"_n,
                    std::make_tuple(from, message, delay)
            );

            t.delay_sec = delay;

            t.send(eosio::current_time_point().sec_since_epoch(), from);

            print("Sent with a delay of ", delay);
        }





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


        /**
           game player 玩家表
        */
        TABLE  gameuser {
                name account;
                uint64_t account_type;
                time_point_sec regtime;
                uint64_t parent;
                uint64_t leijitouru;//累计投入
                uint64_t leijitixian;//累计提现
                uint64_t leijishouyi;//累计收益
                uint64_t weitixianshouyi;//未提现收益
                uint64_t tuijianjiangli;  //推荐奖励
                uint64_t dajiangchileijishouyi; //大奖池累计收益
                uint64_t xiaojiangchileijishouyi;//小奖池累计收益
                uint64_t zhituishuliang;  //直推数量
                uint64_t erdaishuliang;   //二代数量
                uint64_t sandaishuliang;  //三代数量

                uint64_t primary_key() const { return account.value ;}
                uint64_t get_secondary_1() const { return regtime.sec_since_epoch(); }

        };
        /**

         current game status 当前游戏状态

        */
        TABLE game{

                time_point_sec next_small_time; //小奖池开奖时间
                time_point_sec next_big_time; //大奖池开奖时间
                uint64_t next_small_amount; //小奖池金额
                uint64_t next_big_amount;  //大奖池金额

                uint64_t total_amount_in;    //总计游戏金额累计投入
                uint64_t total_amount_out;   //总计游戏金额结算
                uint64_t total_amount_recommend_out;  //总计游戏推荐奖励



        };

        /**
          history of game 游戏开奖历史

        */
        TABLE historygame {

                uint64_t id;          //自增ID
                uint64_t gtype;   //奖池类型
                time_point_sec opentime;    //开奖时间
                uint64_t amountg;  //开奖金额

                uint64_t primary_key() const { return id ;}
                uint64_t get_secondary_1() const { return opentime.sec_since_epoch(); }

        };

        /**

          history of account  历史中奖用户
        */
        TABLE historyuser {
                name account;
                time_point_sec opentimea;    //开奖时间
                uint64_t amounta;  //开奖金额

                uint64_t primary_key() const { return account.value ;}
                uint64_t get_secondary_1() const { return opentimea.sec_since_epoch(); }
        };


        typedef eosio::multi_index<"gameuser"_n, gameuser,  indexed_by<"byregtime"_n, const_mem_fun<gameuser, uint64_t, &gameuser::get_secondary_1>>> user_index;
        typedef eosio::multi_index<"historygame"_n, historygame,  indexed_by<"opentime"_n, const_mem_fun<historygame, uint64_t, &historygame::get_secondary_1>>> history_game_index;
        typedef eosio::multi_index<"historyuser"_n, historyuser,  indexed_by<"opentimea"_n, const_mem_fun<historyuser, uint64_t, &historyuser::get_secondary_1>>> history_user_index;
        typedef eosio::singleton< "game"_n, game >   app_game;


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

        void regaccount(name account);
        void deleteaccount(name account);
        void withdraw(name user);
    };

} /// namespace eosio
