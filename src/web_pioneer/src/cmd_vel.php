<?php
    $link = ufr_client("@new zmq:socket @coder msgpack @debug 0");
    $vel = $_GET['vel'];
    $rot = $_GET['rot'];
    ufr_put($link, "sff\n\n", "cmd_vel", $vel, $rot);
    $val = ufr_get($link, "^is");
    print_r( $val );
    ufr_close($link);
?>