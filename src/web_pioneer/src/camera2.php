<?php
	$link = ufr_client("@new zmq:socket @coder msgpack @debug 0");
	ufr_put($link, "s\n\n", "camera2.jpg");

    	$val = ufr_get($link, "^b\n");
    	header("Content-Type: image/jpg");
	header("Content-Length: " . strlen($val[0]) );
    	print( $val[0] );
	ufr_close($link);
?>
