<?php
    include("config.php");
    session_start();
    if($_SERVER["REQUEST_METHOD"] == "POST") {
        $myusername = mysqli_real_escape_string($db,$_POST['username']);
        $mypassword = mysqli_real_escape_string($db,$_POST['password']);
        $myemail = mysqli_real_escape_string($db,$_POST['email']); 
        $myphone = mysqli_real_escape_string($db,$_POST['phone']);
        $sql = "INSERT INTO UserTable (`User Name`,`User password`,`User Email`,`User Phone`) VALUES ('$myusername','$mypassword','$myemail','$myphone')";
        if (mysqli_query($db,$sql) === TRUE) {
            echo "New record created successfully";
            header("location: index.php");
        } else {
            echo "Error: " . mysqli_error($db);
        }
    }
?>

<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="utf-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1">
	<title>Registration</title>
	<link href="https://fonts.googleapis.com/css?family=Montserrat:400,700" rel="stylesheet">
	<link type="text/css" rel="stylesheet" href="css/bootstrap.min.css" />
	<link type="text/css" rel="stylesheet" href="css/style.css" />
</head>

<body>
	<div id="booking" class="section">
		<div class="section-center">
			<div class="container">
				<div class="row">
					<div class="booking-form">
						<div class="form-header">
							<h1>Create Your Account</h1>
						</div>
						<form action="" method="post">
							<div class="form-group">
								<input class="form-control" type="text" name="username">
								<span class="form-label">Name (unique)</span>
							</div>
                            <div class="form-group">
								<input class="form-control" type="text" name="password">
								<span class="form-label">Password</span>
							</div>
                            <div class="form-group">
								<input class="form-control" type="text" name="email">
								<span class="form-label">email</span>
							</div>
                            <div class="form-group">
								<input class="form-control" type="text" name="phone">
								<span class="form-label">Phone Number</span>
							</div>
							<div class="form-btn">
								<button class="submit-btn">
									Create
								</button>
                                <input type = "submit" value = ""><br />
							</div>
						</form>
					</div>
				</div>
			</div>
		</div>
	</div>

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
</body>
</html>