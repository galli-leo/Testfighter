<h1>Testfighter</h1>
<br>
<?php
$string = file_get_contents("list.json");
$json_a=json_decode($string,true);
$options = file_get_contents("options.json");
$optionsJ = json_decode($options, true);
foreach($json_a as $key=>$value){
	echo "<div class='app'>";
	$app = $value;
	echo "<h2>$key</h2>";
	foreach($app as $name=>$val){
		
		if($name != "time"){
			$readName = $optionsJ[$name]["Name"];
			if($readName == ""){
				$readName = $name;
			}
			if($val == ""){
				$val = "None";
			}
			echo "$readName: $val";
			echo "<br>";
		}
	}
	echo "<br><a href='upload.php?When=update'>Add Build</a>";
	echo "</div>";
}

echo "<br><a href='upload.php?When=init'>Add App</a>";


?>
<br>
<p>
© 2015 SleepyImpStudio (<a href="http://twitter.com/sleepyimpstudio")>@SleepyImpStudio</a>), Leonardo Galli (<a href="http://twitter.com/dj_lectr0")>@DJ_Lectr0</a>)
</p>