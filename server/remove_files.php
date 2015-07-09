<?php
ini_set('display_errors',1);


function doesDirectoryOnlyContainEmptyFolders($path) {
  if(is_dir($path)) {
    $directoryContent = scandir($path);
    if (count($directoryContent) <= 2) {
      return true;
    }
    else {
      foreach ($directoryContent as $subPath) {
        if($subPath !== '.' && $subPath !== '..' && !doesDirectoryOnlyContainEmptyFolders("$path/$subPath")) {
	        echo "Subpath not empty: $path $subPath";
          return false;
        }
      }
      return true;
    }
  }
  echo "Found file for path: $path";
  return false;
}

function deleteDirectory($dir) {
    if (!file_exists($dir)) {
        return true;
    }

    if (!is_dir($dir)) {
        return unlink($dir);
    }

    foreach (scandir($dir) as $item) {
        if ($item == '.' || $item == '..') {
            continue;
        }

        if (!deleteDirectory($dir . DIRECTORY_SEPARATOR . $item)) {
            return false;
        }

    }

    return rmdir($dir);
}
	
function deleteEmptyFoldersRecursivelyUpAndDown($path) {
  if (is_dir($path)) {
    if(doesDirectoryOnlyContainEmptyFolders($path)) {
      deleteDirectory($path);
      echo "Deleted empty folder: $path<br>";
      /*$parentFolder = substr($path, 0, strripos ($path, '/'));
      deleteEmptyFoldersRecursivelyUpAndDown($parentFolder);*/
    }
    else {
      $directoryContent = scandir($path);
      foreach ($directoryContent as $subPath) {
	      if($subPath !== '.' && $subPath !== '..'){
	      	echo "Searching folders: $subPath<br>";
		  	deleteEmptyFoldersRecursivelyUpAndDown("$path/$subPath");
        }
      }
    }
  }
}	

if($_POST["password"]=="testfighter2015"){
	echo("test");
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
	//var_dump(doesDirectoryOnlyContainEmptyFolders("builds/$os/"));
	//var_dump(doesDirectoryOnlyContainEmptyFolders("builds/$os/JumpNoMore/Binaries/"));
	deleteEmptyFoldersRecursivelyUpAndDown("builds/$os");




}
?>