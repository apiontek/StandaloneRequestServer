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
$is_dev = TRUE;

if (isset($_SERVER['REFERER'])) $referer = $_SERVER['REFERER'];
function siteheader()
{
  global $venueName;
  echo "<html>
  <head>
    <meta charset=\"UTF-8\">
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">
    <title>$venueName Songbook</title>

    <link rel=\"preconnect\" href=\"https://cdn.jsdelivr.net\" crossorigin>
    <link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/@xz/fonts@1.0.2/serve/metropolis.css\" integrity=\"sha256-EbF7uuhKup7x84hcfaVXj5gof+v/9hfv7Wsxud7vWyU=\" crossorigin=\"anonymous\">
    <link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/@fontsource/righteous@5.0.14/index.css\" integrity=\"sha256-3LZETteO5Rdt11DQIU0Uj0He3Tdr/JnfBZcsP87UifE=\" crossorigin=\"anonymous\">

    <link rel=\"stylesheet\" type=\"text/css\" href=\"css/style.css\" />

    <script defer src=\"https://cdn.jsdelivr.net/npm/htmx.org@1.9.6/dist/htmx.min.js\" integrity=\"sha256-y7cjwwXPbWMVyJCQmBVSNYhQni4JKln4z8SohYKWidU=\" crossorigin=\"anonymous\"></script>
  </head>
  <body
    hx-on:keydown=\"
      const reqModal = htmx.find('#req-modal');
      if (event.key === 'Escape' && reqModal)
        htmx.trigger(reqModal, 'remove-req-modal');
    \">";
}

function sitefooter() {
  echo "</main></body></html>";
}

function navbar()
{
  global $venueName;
  echo "<header><h1><a href='/' hx-boost='true'>$venueName Songbook</a></h1></header><main>";
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

function searchform($query = "")
{
  $disabled = "";
  if ($query == "") $disabled = "disabled";
  echo '<div id="song-search-input-container"><input
      id="song-search-input"
      type="search"
      name="q"
      value="' . $query . '"
      autofocus
      autocomplete="off"
      placeholder="Type here to search songs"
      hx-get="/search.php"
      hx-sync="this:replace"
      hx-push-url="true"
      hx-trigger="input changed delay:200ms, search"
      hx-target="#data-target"
      hx-select="#data-target"
      hx-on:clear-search="
        this.value = \'\';
        htmx.trigger(this, \'input\');
        htmx.find(\'#song-search-input-clear\').setAttribute(\'disabled\', \'true\');
      "
      hx-on:input="
        if (this.value.length > 0)
          htmx.find(\'#song-search-input-clear\').removeAttribute(\'disabled\');
        else
          htmx.find(\'#song-search-input-clear\').setAttribute(\'disabled\', \'true\');
        "
      >
      <button id="song-search-input-clear" type="reset" ' . $disabled . '
        hx-on:click="htmx.trigger(\'#song-search-input\', \'clear-search\')"
        >
        <svg xmlns="http://www.w3.org/2000/svg" fill="currentColor" class="bi bi-x-lg" viewBox="0 0 16 16">
          <path d="M2.146 2.854a.5.5 0 1 1 .708-.708L8 7.293l5.146-5.147a.5.5 0 0 1 .708.708L8.707 8l5.147 5.146a.5.5 0 0 1-.708.708L8 8.707l-5.146 5.147a.5.5 0 0 1-.708-.708L7.293 8 2.146 2.854Z"/>
        </svg>
      </button>
      </div>';
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


function reqFormContent($songid = "")
{
  echo "<input type=\"hidden\" name=\"songid\" value=\"$songid\">
  <label>Who will sing it?</label>
  <input type=\"text\" name=\"singer\"
    autocomplete=\"off\" autofocus
    placeholder=\"Enter your name or nickname\"
    hx-on:input=\"
      if (this.value.length > 0)
        htmx.find('#send-song-request').removeAttribute('disabled');
      else
        htmx.find('#send-song-request').setAttribute('disabled', 'true');
    \"
    >
  <div class=\"req-modal-buttons\">
    <button
      id=\"cancel-song-request\"
      type=\"button\"
      class=\"close\"
      hx-on:click=\"htmx.trigger('#req-modal', 'remove-req-modal');\"
      >Close</button>
    <button
      type=\"submit\"
      id=\"send-song-request\"
      disabled
      >Send Request</button>
  </div>";
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
