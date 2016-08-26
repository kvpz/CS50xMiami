<?php
    /*
        This file will be contacted with a URL (such as ..search.php?=blah)
        and the code should produce a JSON array of all the places in the DB
        table that matches that input(blah).
        
    */
    require(__DIR__ . "/../includes/config.php");

    // numerically indexed array of places
    $places = [];

    // search database for places matching $_GET["geo"]; store in $places
    $places = CS50::query("SELECT * FROM places WHERE MATCH(postal_code, place_name) AGAINST (?)", $_GET["geo"]);
    
    // output places as JSON (pretty-printed for debugging convenience)
    header("Content-type: application/json");
    print(json_encode($places, JSON_PRETTY_PRINT));

?>