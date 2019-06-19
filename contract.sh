#！/bin/sh

rm -rf /var/www/playerworld18200281.github.io/eosio.token/eosio.token.abi

rm -rf /var/www/playerworld18200281.github.io/eosio.token/eosio.token.wasm

cd /root

sh /root/git_pull.sh

cd /var/www/playerworld18200281.github.io/eosio.token

eosio-cpp -I include -o eosio.token.wasm src/eosio.token.cpp --abigen

cleos -u http://jungle2.cryptolions.io set contract xushenkai123 /var/www/playerworld18200281.github.io/eosio.token -p xushenkai123@active
