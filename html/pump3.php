<?php
    include "config.php";
    if ($conn->connect_error) {
        die("connection failed" . $conn->connect_error);
    }
    $sql1 = "select * from ActionTable where `Action ID` = 6003";
    $sql2 = "update ActionTable set `Action Value`=1 where `Action ID` = 6003";
    $sql3 = "update ActionTable set `Action Value`=0 where `Action ID` = 6003";
    $val = 0;
    if ($result = $conn->query($sql1)) {
        $row = $result->fetch_assoc();
        $val = $row["Action Value"];
        if ($val == 0) {
            if ($res1 = $conn->query($sql2)) {
                echo "true1";
                header("Location: frontend2.php");
            }
            else {
                echo "failed to execute";
            }
        }
        else {
            if ($res1 = $conn->query($sql3)) {
                echo "true2";
                header("Location: frontend2.php");
            }
            else {
                echo "failed to execute";
            }
        }
    }
?>