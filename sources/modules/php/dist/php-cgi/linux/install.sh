#!/usr/bin/env bash

if [ -n "$(command -v php-cgi --version)" ]; then
    exit 0
elif [ -n "$(command -v yum)" ]; then
    sudo yum install php-cgi
elif [ -n "$(command -v apt-get)" ]; then
    sudo apt-get install php-cgi
else
    echo -e "no package installer found on your distribution"
    exit 1
fi
exit 0