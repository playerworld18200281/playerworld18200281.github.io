#! /bin/sh

printf "\033c"
fun_exe_start="\033[42;34m 开始执行 \033[0m"
fun_exe_end="\033[42;34m 结束执行 \033[0m"

git_new_project(){

    con_show_str_start="\033[40;37m【"
    con_show_str_end=" 执行完成】\033[0m"
    project_name="playerworld18200281.github.io"

    rm -rf "/var/www/$project_name"

    echo -e "$con_show_str_start 删除playerworld18200281.github.io $con_show_str_end"

    cd /var/www

    git clone git@github.com:playerworld18200281/playerworld18200281.github.io.git

    echo -e "$con_show_str_start 克隆$project_name $con_show_str_end"

    chmod -R 777 "/var/www/$project_name"

    echo -e "$con_show_str_start 给$project_name 权限777$con_show_str_end"
}

echo -e $fun_exe_start
git_new_project
echo -e $fun_exe_end


