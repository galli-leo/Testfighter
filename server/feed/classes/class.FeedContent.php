<?php

class FeedContent{
	
	
	private $json;
	private $fileName = "";
	function __construct($file) {
    	$string = file_get_contents($file);
	    $this->json = json_decode($string, true);
	    if(!isset($this->json)){
		    $this->json = array();
	    }
	    $this->fileName = $file;
	    
	}
	private static function sortFunction( $a, $b ) {
    	return  strtotime($b["date"]) - strtotime($a["date"]);
	}
	function displayFeed($num){
		//TODO resolve display
		$this->sortJson();
		$display = array_slice($this->json, 0, $num);
		$encodedDisplay = json_encode($display);
		echo $encodedDisplay;
	}
	function addItem($title, $text){
		echo "Adding item ";
		$item["title"] = $title;
		$item["text"] = $text;
		$item["date"] = date('d-m-Y h:i:s', time());
		echo strtotime($item["date"]);
		array_push($this->json, $item);
		$this->sortJson();
	}
	function writeOut(){
		$encoded = json_encode($this->json);
		file_put_contents($this->fileName, $encoded);
	}
	function sortJson(){
		usort($this->json, array("FeedContent","sortFunction"));
		//$this->json = array_reverse($this->json);
	}
	
	
}
?>