<?php
mysql_connect("localhost","root","")or die("could not connect");
mysql_select_db("rohitha")or die("could not connect database");

   $sql = "SELECT * FROM irrigation LIMIT 10 OFFSET (SELECT COUNT(*) FROM irrigation)-10;";
   $result=mysql_query($sql);
   $response = array();
   $moisture = array();
   $humidity = array();
   $temperature = array();
   while($row=mysql_fetch_array($result)) {
      $moist_value = array();
      $humid_value = array();
      $temp_value = array();
      array_push($moist_value, $row["time"]);
      array_push($moist_value, $row["moisture"]);
      array_push($humid_value, $row["time"]);
      array_push($humid_value, $row["humidity"]);
      array_push($temp_value, $row["time"]);
      array_push($temp_value, $row["temperature"]);
      array_push($moisture, $moist_value);
      array_push($humidity, $humid_value);
      array_push($temperature, $temp_value);
   }
   array_push($response, $moisture);
   array_push($response, $humidity);
   array_push($response, $temperature);
   echo json_encode($response);
   
?>
