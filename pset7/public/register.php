<?php
  // configuration
  require("../includes/config.php");
  
  // if user reached page via GET (as by clicking a link or via redirect)
  if ($_SERVER["REQUEST_METHOD"] == "GET")
  {
    // else render form
    render("register_form.php", ["title" => "Register"]);
  }
  // else if user reached via POST (as by submitting a form via POST)
  else if($_SERVER["REQUEST_METHOD"] == "POST")
  {
    if ($_POST["username"] == NULL)// || $_POST["password"] == NULL || $_POST["password"] != $_POST["confirmation"])
    {
      apologize("You must provide a username.");
    }
    else if(($_POST["password"] == NULL))
    {
      apologize("You must provide a password.");
    }
    else if($_POST["password"] != $_POST["confirmation"])
    {
      apologize("Those passwords did not match.");
    }
    else
    {
      // insert a new user if new username
      $row = CS50::query("INSERT IGNORE INTO users (username, hash, cash) VALUES(?, ?, 10000.0000)", 
                    $_POST["username"], password_hash($_POST["password"], PASSWORD_DEFAULT));
      print("Value of row: " . $row);
      if($row == 0)
      {
        // the INSERT failed, presumably because username already exists
        apologize("That username already exists.");
      }
      // else if insert (i.e. registration succeeded
      else
      {
        $rows = CS50::query("SELECT LAST_INSERT_ID() AS id");
        $id = $rows[0]["id"];
        $_SESSION = $id; //session_id($id);
        session_start();
        redirect("/index.php");
        // redirect user to main page (log in the new user)
        
      }
      
    }
  }
  
?>