<?php
	
if($_POST["password"]=="testfighter2015"){
	$os = $_POST["os"];
	$files_str = $_POST['files'];
	$files = explode(";", $files_str);
	var_dump($_POST);
	foreach ($files as $file){
		
		if($file != ""){
			chmod("builds/$os/$file",0755); //Change the file permissions if allowed
    		unlink("builds/$os/$file"); //remove the file
    		echo "Succesfully removed file: builds/$os/$file\n";
		}
	}
	
}
?>