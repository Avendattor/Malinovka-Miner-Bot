<?php
	$ACCOUNT_EXIST	= 			1000;
	$ACCOUNT_NOT_EXIST =		1001;
	$ACCOUNT_NOT_REGISTERED = 	1004;
	$SUCCESS_CONNECTION =		1002;
	$DENIDED_CONNECTION = 		1003;

	$INITIALIZATION_REQUEST = 	2000;
	$UPDATION_REQUEST =			2001;
	$SUCCESS_UPDATION = 		2002;
	$GET_DEADLINE_REQUEST =		3001;
	$GET_CURRENT_DAY_REQUEST = 3002;
	$GET_BOTSERIAL_REQUEST	= 3003;
	class Client {
		public $app_serial = '';
		public $bot_serial = '';
		public $disk_serial = '';
		public $mac_address = '';

		function SetAppSerial($data)
		{
			$this->app_serial = $data;
		}

		function GetAppSerial()
		{
			return $this->app_serial;
		}

		function SetBotSerial($data)
		{
			$this->bot_serial = $data;
		}

		function GetBotSerial()
		{
			return $this->bot_serial;
		}

		function SetDiskSerial($data)
		{
			$this->disk_serial = $data;
		}

		function GetDiskSerial()
		{
			return $this->disk_serial;
		}

		function SetMacAddress($data)
		{
			$this->mac_address = $data;
		}

		function GetMacAddress()
		{
			return $this->mac_address;
		}

	}

	$client = new Client;
	$requestType = NULL;

	if (isset($_POST['type_request']))
		if ($_POST['type_request'])
			$requestType = $_POST['type_request'];

	$isInput = TRUE;
	if (isset($_POST['app_serial']))
		if ($_POST['app_serial'])
			$client->SetAppSerial($_POST['app_serial']);
		else
			$isInput = $isInput && FALSE;
	else
		$isInput = $isInput && FALSE;

	if (isset($_POST['bot_serial']))
		if ($_POST['bot_serial'])
			$client->SetBotSerial($_POST['bot_serial']);
		else
			$isInput = $isInput && FALSE;
	else
		$isInput = $isInput && FALSE;


	if (isset($_POST['disk_serial']))
		if ($_POST['disk_serial'])
			$client->SetDiskSerial($_POST['disk_serial']);
		else
			$isInput = $isInput && FALSE;
	else
		$isInput = $isInput && FALSE;

	if (isset($_POST['mac_address']))
		if ($_POST['mac_address'])
			$client->SetMacAddress($_POST['mac_address']);
		else
			$isInput = $isInput && FALSE;
	else
		$isInput = $isInput && FALSE;

	$db_name = '';
	$db_pass = '';
	$host_name = 'localhost';
	$user_name = '';
	$table_name = 'malinovka_bot';

	$mysql = new mysqli($host_name, $user_name, $db_pass) or die(mysqli_error($mysql));
	mysqli_select_db($mysql, $db_name);

	if ($requestType != NULL)
	{
		if ($isInput == TRUE)
		{
			//$checkAccountQuery = 'SELECT * FROM malinovka_bot WHERE app_serial = "'.mysqli_real_escape_string($mysql, $client->GetAppSerial()).'"';
			$checkAccountQuery = sprintf('SELECT * FROM malinovka_bot WHERE app_serial = \'%s\'',
				mysqli_real_escape_string($mysql, $client->GetAppSerial()));
	
			$result = $mysql->query($checkAccountQuery);
	
			if ($result->num_rows > 0)
			{
				if ($requestType == $INITIALIZATION_REQUEST)
				{
					//$mysql->query($checkAccountQuery);
					// Account exist
					$row = $result->fetch_array(MYSQLI_NUM);
		
					// App serials are equations
					if ($row[1] == $client->GetAppSerial())
					{
						// Auth
						if (is_null($row[3]))
						{
							echo $ACCOUNT_NOT_REGISTERED;
						}
						else if (($row[3] == $client->GetDiskSerial()) && ($row[4] == $client->GetMacAddress()))
						{
							echo $SUCCESS_CONNECTION;
						}
						else
						{
							echo $DENIDED_CONNECTION;
						}
					}
					else
						echo 0;
				}
				else if ($requestType == $UPDATION_REQUEST)
				{
					$getDeadLineQuery = sprintf("SELECT DATE_ADD('%s', INTERVAL 7 DAY)", date("Y-m-d"));

					$result = $mysql->query($getDeadLineQuery);
					$row = $result->fetch_array(MYSQLI_NUM); //mysqli_fetch_array($result, MYSQLI_ASSOC);
					$query = "UPDATE malinovka_bot SET disk_serial = '".$client->GetDiskSerial()."', mac_address = '".$client->GetMacAddress()."', dead_line = '".$row[0]."' WHERE app_serial = '".$client->GetAppSerial()."'";
					//echo $query;
					$result = $mysql->query($query);
					if ($result == TRUE)
					{
						echo $SUCCESS_UPDATION;
					}
					//$query = sprintf('SELECT * FROM malinovka_bot WHERE app_serial = \'%s\'',
				}
				else if ($requestType == $GET_DEADLINE_REQUEST)
				{
					$getDeadlineField = "SELECT dead_line FROM malinovka_bot WHERE app_serial = '".$client->GetAppSerial()."'";
					$result = $mysql->query($getDeadlineField);
					$row = $result->fetch_array(MYSQLI_NUM);
					echo $row[0];
				}
				else if ($requestType == $GET_CURRENT_DAY_REQUEST)
				{
					echo date("Y-m-d");
				}
				else if ($requestType == $GET_BOTSERIAL_REQUEST)
				{
					$getDeadlineField = "SELECT bot_serial FROM malinovka_bot WHERE app_serial = '".$client->GetAppSerial()."'";
					$result = $mysql->query($getDeadlineField);
					$row = $result->fetch_array(MYSQLI_NUM);

					echo $row[0];
				}
			}
			else
			{
				echo $ACCOUNT_NOT_EXIST;
	
				// Insert new client
				// $insert = sprintf('INSERT INTO malinovka_bot SET app_serial = \'%s\', bot_serial = \'%s\', user_name = \'%s\', mac_address = \'%s\'',
				// 	mysqli_real_escape_string($mysql, $client->GetAppSerial()),
				// 	mysqli_real_escape_string($mysql, $client->GetBotSerial()),
				// 	mysqli_real_escape_string($mysql, $client->GetUserName()),
				// 	mysqli_real_escape_string($mysql, $client->GetMacAddress()));
	
				// mysqli_query($mysql, $insert) or die(mysqli_error($mysql));
			}
	
		}
		else
		{

		}

	}
	
?>