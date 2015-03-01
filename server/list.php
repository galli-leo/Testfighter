<?php
$scandir = scandir('builds');
$i = 0;
foreach ($scandir as $file)
{
if($i===0 || $i===1){
	
}else if(strpos($file, ".zip")){
	echo $file;
	
	if($i!=count($scandir)-1){
echo '<br>';
}
	
}


$i++;
}


?>