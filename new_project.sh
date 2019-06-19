#! /bin/sh


git_new_project(){

    con_show_str="[提示信息:]";

    rm -rf /var/www/playerworld18200281.github.io

    echo "$con_show_str 删除playerworld18200281.github.io 完成";

    cd /var/www

    git clone git@github.com:playerworld18200281/playerworld18200281.github.io.git

    echo "$con_show_str clone playerworld18200281.github.io 执行完成";

    chmod -R 777 /var/www/playerworld18200281.github.io

    echo "$con_show_str playerworld18200281 777 权限执行完成";
}

echo "-----函数开始执行-----"
git_new_project
echo "-----函数执行完毕-----"


