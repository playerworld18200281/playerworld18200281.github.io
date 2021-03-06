#! /bin/sh

printf "\033c"




cd "/opt/www"
fun_exe_start="\033[42;34m 开始执行 `date` \033[0m"
fun_exe_end="\033[42;34m 结束执行 `date` \033[0m"
con_show_str_start="\033[46;30m【 执行 "
con_show_str_end=" 完成】\033[0m"
project_name="playerworld18200281.github.io"

git_new_project(){
    rm -rf "/opt/www/$project_name"
    echo -e "$con_show_str_start 删除playerworld18200281.github.io $con_show_str_end"
    git clone git@github.com:playerworld18200281/playerworld18200281.github.io.git
    echo -e "$con_show_str_start 克隆$project_name $con_show_str_end"
    chmod -R 777 "/opt/www/$project_name"
    echo -e "$con_show_str_start 给$project_name 权限777$con_show_str_end"
}

git_pull(){
    cd "/opt/www/$project_name"
    git add .
    git commit -m 'linux add .'
    git pull
}

compile(){
  cd /opt/www/playerworld18200281.github.io/eosio.token
  echo -e "$con_show_str_start 进入$project_name /eosio.token文件夹 $con_show_str_end"
  rm -rf /opt/www/$project_name/eosio.token/eosio.token.abi
  echo -e "$con_show_str_start 删除.abi文件 $con_show_str_end"
  rm -rf /opt/www/$project_name/eosio.token/eosio.token.wasm
  echo -e "$con_show_str_start 删除.wasm $con_show_str_end"
  eosio-cpp -I include -o eosio.token.wasm src/eosio.token.cpp --abigen
  echo -e "$con_show_str_start 编译.wasm .cpp $con_show_str_end"
  cleos -u http://jungle2.cryptolions.io set contract xushenkai123 /opt/www/playerworld18200281.github.io/eosio.token -p xushenkai123@active
  echo -e "$con_show_str_start set contract $con_show_str_end"

  echo -e "$con_show_str_start sendms $con_show_str_end"
  cleos -u http://jungle2.cryptolions.io  push action xushenkai123 sendms '["xushenkai123", "for 600 seconds", 600]' -p xushenkai123
}

contract_exe(){



    echo -e "$con_show_str_start testcheck $con_show_str_end"
    cleos -u http://jungle2.cryptolions.io push action xushenkai123 testcheck '[""]' -p xushenkai123@active

    echo -e "$con_show_str_start adduser $con_show_str_end"
    cleos -u http://jungle2.cryptolions.io push action xushenkai123 adduser '["xushenkai123", 13888888888, "im bob"]' -p xushenkai123@active


    echo -e "$con_show_str_start get table user $con_show_str_end"
    cleos -u http://jungle2.cryptolions.io get table xushenkai123 xushenkai123 user


    echo -e "$con_show_str_start addpost $con_show_str_end"
    cleos -u http://jungle2.cryptolions.io push action xushenkai123 addpost '["xushenkai123", "my post"]' -p xushenkai123

    echo -e "$con_show_str_start get table post $con_show_str_end"
    cleos -u http://jungle2.cryptolions.io get table xushenkai123 xushenkai123 post

}

echo -e $fun_exe_start
git_pull
compile
contract_exe
echo -e $fun_exe_end











