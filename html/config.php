<?php
   define('DB_SERVER', 'localhost:3306');
   define('DB_USERNAME', 'alpha');
   define('DB_PASSWORD', 'alpha');
   define('DB_DATABASE', 'Project');
   $db = mysqli_connect(DB_SERVER,DB_USERNAME,DB_PASSWORD,DB_DATABASE);
?>