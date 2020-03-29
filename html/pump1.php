<?php
    include "config.php";
    if ($db->connect_error) {
        die("connection failed" . $db->connect_error);
    }
    $sql1 = "select * from ActionTable where `Action ID` = 6001";
    $sql2 = "update ActionTable set `Action Value`=1 where `Action ID` = 6001";
    $sql3 = "update ActionTable set `Action Value`=0 where `Action ID` = 6001";
    $val = 0;
    if ($result = $db->query($sql1)) {
        $row = $result->fetch_assoc();
        $val = $row["Action Value"];
        if ($val == 0) {
            if ($res1 = $db->query($sql2)) {
                echo "true1";
                header("Location: main.php");
            }
            else {
                echo "failed to execute";
            }
        }
        else {
            if ($res1 = $db->query($sql3)) {
                echo "true2";
                header("Location: main.php");
            }
            else {
                echo "failed to execute";
            }
        }
    }
?>