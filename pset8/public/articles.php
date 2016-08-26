<?php
    /*
        This file contacts Google News grabbing the info in RSS format.
        The RSS is parsed into XML and returned to browser in a JSON format.
        So this file basically allows populating the popup menus.
    */

    require(__DIR__ . "/../includes/config.php");

    // ensure proper usage
    if (empty($_GET["geo"]))
    {
        http_response_code(400);
        exit;
    }

    // get geography's articles
    $articles = lookup($_GET["geo"]);

    // output articles as JSON (pretty-printed for debugging convenience)
    header("Content-type: application/json");
    print(json_encode($articles, JSON_PRETTY_PRINT));

?>