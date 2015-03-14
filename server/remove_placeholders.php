<?php
ini_set('display_errors', 1);
	$dir = new RecursiveDirectoryIterator(getcwd());
    $ite = new RecursiveIteratorIterator($dir);
    //$files = new RegexIterator($ite, "placeholder.*", RegexIterator::GET_MATCH);
    
    foreach($ite as $file) {
        if(strpos($file, 'placeholder')!==FALSE && strpos($file, 'remove')===FALSE){
        	//echo $file;
        	//chmod($file, 0775);
	        unlink($file);
        }
    }
    echo "All Placeholders successfully removed!";
?>