<?php
include("settings.inc");
$db = new PDO("sqlite:$dbFilePath");

$db->exec("CREATE TABLE IF NOT EXISTS songdb (song_id integer PRIMARY KEY AUTOINCREMENT, artist text, title  TEXT, combined TEXT UNIQUE)"); 
$db->exec("CREATE TABLE IF NOT EXISTS state (accepting bool, serial integer NOT NULL)");
$db->exec("INSERT OR IGNORE INTO state (rowid,accepting,serial) VALUES(0,0,1)");
$db->exec("CREATE UNIQUE INDEX IF NOT EXISTS idx_songstrings ON songdb(combined)");
$db->exec("CREATE TABLE IF NOT EXISTS requests (request_id integer PRIMARY KEY AUTOINCREMENT, artist TEXT, title TEXT, singer TEXT, request_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP)");

$path = parse_url($_SERVER['REQUEST_URI'], PHP_URL_PATH);
$songdbtable = "songdb";
$requeststable = "requests";

if (isset($_SERVER['REFERER'])) $referer = $_SERVER['REFERER']; 
function siteheader($title) 
{
  global $venueName;
  echo "<html>
  <head>
    <meta charset=\"UTF-8\">
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">
    <title>$venueName Songbook &middot; $title</title>
    <link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/@xz/fonts@1.0.2/serve/metropolis.min.css\">
    <link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/@fontsource/righteous@4.5.7/index.min.css\">
    <link rel=\"stylesheet\" type=\"text/css\" href=\"new.min.css\" />
    <link rel=\"stylesheet\" type=\"text/css\" href=\"venuestyle.css\" />
    <script type=\"text/javascript\">
          function submitreq(varid){
              window.location = \"./submitreq.php?id=\" + varid;
          }
    </script>
        </head><body>";
}

function sitefooter() {
  echo "</main></body></html>";
}

function navbar($backurl) 
{
  if ($backurl == "")
    $backurl = index.php;
  global $venueName;
  echo "<header><h1>$venueName Songbook</h1>";
  if ($_SERVER['REQUEST_URI'] != "/index.php" && $_SERVER['REQUEST_URI'] != "/") echo "<nav><a href=\"$backurl\">Back</a></nav>";
  echo "</header><main>";
}

function setAccepting($accepting)
{
  global $db;
  if ($accepting == 1)
  {
    echo("setting accepting to 1");
    $db->exec("UPDATE state SET accepting=1");
  }
  else
  {
    echo("setting accepting to 0");
    $db->exec("UPDATE state SET accepting=0");
  }
}

function getAccepting()
{
  global $db;
  $accepting = false;
        foreach ($db->query("SELECT accepting FROM state LIMIT 1") as $row)
        {
                $accepting = $row['accepting'];
        }
  return $accepting;
}

function searchform() 
{
  echo '<form method="get" action="search.php">
    <label>Song search</label>
    <input type="text" name="q" autofocus autocomplete="off">
    <input type="submit" value="Search">
  </form>';
  echo '<div id="data-target">';
  helpHints();
  echo '</div>';
}



function helpHints()
{
  echo '<details open>
    <summary>Instructions</summary>
    <p>Enter at least 3 characters (letters/numbers) as a search query.</p>
    <p>You may enter any part of the artist and/or title of the song, and partial words are allowed.</p>
    <p>For example, "pai bla stone" would match "Rolling Stones, The - Paint it black".</p>
  </details>';
  if (!getAccepting())
    echo '<details open>
      <summary>Requests closed</summary>
      <p>You will not be able to request songs at this time.</p>
    </details>';
}



function getSerial()
{
  global $db;
  $serial = -1;
  foreach ($db->query("SELECT serial FROM state LIMIT 1") as $row)
        {
          $serial = (int)$row['serial'];
        }
  return $serial;
}

function newSerial()
{
        global $db;
        $serial = getSerial();
        $newSerial = mt_rand(0,99999);
        while ($newSerial == $serial)
        {
                $newSerial = mt_rand(0,99999);
        }
        $db->exec("UPDATE state SET serial=$newSerial");
        return $newSerial;
}

function getVenue()
{
  // We don't really do multiple venues in standalone, just fake it
        global $db;
  global $venueName;
        $serial = -1;
  $venue = array();
  $venue['venue_id'] = $venue_id;
        $venue['accepting'] = getAccepting();
  $venue['name'] = $venueName;
  $venue['url_name'] = "none";
  return $venue;
}

function getVenues()
{
  // We don't really do multiple venues in standalone, just fake it
  global $db;
  global $venueName;
  $venues = array();
  $venue['venue_id'] = 0;
        $venue['accepting'] = getAccepting();
        $venue['name'] = $venueName;
  $venue['url_name'] = "none";
  $venues['venues'][] = $venue;
  return $venues;
}

function getRequests()
{
        global $db;
        $requests = array();
        $result = $db->query("SELECT request_id,artist,title,singer,strftime('%s', request_time) AS unixtime FROM requests");
  if ($result)
  {
          foreach ($result as $row)
          {
                  $request['request_id'] = (int)$row['request_id'];
                  $request['artist'] = $row['artist'];
                  $request['title'] = $row['title'];
                  $request['singer'] = $row['singer'];
                  $request['request_time'] = (int)$row['unixtime'];
                  $requests['requests'][] = $request;
          }
  }
        return $requests;
}


?>
