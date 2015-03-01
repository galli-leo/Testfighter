<?php
$filename = $filename= $_GET['filename'];
$array = array();
$iterator = new RecursiveIteratorIterator(
    new RecursiveDirectoryIterator("builds/$filename/", 
        RecursiveDirectoryIterator::SKIP_DOTS),
    RecursiveIteratorIterator::SELF_FIRST);
foreach ($iterator  as $item) {
    // Note SELF_FIRST, so array keys are in place before values are pushed.

        $subPath = $iterator->getSubPathName();
            if($item->isDir()) {
                // Create a new array key of the current directory name.
                $array[$subPath] = array();
            } else {
                // Add a new element to the array of the current file name.
                $array[$subPath][] = $subPath;
            }
    }
$hashes;
foreach ($array as $key=>$val){
	if(!is_dir("builds/$filename/$key")){
		$hash = md5_file("builds/$filename/$key");
		echo "hash of builds/$filename/$key is $hash";
		echo "<br>";
		$hashes["$filename/$key"] = $hash;
	}else{
		echo "$key is dir";
		echo "<br>";
	}
}

$hashesJsonString = json_encode($hashes);
file_put_contents("builds/hash$filename.json", $hashesJsonString);
?>