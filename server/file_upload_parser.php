<?php
function printOption($type, $name, $key){
	switch($type){
		case "yes/no":
			echo "<input type=\"checkbox\" name=\"$key\" value=\"true\">$name";
			break;
		case "text":
			echo "<textarea rows=\"4\" cols=\"50\" name=\"$key\">$name</textarea>";
			break;
		case "textfield":
			echo "<input type=\"text\" name=\"$key\" value=\"$name\">";
			break;
	}
	echo "<br>";
	
}
$fileName = $_FILES["file1"]["name"]; // The file name
$fileTmpLoc = $_FILES["file1"]["tmp_name"]; // File in the PHP tmp folder
$fileType = $_FILES["file1"]["type"]; // The type of file it is
$fileSize = $_FILES["file1"]["size"]; // File size in bytes
$fileErrorMsg = $_FILES["file1"]["error"]; // 0 for false... and 1 for true
echo "os ".$_POST["os"];
if (!$fileTmpLoc) { // if file not chosen
    echo "ERROR: Please browse for a file before clicking the upload button.";
    //exit();
}
$string = file_get_contents("options.json");

$jsonIterator = json_decode($string);

if(move_uploaded_file($fileTmpLoc, "test/$fileName")){
    echo "$fileName upload is complete";
    
    $zip = new ZipArchive;
	$res = $zip->open("test/$fileName");
	if ($res === TRUE) {
	$zip->extractTo('test/'.str_replace('.zip', '', $fileName));
	$zip->close();
	?>
	
	<form action="build_submit.php" method="post">
		<?php
		echo $json_a["bUsesSteam"];
		foreach ($jsonIterator as $key => $val) {
			
			if($val->When == $_POST["When"] || $val->When == "all"){
				printOption($val->Type, $val->Name, $key);
			}
		}
		?>
		<input type="hidden" name="filename" value="<?php echo str_replace('.zip', '', $fileName)?>"></input>
		<input type="hidden" name="When" value="<?php echo $_POST["When"]?>"></input>
		<input type="hidden" name="os" value="<?php echo $_POST["os"]?>"></input>
		<input type="submit" value="Submit">
	</form>
	<?php
	echo 'woot!';
	
	} else {
		echo 'doh!';
	}
    
} else {
    echo "move_uploaded_file function failed";
}
?>