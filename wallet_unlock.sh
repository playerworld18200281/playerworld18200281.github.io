#!/usr/bin/expect

set timeout 30
 echo -e "$con_show_str_start 解锁钱包 $con_show_str_end"
spawn  wallet unlock

expect "password:"

  send "PW5KJem3gRTiBKV7sZXJYHMZwtyxYTWqH1pWYZvaKDuTVhrBikxSi\r"

interact
 echo -e "$con_show_str_start 自动输入密码 $con_show_str_end"