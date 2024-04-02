<?php
Class dbObj {

	/* Database connection start */
	var $servername;
	var $username;
	var $password;
	var $dbname;
	var $conn;
    
	function getConnstring() {
		
		$config = parse_ini_file("gateway_config.ini", true);
		$this->servername = $config['database']['host'];
		$this->username = $config['database']['username'];
		$this->password = $config['database']['password'];
		$this->dbname = $config['database']['dbname'];

		$con = mysqli_connect($this->servername, $this->username, $this->password, $this->dbname) or die("Connection failed: " . mysqli_connect_error());

		/* check connection */
		if (mysqli_connect_errno()) {
			printf("Connect failed: %s\n", mysqli_connect_error());
			exit();
		} else {
			$this->conn = $con;
		}
		return $this->conn;
	}
}

?>