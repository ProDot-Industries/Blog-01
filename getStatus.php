<?php

$servername = "localhost";
$username = "alpha";
$password = "alpha";
$dbname = "Project";
$apiKeyInternal = "1qaz2wsx3edc";
    $action_id = $_GET["action_id"];
    // Check connection
   // if ($db->connect_error) {
     //   die("Connection failed: " . $db->connect_error);
    //}
   
    $sql = "SELECT `Action Value` FROM ActionTable WHERE `Action ID`=$action_id";
    $conn = new mysqli($servername,$username,$password,$dbname);

    $result = $conn->query($sql);
    
    if ($result->num_rows > 0) {
    
    while ($row = $result->fetch_assoc())
    {
        echo $row['Action Value'];
    }
        
    } else {
        echo "Error:" . $sql . "<br>" . $db->error;
    }
    
    $conn->close();
?>