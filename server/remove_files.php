<?php
if($_POST["password"]=="testfighter2015"){
	$files_str = $_POST['files'];
	$files = explode(";", $files_str);
	foreach ($files as $file){
		if($file != ""){
			chmod($file,0755); //Change the file permissions if allowed
    		unlink($file); //remove the file
		}
	}
	echo "Succesfully removed files";
}
?>