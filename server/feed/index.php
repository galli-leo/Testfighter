<?php
require_once("classes/class.FeedContent.php");
$feedC = new FeedContent("items.json");


if(isset($_GET["title"])){
	//PUT
	$feedC->addItem($_GET["title"], $_GET["text"]);
}else if(isset($_GET["num"])){
	//GET
	$feedC->displayFeed($_GET["num"]);
}

$feedC->writeOut();

?>