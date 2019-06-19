#！/bin/sh

cd /root

sh /root/git_pull.sh

cd /var/www/playerworld18200281.github.io/eosio.token

rm -rf eosio.token.abi

rm -rf eosio.token.wasm

eosio-cpp -I include -o eosio.token.wasm src/eosio.token.cpp --abigen

cleos -u http://jungle2.cryptolions.io set contract xushenkai123 /var/www/playerworld18200281.github.io/eosio.token -p xushenkai123@active
