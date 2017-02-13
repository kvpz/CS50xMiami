<?php
    /*
        This file will be contacted with a URL (such as ..search.php?=blah)
        and the code should produce a JSON array of all the places in the DB
        table that matches that input(blah).
        
    */
    require(__DIR__ . "/../includes/config.php");

    // numerically indexed array of places
    $places = [];

    $query_format = "Select * From places Where postal_code Like \"{QUERY}\" OR place_name LIKE \"{QUERY}\"";
    $query = str_replace("{QUERY}", $_GET["geo"] . "%", $query_format);

    //$places = CS50::query("SELECT * FROM places WHERE MATCH(postal_code, place_name, admin_name1, admin_code1, admin_name2) AGAINST (?) OR postal_code LIKE (?)", $_GET["geo"], $_GET["geo"]);
    $places = CS50::query($query);

    // output places as JSON (pretty-printed for debugging convenience)
    header("Content-type: application/json");
    print(json_encode($places, JSON_PRETTY_PRINT));

?>