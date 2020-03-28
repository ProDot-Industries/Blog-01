<html>
	<head>
		<meta name="viewport" content ="width=device-width,initial-scale=1,user-scalable=yes" />
		<script src='raphael-2.1.4.min.js'></script>
    	<script src='justgage.js'><meta name="viewport" content ="width=device-width,initial-scale=1,user-scalable=yes" /></script>
		
		<?php
			$page = $_SERVER['PHP_SELF'];
			include "session.php";
			//page auto refresh time
			$sec = "10";
			date_default_timezone_set("Asia/Kolkata");
			$showTime = date("H:i:s");
			
			$servername = "localhost";
    		$dbname = "Project";
    		$username = "alpha";
    		$password = "alpha";
    		$conn = new mysqli($servername,$username,$password,$dbname);
    		if ($conn->connect_error) {
        		die("connection failed" . $conn->connect_error);
    		}
    		$sql1 = "SELECT * FROM view5 order by `Sensor ID` ASC";
    		$row_Event_ID 		= array (0,0,0,0,0,0,0,0,0,0,0,0,0);
    		$row_Sensor_ID 		= array (0,0,0,0,0,0,0,0,0,0,0,0,0);
    		$row_Sensor_Value 	= array (0,0,0,0,0,0,0,0,0,0,0,0,0);
			$row_CurrentTime 	= array (0,0,0,0,0,0,0,0,0,0,0,0,0);
			
			$sql2 = "SELECT * FROM ActionTable";
			$row_Event2_ID 		= array (0,0,0,0);
    		$row_Sensor2_ID 	= array (0,0,0,0);
    		$row_Sensor2_Value 	= array (0,0,0,0);
			$row_ActionTime 	= array (0,0,0,0);

			$sql3 = "SELECT * FROM view6 order by `Sensor ID` ASC";
			$row_Event3_ID 		= array (0,0,0,0,0,0,0,0);
    		$row_Sensor3_ID 	= array (0,0,0,0,0,0,0,0);
    		$row_Sensor3_Value 	= array (0,0,0,0,0,0,0,0);
			$row_CurrentTime3 	= array (0,0,0,0,0,0,0,0);

    		if ($result = $conn->query($sql1)) {
        		$x = 0;
        		while ($row = $result->fetch_assoc()) {
            		$row_Event_ID[$x] = $row["Event ID"];
            		$row_Sensor_ID[$x] = $row["Sensor ID"];
            		$row_Sensor_Value[$x] = $row["Sensor Value"];
		            $row_CurrentTime[$x] = $row["EventTime"];
            		++$x;
        		}
        		$result->free();
			}
			
			if ($result = $conn->query($sql2)) {
        		$x = 0;
        		while ($row = $result->fetch_assoc()) {
            		$row_Action_ID[$x] = $row["Action ID"];
            		$row_Action_Event[$x] = $row["Action Event"];
            		$row_Action_Value[$x] = $row["Action Value"];
		            $row_ActionTime[$x] = $row["Action Time"];
            		++$x;
        		}
        		$result->free();
			}

			if ($result = $conn->query($sql3)) {
        		$x = 0;
        		while ($row = $result->fetch_assoc()) {
            		$row_Event3_ID[$x] = $row["Event ID"];
            		$row_Sensor3_ID[$x] = $row["Sensor ID"];
            		$row_Sensor3_Value[$x] = $row["Sensor Value"];
		            $row_CurrentTime3[$x] = $row["EventTime"];
            		++$x;
        		}
        		$result->free();
			}

			$temp = ($row_Sensor_Value[6]+$row_Sensor_Value[7]+$row_Sensor_Value[8])/3;
			$light = ($row_Sensor_Value[9]+$row_Sensor_Value[10]+$row_Sensor_Value[11]+$row_Sensor_Value[12])/4;

			echo "
				<style>
					a {
						color: #323232;
						text-decoration: none;
					}
					body {
						font-family: 'Open Sans', sans-serif;
						font-weight: 300;
						line-height: 1.42em;
						color:#112d4e;
						background-color:#effffb;
					}
					h1 {
						font-size:3em;
						font-weight: 300;
						line-height:1em;
						text-align: center;
						color: #112d4e;
					}
					h2 {
						font-size:2em;
						font-weight: 300;
						text-align: center;
						display: block;
						line-height:1em;
						padding-bottom: 2em;
						color: #112d4e;
					}
					h2 a {
						font-weight: 700;
						text-transform: uppercase;
						color: #1b262c;
						text-decoration: none;
					}
					.blue { color: #185875; }
					.yellow { color: #FFF842; }
					.container th h1 {
						font-weight: bold;
						font-size: 1em;
						text-align: left;
						color: #185875;
					}
					.container td {
						font-weight: normal;
						font-size: 1em;
						-webkit-box-shadow: 0 2px 2px -2px #0E1119;
						-moz-box-shadow: 0 2px 2px -2px #0E1119;
						box-shadow: 0 2px 2px -2px #0E1119;
					}
					.container {
						border-radius: 25px;
						text-align: left;
						overflow: hidden;
						width: 50%;
						margin: 0 auto;
						display: table;
						padding: 0 0 8em 0;
					}
					.container td, .container th {
						padding-bottom: 2%;
						padding-top: 2%;
						padding-left:2%;
					}
					/* Background-color of the odd rows */
					.container tr:nth-child(odd) {
						background-color: #f9f7f7;
					}
					/* Background-color of the even rows */
					.container tr:nth-child(even) {
						background-color: #dbe2ef;
					}
					.container th {
						background-color: #a8e6cf;
					}
					.container td:first-child { color: #112d4e; }
					.container tr:hover {
						background-color: #ecfbfc;
						-webkit-box-shadow: 0 6px 6px -6px #ecfbfc;
						-moz-box-shadow: 0 6px 6px -6px #ecfbfc;
						box-shadow: 0 6px 6px -6px #ecfbfc;
					}
					.container td:hover {
						background-color:  #ecfbfc;
						color: #403E10;
						font-weight: bold;
						box-shadow: #ecfbfc -1px 1px, #ecfbfc -2px 2px, #ecfbfc -3px 3px, #ecfbfc -4px 4px, #ecfbfc -5px 5px, #ecfbfc -6px 6px;
						transform: translate3d(6px, -6px, 0);
						transition-delay: 0s;
						transition-duration: 0.4s;
						transition-property: all;
						transition-timing-function: line;
					}
					div.relative {
						color: white;
						position: absolute;
						left: 10px;
						top: 100px;
					}
					div.relative1 {
						position: absolute;
						right: 10px;
						top: 100px;
					}
					.button{
						border: none;
						padding: 10px;
						text-align: center;
						text-decoration: none;
						display: inline-block;
						font-size: 16px;
						margin: 4px 2px;
						cursor: pointer;
						color: white;
						background-color: #fff2e5;
						border-radius: 25px;
						width: 70px;
					}

					.button1{
						position: absolute;
						left: 30px;
						top: 500px;
					}
					.button2{
						position: absolute;
						left: 120px;
						top: 500px;
					}
					.button3{
						position: absolute;
						left: 210px;
						top: 500px;
					}
					.button6{
						position: absolute;
						right:30px;
						top: 500px;
					}
					.button5{
						position: absolute;
						right: 120px;
						top: 500px;
					}
					.button4{
						position: absolute;
						right: 210px;
						top: 500px;
					}
										
					@media (max-width: 800px) {
						.container td:nth-child(4),
						.container th:nth-child(4) { display: none; }
					}
				</style>
			</head>
		<body>
			<h1> <span class='yellow'></pan></h1>
			<h2>Monitor & Control Panel</h2>
			<table class='container'>
				<thead>
					<tr>
						<th><h1>Event ID</h1></th>
						<th><h1>Sensor ID</h1></th>
						<th><h1>Sensor Value</h1></th>
						<th><h1>Time</h1></th>
					</tr>
				</thead>
				<tbody>
					<tr>
						<td>$row_Event_ID[0]</td>
						<td>$row_Sensor_ID[0]</td>
						<td>$row_Sensor_Value[0]</td>
						<td>$row_CurrentTime[0]</td>
					</tr>
					<tr>
						<td>$row_Event_ID[1]</td>
						<td>$row_Sensor_ID[1]</td>
						<td>$row_Sensor_Value[1]</td>
						<td>$row_CurrentTime[1]</td>
					</tr>
					<tr>
						<td>$row_Event_ID[2]</td>
						<td>$row_Sensor_ID[2]</td>
						<td>$row_Sensor_Value[2]</td>
						<td>$row_CurrentTime[2]</td>
					</tr>
					<tr>
						<td>$row_Event_ID[3]</td>
						<td>$row_Sensor_ID[3]</td>
						<td>$row_Sensor_Value[3]</td>
						<td>$row_CurrentTime[3]</td>
					</tr>
					<tr>
						<td>$row_Event_ID[4]</td>
						<td>$row_Sensor_ID[4]</td>
						<td>$row_Sensor_Value[4]</td>
						<td>$row_CurrentTime[4]</td>
					</tr>
					<tr>
						<td>$row_Event_ID[5]</td>
						<td>$row_Sensor_ID[5]</td>
						<td>$row_Sensor_Value[5]</td>
						<td>$row_CurrentTime[5]</td>
					</tr>
					<tr>
						<td>$row_Event_ID[6]</td>
						<td>$row_Sensor_ID[6]</td>
						<td>$row_Sensor_Value[6]</td>
						<td>$row_CurrentTime[6]</td>
					</tr>
					<tr>
						<td>$row_Event_ID[7]</td>
						<td>$row_Sensor_ID[7]</td>
						<td>$row_Sensor_Value[7]</td>
						<td>$row_CurrentTime[7]</td>
					</tr>
					<tr>
						<td>$row_Event_ID[8]</td>
						<td>$row_Sensor_ID[8]</td>
						<td>$row_Sensor_Value[8]</td>
						<td>$row_CurrentTime[8]</td>
					</tr>
					<tr>
						<td>$row_Event_ID[9]</td>
						<td>$row_Sensor_ID[9]</td>
						<td>$row_Sensor_Value[9]</td>
						<td>$row_CurrentTime[9]</td>
					</tr>
					<tr>
						<td>$row_Event_ID[10]</td>
						<td>$row_Sensor_ID[10]</td>
						<td>$row_Sensor_Value[10]</td>
						<td>$row_CurrentTime[10]</td>
					</tr>
					<tr>
						<td>$row_Event_ID[11]</td>
						<td>$row_Sensor_ID[11]</td>
						<td>$row_Sensor_Value[11]</td>
						<td>$row_CurrentTime[11]</td>
					</tr>
					<tr>
						<td>$row_Event_ID[12]</td>
						<td>$row_Sensor_ID[12]</td>
						<td>$row_Sensor_Value[12]</td>
						<td>$row_CurrentTime[12]</td>
					</tr>
				</tbody>
			</table>
			<table class='container'>
				<thead>
					<tr>
						<th><h1>Event ID</h1></th>
						<th><h1>Sensor ID</h1></th>
						<th><h1>Sensor Value</h1></th>
						<th><h1>Time</h1></th>
					</tr>
				</thead>
				<tbody>
					<tr>
						<td>$row_Event3_ID[0]</td>
						<td>$row_Sensor3_ID[0]</td>
						<td>$row_Sensor3_Value[0]</td>
						<td>$row_CurrentTime3[0]</td>
					</tr>
					<tr>
						<td>$row_Event3_ID[1]</td>
						<td>$row_Sensor3_ID[1]</td>
						<td>$row_Sensor3_Value[1]</td>
						<td>$row_CurrentTime3[1]</td>
					</tr>
					<tr>
						<td>$row_Event3_ID[2]</td>
						<td>$row_Sensor3_ID[2]</td>
						<td>$row_Sensor3_Value[2]</td>
						<td>$row_CurrentTime3[2]</td>
					</tr>
					<tr>
						<td>$row_Event3_ID[3]</td>
						<td>$row_Sensor3_ID[3]</td>
						<td>$row_Sensor3_Value[3]</td>
						<td>$row_CurrentTime3[3]</td>
					</tr>
				</tbody>
			</table>

			<table class='container'>
				<thead>
					<tr>
						<th><h1>Action ID</h1></th>
						<th><h1>Action Event</h1></th>
						<th><h1>Action value</h1></th>
						<th><h1>Time</h1></th>
					</tr>
				</thead>
				<tbody>
					<tr>
						<td>$row_Action_ID[0]</td>
						<td>$row_Action_Event[0]</td>
						<td>$row_Action_Value[0]</td>
						<td>$row_ActionTime[0]</td>
					</tr>
					<tr>
						<td>$row_Action_ID[1]</td>
						<td>$row_Action_Event[1]</td>
						<td>$row_Action_Value[1]</td>
						<td>$row_ActionTime[1]</td>
					</tr>
					<tr>
						<td>$row_Action_ID[2]</td>
						<td>$row_Action_Event[2]</td>
						<td>$row_Action_Value[2]</td>
						<td>$row_ActionTime[2]</td>
					</tr>
					<tr>
						<td>$row_Action_ID[3]</td>
						<td>$row_Action_Event[3]</td>
						<td>$row_Action_Value[3]</td>
						<td>$row_ActionTime[3]</td>
					</tr>
					<tr>
						<td>$row_Action_ID[4]</td>
						<td>$row_Action_Event[4]</td>
						<td>$row_Action_Value[4]</td>
						<td>$row_ActionTime[4]</td>
					</tr>
					<tr>
						<td>$row_Action_ID[5]</td>
						<td>$row_Action_Event[5]</td>
						<td>$row_Action_Value[5]</td>
						<td>$row_ActionTime[5]</td>
					</tr>
					<tr>
						<td>$row_Action_ID[6]</td>
						<td>$row_Action_Event[6]</td>
						<td>$row_Action_Value[6]</td>
						<td>$row_ActionTime[6]</td>
					</tr>
					<tr>
						<td>$row_Action_ID[7]</td>
						<td>$row_Action_Event[7]</td>
						<td>$row_Action_Value[7]</td>
						<td>$row_ActionTime[7]</td>
					</tr>
				</tbody>
			</table>
			
			<div id='smallbudy' class='relative' style='width:300px; height:400px'></div>
			<div id='smallbudy1' class='relative1' style='width:300px; height:400px'></div>

			
			<button class='button button1' ><a color: #2c87f0; href=pump1.php>Toggle Pump 1</button>
			<button class='button button2' ><a href=pump2.php>Toggle Pump 2</button>
			<button class='button button3' ><a href=pump3.php>Toggle Pump 3</button>
				<script>
					
					var T = new JustGage({
						id: 'smallbudy',
						value:$temp,
						min: 0,
						max: 500,
						title: 'Temperature'
					});

					var H= new JustGage({
						id: 'smallbudy1',
						value:$light,
						min: 0,
						max: 1000,
						title: 'Light'
					});
				</script>"
			?>
		</body>
</html>

<html>
    <head>
		<meta name="viewport" content ="width=device-width,initial-scale=1,user-scalable=yes" />
        <meta http-equiv="refresh" content="<?php echo $sec?>;URL='<?php echo $page?>'">
    </head>
<body>
    <script src="js/jquery.min.js"></script>
	<script>
		$('.form-control').each(function () {
			floatedLabel($(this));
		});

		$('.form-control').on('input', function () {
			floatedLabel($(this));
		});

		function floatedLabel(input) {
			var $field = input.closest('.form-group');
			if (input.val()) {
				$field.addClass('input-not-empty');
			} else {
				$field.removeClass('input-not-empty');
			}
		}
    </script>
    <div class="form-btn">
		<button class="submit-btn" >
			<a href = logout.php> logout </a>
		</button>
    </div>

</body>
</html>
