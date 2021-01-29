#!/usr/local/bin/php -q

<?php

	/**
	* 
	*/
	class PduBase
	{
		public $startflag = 123456789;
		public $terminal_token ;
		public $command_id  ;
		public $seq_id ;
		public $length ;
		public $body ;
		
	}

	/**
	* 
	*/
	class TcpClient
	{
		
		function __construct(argument)
		{
			# code...
		}

  	    $buff_len = 1024 ;
    	$recv_buff='';

    	const basic_length_unit = 4;
	    const pdu_header_length = 20;
	        
    	$socket=socket_create(AF_INET,SOCK_STREAM,SOL_TCP);

    	public function connect($ip, $port)
    	{
    		if(socket_connect($socket, $ip, $port)){
    			return 0 ;
    		}else{
    			return -1 ;
    		}
    	}

    	public function send($pdubase)
    	{
    		$buffer = pack("N",$pdubase.startflag, $pdubase.terminal_token, $pdubase.command_id, $pdubase.seq_id, $pdubase.length);
    		$buffer += $pdubase.body ;
    		socket_write($socket, $buffer, strlen($buffer));
    	}

    	public function recv()
    	{

	        $offset = 0 ;
	        while ( True ) {

		        $recv_buff=socket_read($socket,$buff_len, PHP_BINARY_READ);
		        $pdubase = new PduBase();
		        if (strlen($recv_buff)>=4) {
					$binary_startflag = substr($recv_buff, $offset, $basic_length_unit);
					if (unpack("N", $binary_startflag) != pdubase.startflag) {
						echo "wrong startflag";
						return null;
					}			
					if (strlen($recv_buff)>=$pdu_header_length) {
						//pdu header ready.
						$offset += self::basic_length_unit ;
						$pdubase.terminal_token = unpack("N", substr($recv_buff, $offset, self::basic_length_unit));
						$offset += self::basic_length_unit ;
						$pdubase.command_id = unpack("N", substr($recv_buff, $offset, self::basic_length_unit));
						$offset += self::basic_length_unit ;
						$pdubase.seq_id = unpack("N", substr($recv_buff, $offset, self::basic_length_unit));
						$offset += self::basic_length_unit ;
						$pdubase.length = unpack("N", substr($recv_buff, $offset, self::basic_length_unit));
						$offset += self::basic_length_unit ;
						if ($pdubase.length + $offset <= strlen($recv_buff)) {
							$pdubase.body = substr($recv_buff, $offset, $pdubase.length);
							return $pdubase ;
						}else{
							echo "need read more.";
						}
						
					}else{
						echo "not full pdu header , go on read.";
					}
			    }

	        }
    	}

    	public function close()
    	{
    		socket_close($socket);
    	}

	}

	
?>