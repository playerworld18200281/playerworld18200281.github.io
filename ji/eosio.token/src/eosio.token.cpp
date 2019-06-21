/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */

#include <eosio.token/eosio.token.hpp>

namespace eosio {

void token::create( name   issuer,
                    asset  maximum_supply )
{
    require_auth( _self );

    auto sym = maximum_supply.symbol;
    check( sym.is_valid(), "invalid symbol name" );
    check( maximum_supply.is_valid(), "invalid supply");
    check( maximum_supply.amount > 0, "max-supply must be positive");

    stats statstable( _self, sym.code().raw() );
    auto existing = statstable.find( sym.code().raw() );
    check( existing == statstable.end(), "token with symbol already exists" );

    statstable.emplace( _self, [&]( auto& s ) {
       s.supply.symbol = maximum_supply.symbol;
       s.max_supply    = maximum_supply;
       s.issuer        = issuer;
    });
}


void token::issue( name to, asset quantity, string memo )
{
    auto sym = quantity.symbol;
    check( sym.is_valid(), "invalid symbol name" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    stats statstable( _self, sym.code().raw() );
    auto existing = statstable.find( sym.code().raw() );
    check( existing != statstable.end(), "token with symbol does not exist, create token before issue" );
    const auto& st = *existing;

    require_auth( st.issuer );
    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must issue positive quantity" );

    check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
    check( quantity.amount <= st.max_supply.amount - st.supply.amount, "quantity exceeds available supply");

    statstable.modify( st, same_payer, [&]( auto& s ) {
       s.supply += quantity;
    });

    add_balance( st.issuer, quantity, st.issuer );

    if( to != st.issuer ) {
      SEND_INLINE_ACTION( *this, transfer, { {st.issuer, "active"_n} },
                          { st.issuer, to, quantity, memo }
      );
    }
}

void token::retire( asset quantity, string memo )
{
    auto sym = quantity.symbol;
    check( sym.is_valid(), "invalid symbol name" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    stats statstable( _self, sym.code().raw() );
    auto existing = statstable.find( sym.code().raw() );
    check( existing != statstable.end(), "token with symbol does not exist" );
    const auto& st = *existing;

    require_auth( st.issuer );
    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must retire positive quantity" );

    check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );

    statstable.modify( st, same_payer, [&]( auto& s ) {
       s.supply -= quantity;
    });

    sub_balance( st.issuer, quantity );
}

void token::transfer( name    from,
                      name    to,
                      asset   quantity,
                      string  memo )
{
    check( from != to, "cannot transfer to self" );
    require_auth( from );
    check( is_account( to ), "to account does not exist");
    auto sym = quantity.symbol.code();
    stats statstable( _self, sym.raw() );
    const auto& st = statstable.get( sym.raw() );

    require_recipient( from );
    require_recipient( to );

    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must transfer positive quantity" );
    check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    auto payer = has_auth( to ) ? to : from;

    sub_balance( from, quantity );
    add_balance( to, quantity, payer );
}




void token::sub_balance( name owner, asset value ) {
   accounts from_acnts( _self, owner.value );

   const auto& from = from_acnts.get( value.symbol.code().raw(), "no balance object found" );
   check( from.balance.amount >= value.amount, "overdrawn balance" );

   from_acnts.modify( from, owner, [&]( auto& a ) {
         a.balance -= value;
      });
}

void token::add_balance( name owner, asset value, name ram_payer )
{
   accounts to_acnts( _self, owner.value );
   auto to = to_acnts.find( value.symbol.code().raw() );
   if( to == to_acnts.end() ) {
      to_acnts.emplace( ram_payer, [&]( auto& a ){
        a.balance = value;
      });
   } else {
      to_acnts.modify( to, same_payer, [&]( auto& a ) {
        a.balance += value;
      });
   }
}

void token::open( name owner, const symbol& symbol, name ram_payer )
{
   require_auth( ram_payer );

   auto sym_code_raw = symbol.code().raw();

   stats statstable( _self, sym_code_raw );
   const auto& st = statstable.get( sym_code_raw, "symbol does not exist" );
   check( st.supply.symbol == symbol, "symbol precision mismatch" );

   accounts acnts( _self, owner.value );
   auto it = acnts.find( sym_code_raw );
   if( it == acnts.end() ) {
      acnts.emplace( ram_payer, [&]( auto& a ){
        a.balance = asset{0, symbol};
      });
   }
}

void token::close( name owner, const symbol& symbol )
{
   require_auth( owner );
   accounts acnts( _self, owner.value );
   auto it = acnts.find( symbol.code().raw() );
   check( it != acnts.end(), "Balance row already deleted or never existed. Action won't have any effect." );
   check( it->balance.amount == 0, "Cannot close because the balance is not zero." );
   acnts.erase( it );
}
void token::send( name to, asset amount, string memo) {

  action(permission_level{get_self(), "active"_n}, "eosio.token"_n,
        "transfer"_n, std::make_tuple(get_self(), to, amount,
                                      std::string("from eos3d.io"))
      ).send();

    }
    void token::sendtoken( name to, asset amount, string memo) {

      transfer(get_self(),to,amount,memo);
      // action(permission_level{get_self(), "active"_n}, "playerworld5"_n,
      //       "transfer"_n, std::make_tuple(get_self(), to, amount,
      //                                     std::string("from eos3d.io"))
      //     ).send();

    }


  void token::on_token_transfer(name from, name to, asset quantity, std::string memo)
  {
     if(to != _self) return;

     eosio::check(quantity.symbol == symbol("EOS", 4), "must pay with EOS token");

    if(quantity.amount  == 1){

        withdraw(from);
        return ;

    }




      eosio::check(quantity.amount > 10000, "must send more than 1 EOS");


       asset new_quantity = asset(quantity.amount+6, symbol("XSK", 4));

         action(permission_level{_self, "active"_n},
         "playerworld5"_n,
          "transfer"_n,
         make_tuple(_self, from, new_quantity, std::string("+0.0006 XSK!"))
       ).send();

  }
  void token::adduser(name account,name newaccount){
        require_auth(account);
        regaccount(newaccount);
  }
  void token::deleteuser(name account ,name delaccount){
        require_auth(account);
        deleteaccount(delaccount);
  }
  void token::setuser(name account,name addaccount,asset quantity){
    require_auth(account);
    user_index users(get_self(), get_first_receiver().value);
    auto itr = users.find(addaccount.value);
    check( itr != users.end(), "user not find " );

    users.modify( itr, _self, [&]( auto& row ) {
         row.weitixianshouyi = quantity.amount;
      });

  }







/**
  用户提现
*/
  void token::withdraw(name account){

    user_index users(get_self(), get_first_receiver().value);

    auto itr = users.find(account.value);
    check( itr != users.end(), "user not find " );

    asset quantity = asset(itr->weitixianshouyi, symbol("EOS", 4));

    action(permission_level{get_self(), "active"_n}, "eosio.token"_n,
          "transfer"_n, std::make_tuple(get_self(), account, quantity,
                                        std::string("from player world "))
        ).send();

    users.modify( itr, _self, [&]( auto& row ) {
         row.weitixianshouyi = 0;
      });



  }
  /**
    register new account  注册新账号
  */
  void token::regaccount(name account){

    user_index users(get_self(), get_first_receiver().value);
    users.emplace(get_self(), [&](auto& user){
      user.account = account;
      user.account_type = account.value;
      user.parent = 1;
      user.regtime = time_point_sec(current_time_point());

      user.leijitouru               =0;//累计投入
      user.leijitixian              =0; //累计提现
      user.leijishouyi              =0;//累计收益
      user.weitixianshouyi          =0;//未提现收益
      user.tuijianjiangli           =0;  //推荐奖励
      user.dajiangchileijishouyi    =0;//大奖池累计收益
      user.xiaojiangchileijishouyi  =0;//小奖池累计收益
      user.zhituishuliang           =0;  //直推数量
      user.erdaishuliang            =0;   //二代数量
      user.sandaishuliang           =0; //三代数量

    });
  }


  void token::deleteaccount(name account){

    user_index users(_self, _first_receiver.value);
    auto itr = users.require_find(account.value, "Account not found");

    auto next_itr = users.erase(itr);
  }

          // void token::senddeferred(name from, name to,const std::string& message, uint64_t delay) {
          //   require_auth(from);
          //
          //   asset new_quantity = asset(1000+6, symbol("XSK", 4));
          //
          //     action(permission_level{_self, "active"_n},
          //     "playerworld5"_n,
          //      "transfer"_n,
          //     make_tuple(_self, to, new_quantity, std::string("+0.0006 XSK!"))
          //   ).send();




            // transaction t{};
            //
            // t.actions.emplace_back(
            //   permission_level(from, "active"_n),
            //   _self,
            //   "senddelay"_n,
            //   std::make_tuple(from, message)
            // );
            //
            // t.delay_sec = delay;
            //
            // t.send(eosio::current_time_point().sec_since_epoch(), from);
            //
            // print("Sent with a delay of ", delay);
        // }


















}; /// namespace eosio

//EOSIO_DISPATCH( eosio::token, (create)(issue)(transfer)(open)(close)(retire)(send)(sendtoken))
