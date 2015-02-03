<?php
require_once("feed/classes/class.FeedContent.php");
$feedC = new FeedContent("feed/items.json");
$filename= $_POST['filename'];
echo $filename;
echo "<br>";

$dist= $_POST['dist'];
$changelog= $_POST['changelog'];
$os = $_POST["os"];
echo $os;
echo "<br>";
/*$distt = "";
for($i = 0; $i < count($dist); $i++)
{
	$distt .= $dist[$i].'\\n';
	echo $dist[$i];
}
 file_put_contents (  'test/'.$filename.'/dependencies.txt' ,  $distt);*/
  
$string = file_get_contents("$os"."_list.json");
echo $os;
echo $filename;
$jsonA = json_decode($string, true);
$stringo = file_get_contents("options.json");
$jsonIterator = json_decode($stringo, true);
$root;
if(array_key_exists($filename, $jsonA)){
	//We are updating
	$root["time"] = time();
	foreach ($jsonIterator as $key => $val) {
		if($val["When"] == "update" && isset($_POST[$key])){
		$root[$key] = $_POST[$key];
		}else{
			$root[$key] = $jsonA[$filename][$key];
		}	
	}
	$feedC->addItem("Version " . $_POST["version"] . " of " . $filename . " released!", "Changelog:\n".$_POST["changelog"]);

}else{
	//We are creating new
	$root["time"] = time();

	foreach ($jsonIterator as $key => $val) {
			
		$root[$key] = $_POST[$key];
			
	}
	$root["version"] = "1.0.0";
	$feedC->addItem("$filename released!", "Description:\n".$_POST["description"]);
}
$feedC->writeOut();
$jsonA[$filename] = $root;
$jsonStr = json_encode($jsonA);
file_put_contents($os.'_list.json', $jsonStr);

/*Create hashes

*/
unlink('test/'.$filename.'.zip');
rename('test/'.$filename, 'builds/'.$os .'/'.$filename);
//echo "http://leonardogalli.ch/beta/hashCreate.php?filename=$filename<br>";

$array = array();
$iterator = new RecursiveIteratorIterator(
    new RecursiveDirectoryIterator("builds/$os/$filename/", 
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
		$hash = md5_file("builds/$os/$filename/$key");
		echo "hash of builds/$filename/$key is $hash";
		echo "<br>";
		$hashes["$filename/$key"] = $hash;
	}else{
		echo "$key is dir";
		echo "<br>";
	}
}

$hashesJsonString = json_encode($hashes);
file_put_contents("builds/hashes/$os/$filename.json", $hashesJsonString);
?>