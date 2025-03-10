<?php
	$text = $_GET["text"];
	$link = ufr_publisher("@new zmq:topic @coder msgpack @host 10.0.0.2 @port 5004");
	ufr_put($link, "s\n", $text);
	ufr_close($link);
	echo "OK\n";
?>
