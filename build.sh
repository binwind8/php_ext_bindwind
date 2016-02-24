#! /bin/sh
make clean;
#/usr/local/php5/bin/phpize;
./configure --with-php-config=/usr/local/php5/bin/php-config;
make && make install;
/etc/init.d/php-fpm restart;
php ./binwin.php;

