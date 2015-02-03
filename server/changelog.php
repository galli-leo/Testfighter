<?php

$file = $_GET['file'].'changelog.txt';
$change = file_get_contents("builds/$file");
echo $change;
?>