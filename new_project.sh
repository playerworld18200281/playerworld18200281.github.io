#! /bin/sh


git_new_project(){

    con_show_str="/---提示信息---/"

    project_name="playerworld18200281.github.io"

    rm -rf "/var/www/$project_name"

    echo "$con_show_str 删除playerworld18200281.github.io 完成"

    cd /var/www

    git clone "git@github.com:playerworld18200281/$project_name.git"

    echo "$con_show_str 克隆$project_name 执行完成"

    chmod -R 777 "/var/www/$project_name"

    echo "$con_show_str $project_name 权限777执行完成"
}

echo "-----函数开始执行-----"
git_new_project
echo "-----函数执行完毕-----"


