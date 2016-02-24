<?php
dsfsdfsfd
$br = (php_sapi_name() == "cli")? "":"<br>";
//confirm_binwin_compiled("sdfsdfsdf");


ini_set("display_errors", "on");
error_reporting(E_ALL);
$config = array(
	'ROOT' => dirname(__FILE__),
);
$obj = new bw_framework();
echo $obj->on2($config);
?>
