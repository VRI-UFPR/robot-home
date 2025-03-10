<?php
    $link = ufr_client("@new zmq:socket @coder msgpack @debug 0");
    ufr_put($link, "s\n\n", "odom");
    $val = ufr_get($link, "^fffff\n");
    ufr_close($link);
    echo json_encode($val);
?>
