<?php
	class MyDB extends SQLite3 {
    	function __construct() {
        	$this->open('IoT.db');
      	}
   	}
   	error_log("Rohitha");
   
   	$db = new MyDB();

   	if(!$db) {
    	echo $db->lastErrorMsg();
   	}
	if(isset($_POST)) {
		error_log(print_r($_POST, true));
		$moisture = $_POST["moisture"];
		$humidity = $_POST["humidity"];
		$temperature = $_POST['temperature'];
		$sql = "insert into irrigation('moisture','humidity','temperature') values('$moisture','$humidity','$temperature');";
		error_log(print_r($sql,true));
		if($db->query($sql))
			echo "success";
	}
	echo "Rohitha";
?>