<?php 
$file = $_GET['file'];
if($file=='GreenBirdStudiosBetaLauncher'){
	echo md5_file("builds/GreenBirdStudiosBetaLauncher.jar");
}else{
echo md5_file("builds/$file.zip");
}



?>