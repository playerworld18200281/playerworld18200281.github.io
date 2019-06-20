#include <eosio/eosio.hpp>
#include <eosio/system.hpp>

using namespace eosio;

CONTRACT databases : public contract {
public:
using contract::contract;

ACTION testcheck(){
    user_index users(get_self(), get_first_receiver().value);
    check(users.get_code() == "test"_n, "Codes don't match");
    check(users.get_scope() == name{"test"}.value, "Scopes don't match");
    print("match");
}

ACTION adduser(name account, uint64_t phone, std::string signature){
    require_auth(account);
    user_index users(get_self(), get_first_receiver().value);
    users.emplace(account, [&](auto& user){
        user.account = account;
        user.phone = phone;
        user.regtime = time_point_sec(current_time_point());
        user.signature = signature;
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

private:
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
};