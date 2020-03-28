<?php
    include "config.php";
    $apiKeyInternal = "1qaz2wsx3edc";
    $apiKeyExternal = $SensorID = $SensorValue = "";

    if ($_SERVER["REQUEST_METHOD"] == "POST") {
        $apiKeyExternal = test_input($_POST["apikey"]);
        if ($apiKeyExternal == $apiKeyInternal) {
            $SensorID = test_input($_POST["SensorID"]);
            $SensorValue = test_input($_POST["SensorValue"]);


            if ($conn->connect_error) {
                die("connection failed" . $conn->connect_error);
            }
            $sql = "INSERT INTO `EventTable` (`Sensor ID`, `Sensor Value`) VALUES ($SensorID,$SensorValue)";
            if ($conn->query($sql) === TRUE) {
                echo "new record";
            }
            else {
                echo "Error: " . $sql . "<br>" . $conn->error;
            }
            $conn->close();
        }
        else {
            echo "Wrong API";
        }
    }
    else {
        echo "no data posted";
    }

    function test_input($data) {
        $data = trim($data);
        $data = stripslashes($data);
        $data = htmlspecialchars($data);
        return $data;
    }
?>