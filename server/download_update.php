<?php

$os = $_GET["os"];
$file = $_GET["file"];
header("Content-Length: ".filesize("builds/apps/$os/$file"));
header("Content-Type: application/zip");


echo file_get_contents("builds/apps/$os/$file");

?>