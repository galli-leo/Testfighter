<?php 
$app = $_GET['app'];
$os = $_GET['os'];

echo md5_file("builds/apps/$os/$app.zip");




?>